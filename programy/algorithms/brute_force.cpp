#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"


namespace BruteForceAlgorithm {
    bool fix_automaton(
        Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const int missing_edges_idx,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
        if (missing_edges_idx == missing_edges.size()) {
            const bool valid = validate_automaton(
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

            const bool fixed = fix_automaton(
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

    pair<bool, Automaton> run(
        const Automaton &broken_automaton,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
        Automaton automaton = broken_automaton;

        vector<pair<State, Alphabet>> missing_edges;

        const bool automaton_fixable = fix_automaton(
            automaton,
            missing_edges,
            0,
            positive_samples,
            negative_samples
        );

        return {automaton_fixable, automaton};
    }
};
