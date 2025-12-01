#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp"

namespace HeuristicIterativeRepairAlgorithm {
    //checking if sample has any wrong transition
    bool simulate_sample(const Automaton &automaton, const vector<Alphabet> &sample, bool positive) {
        State current = automaton.start_state;
        for (Alphabet symbol : sample) {
            State next = automaton.transition_function.get_transition(current, symbol);
            if (next == automaton.transition_function.invalid_edge) return false;
            current = next;
        }
        bool accepted = automaton.accepting[current];

        return positive ? accepted : !accepted;
    }

    template<auto validate_automaton_func = validate_automaton>
    bool run_heuristic_repair(
        Automaton &automaton,
        const Samples &positive_samples,
        const Samples &negative_samples,
        int max_iterations
    ) {
        vector<pair<State, Alphabet>> missing_edges;
        get_missing_edges(automaton, missing_edges);

        for (auto &[from, symbol] : missing_edges) {
            automaton.transition_function.set_transition(from, symbol, rand() % automaton.num_states);
        }

        for (int iter = 0; iter < max_iterations; iter++) {
            bool any_change = false;

            vector<vector<Alphabet>> invalid_samples;
            for (const auto &s : positive_samples) {
                if (!simulate_sample(automaton, s, true)) {
                    invalid_samples.push_back(s);
                }
            }
            for (const auto &s : negative_samples) {
                if (!simulate_sample(automaton, s, false)) {
                    invalid_samples.push_back(s);
                }
            }

            if (invalid_samples.empty()) return true;

            for (const auto &sample : invalid_samples) {
                State current = automaton.start_state;
                for (Alphabet symbol : sample) {
                    State old_to = automaton.transition_function.get_transition(current, symbol);
                    State best_to = old_to;
                    int best_score = invalid_samples.size();

                    for (State candidate = 0; candidate < automaton.num_states; candidate++) {
                        if (candidate == old_to) continue;
                        automaton.transition_function.set_transition(current, symbol, candidate);

                        //counting invalid samples after change
                        int errors = 0;
                        for (const auto &s2 : positive_samples) {
                            if (!simulate_sample(automaton, s2, true)) {
                                errors++;
                            }
                        }
                        for (const auto &s2 : negative_samples) {
                            if (!simulate_sample(automaton, s2, false)) {
                                errors++;
                            }
                        }

                        if (errors < best_score) {
                            best_score = errors;
                            best_to = candidate;
                        }
                    }

                    if (best_to != old_to) {
                        automaton.transition_function.set_transition(current, symbol, best_to);
                        any_change = true;
                        current = best_to;
                    } else {
                        automaton.transition_function.set_transition(current, symbol, old_to);
                        current = old_to;
                    }
                }
            }

            if (!any_change) break;
        }

        for (const auto &s : positive_samples) {
            if (!simulate_sample(automaton, s, true)) {
                return false;
            }
        }

        for (const auto &s : negative_samples) {
            if (!simulate_sample(automaton, s, false)) {
                return false;
            }
        }
        return true;
    }

    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run(
        const AlgorithmInput &input
    ) {
        const auto &[broken_automaton, positive_samples, negative_samples] = input;
        Automaton automaton = broken_automaton;

        int max_iterations = 500;

        const bool fixable = run_heuristic_repair<validate_automaton_func>(
            automaton,
            positive_samples,
            negative_samples,
            max_iterations
        );

        return {fixable, automaton};
    }
};
