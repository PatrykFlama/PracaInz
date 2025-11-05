#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./brute_force.cpp"
#include "./helpers/misc.cpp"
#include "./types.cpp"

namespace PreprocessJumpsAlgorithm {
    // JumpEntry: (state_reached, pos_reached_in_sample)
    using JumpEntry = pair<State, int>;
    // jump_table_sample[pos][state] = JumpEntry
    using JumpTableForSample = vector<vector<JumpEntry>>;
    // jump_table[sample_idx] -> JumpTableForSample
    using JumpTable = vector<JumpTableForSample>;

    JumpTable build_jump_table(const Automaton& automaton, const Samples& samples) {
        JumpTable table;
        table.reserve(samples.samples.size());

        for (const auto& sample : samples.samples) {
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

    bool validate_automaton_w_jumps_helper(
        const Automaton& automaton,
        const Samples& samples,
        const bool expected_accepting) {
        const JumpTable table = build_jump_table(automaton, samples);
        const State start = automaton.start_state;

        for (size_t i = 0; i < samples.samples.size(); i++) {
            const auto& dp = table[i];
            const int L = samples.samples[i].size();
            const auto res = dp[0][start];
            const State final_state = res.first;
            const int pos_reached = res.second;

            // sample is rejected if we couldnt jump through all samples
            if (pos_reached < L) {
                if (expected_accepting)
                    return false;
                else
                    continue;
            }

            const bool is_accepting = automaton.accepting[final_state];
            if (is_accepting != expected_accepting) {
                return false;
            }
        }

        return true;
    }

    bool validate_automaton_using_jumps(
        const Automaton& automaton,
        const Samples& positive_samples,
        const Samples& negative_samples) {
        const bool positive_valid = validate_automaton_w_jumps_helper(
            automaton,
            positive_samples,
            true);

        if (!positive_valid) return false;

        const bool negative_valid = validate_automaton_w_jumps_helper(
            automaton,
            negative_samples,
            false);

        return negative_valid;
    }

    AlgorithmOutput run_rec(const AlgorithmInput& input) {
        return BruteForceAlgorithm::run_rec<validate_automaton_using_jumps>(input);
    }

    AlgorithmOutput run_iter(const AlgorithmInput& input) {
        return BruteForceAlgorithm::run_iter<validate_automaton_using_jumps>(input);
    }
};  // namespace PreprocessJumpsAlgorithm
