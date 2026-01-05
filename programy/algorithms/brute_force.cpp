#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp"


namespace BruteForceAlgorithm {
    template<auto validate_automaton_func = validate_automaton>
    bool fix_automaton_rec(
        Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const size_t missing_edges_idx,
        const Samples &positive_samples,
        const Samples &negative_samples,
        std::atomic<bool>* stop_flag
    ) {
        if (should_stop(stop_flag)) return false;

        if (missing_edges_idx == missing_edges.size()) {
            const bool valid = validate_automaton_func(
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

            const bool fixed = fix_automaton_rec<validate_automaton_func>(
                automaton,
                missing_edges,
                missing_edges_idx + 1,
                positive_samples,
                negative_samples,
                stop_flag
            );

            if (fixed) return true;
        }

        // automaton.transition_function.set_transition(from_state, symbol, automaton.transition_function.invalid_edge);   // probably useless

        return false;
    }

    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run_rec(
        const AlgorithmInput &input
    ) {
        const auto &[broken_automaton, positive_samples, negative_samples, stop_flag] = input;

        Automaton automaton = broken_automaton;
        vector<pair<State, Alphabet>> missing_edges;

        get_missing_edges(automaton, missing_edges);

        const bool automaton_fixable = fix_automaton_rec<validate_automaton_func>(
            automaton,
            missing_edges,
            0,
            positive_samples,
            negative_samples,
            input.stop_flag
        );

        return {automaton_fixable, automaton};
    }

    template<auto validate_automaton_func = validate_automaton>
    bool fix_automaton_it(
        Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const Samples &positive_samples,
        const Samples &negative_samples,
        std::atomic<bool>* stop_flag
    ) {
        const size_t K = missing_edges.size();
        const size_t N = automaton.num_states;

        if (K == 0) {
            return validate_automaton_func(automaton, positive_samples, negative_samples);
        }

        vector<State> index(K, 0);

        while (true) {
            if (should_stop(stop_flag)) return false;
            for (State i = 0; i < K; i++) {
                const auto &[from_state, symbol] = missing_edges[i];
                automaton.transition_function.set_transition(from_state, symbol, index[i]);
            }

            if (validate_automaton_func(automaton, positive_samples, negative_samples)) {
                return true;
            }

            size_t pos = K - 1;
            while (pos >= 0) {
                index[pos]++;
                if (index[pos] < N) {
                    break; 
                }
                index[pos] = 0; 
                pos--;
            }

            if (pos < 0) {
                break;
            }
        }

        return false;
    }


    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run_iter(
        const AlgorithmInput &input
    ) {
        const auto &[broken_automaton, positive_samples, negative_samples, stop_flag] = input;

        Automaton automaton = broken_automaton;
        vector<pair<State, Alphabet>> missing_edges;

        get_missing_edges(automaton, missing_edges);

        const bool fixable = fix_automaton_it<validate_automaton_func>(
            automaton,
            missing_edges,
            positive_samples,
            negative_samples,
            input.stop_flag
        );

        return {fixable, automaton};
    }
};
