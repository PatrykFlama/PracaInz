#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"


namespace BruteForceAlgorithm {
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

    pair<bool, Automaton> run_rec(
        const Automaton &broken_automaton,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
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


    pair<bool, Automaton> run_iter(
        const Automaton &broken_automaton,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
        Automaton automaton = broken_automaton;

        vector<pair<State, Alphabet>> missing_edges;
        get_missing_edges(automaton, missing_edges);
        
        vector<State> missing_edges_to_state(missing_edges.size(), 0);
        int missing_edges_idx = 0;
        bool automata_fixed = false;

        while (!automata_fixed) {
            // we set all edges - validate and if not valid we have to update previous configuration
            if (missing_edges_idx == (int)missing_edges.size()) {
                const bool valid = validate_automaton(
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
