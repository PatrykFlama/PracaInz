#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp"


namespace PruningExperimentAlgorithm {
    template<auto validate_automaton_func = validate_automaton>
    bool fix_automaton_rec(
        Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const size_t missing_edges_idx,
        const pair<Samples, Samples> &positive_samples,
        const pair<Samples, Samples> &negative_samples,
        std::atomic<bool>* stop_flag
    ) {
        if (should_stop(stop_flag)) return false;

        if (missing_edges_idx == missing_edges.size()) {
            const bool valid = validate_automaton_func(
                automaton,
                positive_samples.first,
                negative_samples.first
            );

            if (!valid) return false;

            return validate_automaton_func(
                automaton,
                positive_samples.second,
                negative_samples.second
            );
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

    template<typename SplitStrategy>
    pair<Samples, Samples> split_samples(const Samples &samples, SplitStrategy split_strategy) {
        const size_t total_samples = samples.samples.size();
        const size_t mid = split_strategy(total_samples);

        Samples first_part, second_part;
        first_part.samples.insert(first_part.samples.end(), samples.samples.begin(), samples.samples.begin() + mid);
        second_part.samples.insert(second_part.samples.end(), samples.samples.begin() + mid, samples.samples.end());

        return {first_part, second_part};
    }

    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run(
        const AlgorithmInput &input
    ) {
        const auto &[broken_automaton, positive_samples, negative_samples, stop_flag] = input;

        auto split_strategy = [&](size_t total_samples) -> size_t {
            return (size_t)min(pow(broken_automaton.num_states, 2), (double)total_samples);
        };

        const auto pos_samples = split_samples(
            positive_samples,
            split_strategy
        );
        const auto neg_samples = split_samples(
            negative_samples,
            split_strategy
        );

        Automaton automaton = broken_automaton;
        vector<pair<State, Alphabet>> missing_edges;

        get_missing_edges(automaton, missing_edges);

        const bool automaton_fixable = fix_automaton_rec<validate_automaton_func>(
            automaton,
            missing_edges,
            0,
            pos_samples,
            neg_samples,
            input.stop_flag
        );

        return {automaton_fixable, automaton};
    }
};  // namespace PruningExperimentAlgorithm
