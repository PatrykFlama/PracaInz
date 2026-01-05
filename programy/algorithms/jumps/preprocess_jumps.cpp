#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "./jump_tables.cpp"
#include "../brute_force.cpp"
#include "../helpers/misc.cpp"
#include "../types.cpp"

namespace PreprocessJumpsAlgorithm {
    JumpTables jump_tables;

    JumpTable &build_jump_table(JumpTable &table, const Automaton& automaton, const Samples& samples) {
        table.clear();
        table.reserve(samples.samples.size());

        for (const auto& sample : samples.samples) {
            const size_t L = sample.size();
            // table for this word: (L+1) positions, each pos has num_states entries
            JumpTableForSample dp(L + 1, vector<JumpEntry>(automaton.num_states));

            // before the jump: state and position remains the same,
            for (State s = 0; s < automaton.num_states; s++) {
                dp[L][s] = JumpEntry(s, L);
            }

            for (size_t pos = L; pos-- > 0;) {
                const Alphabet symbol = sample[pos];

                for (State st = 0; st < automaton.num_states; st++) {
                    State trans = automaton.transition_function.get_transition(st, symbol);
                    if (trans == automaton.transition_function.invalid_edge) {
                        // the unknown edge
                        dp[pos][st] = JumpEntry(st, pos);
                    } else {
                        // jump
                        dp[pos][st] = dp[pos + 1][trans];
                    }
                }
            }

            table.push_back(std::move(dp));
        }

        return table;
    }

    bool validate_automaton_w_jumps_helper(
        const Automaton& automaton,
        const Samples& samples,
        const bool expected_accepting,
        const JumpTable& table
    ) {
        const State start = automaton.start_state;
    
        for (size_t i = 0; i < samples.samples.size(); i++) {
            const auto& sample = samples.samples[i];
            const size_t L = sample.size();
            const auto& dp = table[i];
    
            State st = start;
            size_t pos = 0;
            bool valid_sample = true;
    
            while (pos < L) {
                // jump
                const JumpEntry jump = dp[pos][st];
                State new_st = jump.state_reached;
                size_t new_pos = jump.pos_reached_in_sample;
    
                // if we are on invalid edge, use get_transition and try to jump once again
                if (new_pos == pos && new_st == st) {
                    const Alphabet sym = sample[pos];
                    const State next = automaton.transition_function.get_transition(st, sym);
    
                    if (next == automaton.transition_function.invalid_edge) {
                        valid_sample = false;
                        break;
                    }
    
                    st = next;
                    pos++;
                    continue;
                }
    
                st = new_st;
                pos = new_pos;
            }
    
            if (!valid_sample)
                continue;
    
            const bool is_accepting = automaton.accepting[st];
            if (is_accepting != expected_accepting) {
                return false;
            }
        }
    
        return true;
    }  

    bool validate_automaton_using_jumps(
        const Automaton& automaton,
        const Samples& positive_samples,
        const Samples& negative_samples
    ) {
        const bool positive_valid = validate_automaton_w_jumps_helper(
            automaton,
            positive_samples,
            true,
            jump_tables.positive
        );

        if (!positive_valid) return false;

        const bool negative_valid = validate_automaton_w_jumps_helper(
            automaton,
            negative_samples,
            false,
            jump_tables.negative
        );

        return negative_valid;
    }
        

    void build_jump_tables(const AlgorithmInput& input) {
        const auto& [broken_automaton, positive_samples, negative_samples, stop_flag] = input;
        build_jump_table(jump_tables.negative, broken_automaton, negative_samples);
        build_jump_table(jump_tables.positive, broken_automaton, positive_samples);
    }

    AlgorithmOutput run_rec(const AlgorithmInput& input) {
        build_jump_tables(input);
        return BruteForceAlgorithm::run_rec<validate_automaton_using_jumps>(input);
    }

    AlgorithmOutput run_iter(const AlgorithmInput& input) {
        build_jump_tables(input);
        return BruteForceAlgorithm::run_iter<validate_automaton_using_jumps>(input);
    }

};
