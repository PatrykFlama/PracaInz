#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp"

namespace EdgeOrderingAlgorithm {
    pair<State, Alphabet> pick_most_constraining_edge(
        const Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
        unordered_map<long long, int> counter; 
        auto encode = [&](State s, Alphabet a) {
            return (static_cast<long long>(s) << 32) |
                static_cast<long long>(a);
        };

        auto simulate = [&](const Samples &S){
            for (const auto &sample : S.samples) {
                State s = automaton.start_state;

                for (Alphabet a : sample) {
                    State t = automaton.transition_function.get_transition(s, a);
                    if (t == automaton.transition_function.invalid_edge) {
                        counter[encode(s, a)]++;
                        return;
                    }
                    s = t;
                }
            }
        };

        simulate(positive_samples);
        simulate(negative_samples);

        pair<State, Alphabet> best = missing_edges[0];
        int best_score = -1;

        for (auto &e : missing_edges) {
            int score = counter[encode(e.first, e.second)];
            if (score > best_score) {
                best_score = score;
                best = e;
            }
        }

        return best;
    }


    template<auto validate_automaton_func = validate_automaton>
    bool fix_automaton_rec_bt(
        Automaton &automaton,
        const Samples &positive_samples,
        const Samples &negative_samples
    ) {
        vector<pair<State, Alphabet>> missing_edges;
        get_missing_edges(automaton, missing_edges);

        if (missing_edges.empty()) {
            return validate_automaton_func(automaton, positive_samples, negative_samples);
        }

        auto chosen = pick_most_constraining_edge(
            automaton, missing_edges, positive_samples, negative_samples
        );

        auto [from, symbol] = chosen;

        for (State to = 0; to < automaton.num_states; to++) {
            automaton.transition_function.set_transition(from, symbol, to);

            if (fix_automaton_rec_bt<validate_automaton_func>(
                    automaton, positive_samples, negative_samples)) {
                return true;
            }
        }

        automaton.transition_function.set_transition(
            from, symbol, automaton.transition_function.invalid_edge);

        return false;
    }

    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run_backtracking_dynamic(
        const AlgorithmInput &input
    ) {
        const auto &[broken_automaton, positive_samples, negative_samples] = input;

        Automaton automaton = broken_automaton;

        const bool fixable = fix_automaton_rec_bt<validate_automaton_func>(
            automaton,
            positive_samples,
            negative_samples
        );

        return {fixable, automaton};
    }
}