#pragma once

#include <bits/stdc++.h>
#include <tuple>
using namespace std;
#include "../structures/structures.cpp"
#include "./types.cpp"

// JumpEntry: (state_reached, pos_reached_in_sample)
using JumpEntry = pair<State, int>;
// jump_table_sample[pos][state] = JumpEntry
using JumpTableForSample = vector<vector<JumpEntry>>;
//jump_table[sample_idx] -> JumpTableForSample
using JumpTable = vector<JumpTableForSample>;

JumpTable build_jump_table(const Automaton &automaton, const Samples &samples) {
    JumpTable table;
    table.reserve(samples.samples.size());

    for (const auto &sample : samples.samples) {
        const int L = sample.size();
        // table for this word: (L+1) positions, each pos has num_states entries
        JumpTableForSample dp(L + 1, vector<JumpEntry>(automaton.num_states));

        // before the jump: state and position remains the same, 
        for (State s = 0; s < automaton.num_states; s++) {
            dp[L][s] = {s, L};
        }

        for (int pos = L - 1; pos >= 0; pos--) {
            const Alphabet symbol = sample[pos];

            for (State st = 0; st < automaton.num_states; st++) {
                Alphabet trans = automaton.transition_function.get_transition(st, symbol);
                if (trans == automaton.transition_function.invalid_edge) {
                    // the unknown edge
                    dp[pos][st] = {st, pos};
                } else {
                    // jump
                    dp[pos][st] = dp[pos + 1][trans];
                }
            }
        }

        table.push_back(move(dp));
    }

    return table;
}
    
namespace ValidateAutomatonWithJumps {
    bool validate_automaton_w_jumps_helper(
        const Automaton &automaton,
        const Samples &samples,
        const bool expected_accepting
    ) {
        const JumpTable table = build_jump_table(automaton, samples);
        const State start = automaton.start_state;
        
        
        for (size_t i = 0; i < samples.samples.size(); i++) {
            const auto &dp = table[i];
            const int L = samples.samples[i].size();
            const auto res = dp[0][start];
            const State final_state = res.first;
            const int pos_reached = res.second;
        
        
            // sample is rejected if we couldnt jump through all sample
            if (pos_reached < L) {
                if (expected_accepting) return false;
                else continue;
            }
        
        
            const bool is_accepting = automaton.accepting[final_state];
            if (is_accepting != expected_accepting) {
                return false;
            } 
        }
    
        return true;
    }
};
    
    
bool validate_automaton_using_jumps(
    const Automaton &automaton,
    const Samples &positive_samples,
    const Samples &negative_samples
) {
    const bool positive_valid = ValidateAutomatonWithJumps::validate_automaton_w_jumps_helper(
        automaton, 
        positive_samples, 
        true
    );

    if (!positive_valid) return false;

    const bool negative_valid = ValidateAutomatonWithJumps::validate_automaton_w_jumps_helper(
        automaton, 
        negative_samples, 
        false
    );
    
    return negative_valid;
}
    
namespace PreprocessJumpsAlgorithm {
    void get_missing_edges(
        Automaton &automaton,
        vector<pair<State, Alphabet>> &missing_edges
    ) {
        for (State state = 0; state < automaton.num_states; state++) {
            for (Alphabet symbol = 0; symbol < automaton.num_alphabet; symbol++) {
                if (automaton.transition_function.get_transition(state, symbol) == automaton.transition_function.invalid_edge) {
                    missing_edges.push_back({state, symbol});
                }
            }
        }
    }

    bool fix_automaton_rec(
        Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const int missing_edges_idx,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
        if (missing_edges_idx == (int)missing_edges.size()) {
            const bool valid = validate_automaton_using_jumps(
                automaton,
                positive_samples,
                negative_samples
            );

            if (valid) return true;
            return false;
        }

        const auto [from_state, symbol] = missing_edges[missing_edges_idx];

        for (State to_state = 0; to_state < automaton.num_states; to_state++) {
            automaton.transition_function.set_transition(from_state, symbol, to_state);

            const bool fixed = fix_automaton_rec(
                automaton,
                missing_edges,
                missing_edges_idx + 1,
                positive_samples,
                negative_samples
            );

            if (fixed) return true;
        }

        // automaton.transition_function.set_transition(from_state, symbol, automaton.transition_function.invalid_edge);   // probably useless

        return false;
    }

    AlgorithmOutput run_rec(const AlgorithmInput &input) {
        const auto &[broken_automaton, positive_samples, negative_samples] = input;

        Automaton automaton = broken_automaton;

        vector<pair<State, Alphabet>> missing_edges;
        get_missing_edges(automaton, missing_edges);

        const bool automaton_fixable = fix_automaton_rec(
            automaton,
            missing_edges,
            0,
            positive_samples,
            negative_samples
        );

        return {automaton_fixable, automaton};
    }

    AlgorithmOutput run_iter(const AlgorithmInput &input) {
        const auto &[broken_automaton, positive_samples, negative_samples] = input;

        Automaton automaton = broken_automaton;

        vector<pair<State, Alphabet>> missing_edges;
        get_missing_edges(automaton, missing_edges);
        
        vector<State> missing_edges_to_state(missing_edges.size(), 0);
        int missing_edges_idx = 0;
        bool automata_fixed = false;

        while (!automata_fixed) {
            // we set all edges - validate and if not valid we have to update previous configuration
            if (missing_edges_idx == (int)missing_edges.size()) {
                const bool valid = validate_automaton_using_jumps(
                    automaton,
                    positive_samples,
                    negative_samples
                );

                if (valid) {
                    automata_fixed = true;
                    break;
                }

                missing_edges_idx--;
            }

            // we checked all states for this edge - we have to update previous confiugration
            if (missing_edges_to_state[missing_edges_idx] == automaton.num_states) {
                missing_edges_to_state[missing_edges_idx] = 0;
                missing_edges_idx--;
                if (missing_edges_idx < 0) break;
            }

            // set current edge to next state and move forward
            missing_edges_to_state[missing_edges_idx]++;
            if (missing_edges_to_state[missing_edges_idx] == automaton.num_states) continue;

            const auto [from_state, symbol] = missing_edges[missing_edges_idx];
            const State to_state = missing_edges_to_state[missing_edges_idx];
            automaton.transition_function.set_transition(from_state, symbol, to_state);

            missing_edges_idx++;
        }

        return {automata_fixed, automaton};
    }
};