#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp"

namespace HeuristicIterativeRepairAlgorithm {
    bool simulate_sample(
        const Automaton &automaton,
        const vector<Alphabet> &sample,
        bool positive
    ) {
        State current = automaton.start_state;

        for (Alphabet symbol : sample) {
            State next =
                automaton.transition_function.get_transition(current, symbol);
            if (next == automaton.transition_function.invalid_edge)
                return false;
            current = next;
        }

        bool accepted = automaton.accepting[current];
        return positive ? accepted : !accepted;
    }

    template<auto validate_automaton_func = validate_automaton>
    bool hill_climb(
        Automaton &automaton,
        const Samples &positive_samples,
        const Samples &negative_samples,
        int max_iterations,
        std::atomic<bool>* stop_flag
    ) {
        for (int iter = 0; iter < max_iterations; iter++) {
            bool any_change = false;
            vector<vector<Alphabet>> invalid_samples;

            for (const auto &s : positive_samples) {
                if (should_stop(stop_flag)) return false;
                if (!simulate_sample(automaton, s, true)) {
                    invalid_samples.push_back(s);
                }
            }

            for (const auto &s : negative_samples) {
                if (should_stop(stop_flag)) return false;
                if (!simulate_sample(automaton, s, false)) {
                    invalid_samples.push_back(s);
                }
            }

            if (invalid_samples.empty()) {
                return true;
            }

            for (const auto &sample : invalid_samples) {
                if (should_stop(stop_flag)) return false;
                State current = automaton.start_state;

                for (Alphabet symbol : sample) {
                    State old_to = automaton.transition_function.get_transition(current, symbol);

                    State best_to = old_to;
                    size_t best_score = invalid_samples.size();

                    for (State candidate = 0;candidate < automaton.num_states;candidate++) {
                        if (candidate == old_to) continue;

                        automaton.transition_function.set_transition(current, symbol, candidate);

                        size_t errors = 0;
                        for (const auto &s2 : positive_samples) {
                            errors += !simulate_sample(automaton, s2, true);
                        }
                        for (const auto &s2 : negative_samples) {
                            errors += !simulate_sample(automaton, s2, false);
                        }

                        if (errors < best_score) {
                            best_score = errors;
                            best_to = candidate;
                        }
                    }

                    automaton.transition_function.set_transition(current, symbol, best_to);

                    if (best_to != old_to) {
                        any_change = true;
                    }
                    current = best_to;
                }
            }

            if (!any_change) {
                break;
            }
        }

        const bool valid = validate_automaton_func(
                automaton,
                positive_samples,
                negative_samples
            );

        if (valid) return true;
        return false;
    }

    template<auto validate_automaton_func = validate_automaton>
    bool run_with_random_restarts(
        Automaton &result,
        const Automaton &broken_automaton,
        const Samples &positive_samples,
        const Samples &negative_samples,
        int max_iterations,
        int max_restarts,
        std::atomic<bool>* stop_flag
    ) {
        for (int restart = 0; restart < max_restarts; restart++) {
            if (should_stop(stop_flag)) return false;
            Automaton automaton = broken_automaton;

            vector<pair<State, Alphabet>> missing_edges;
            get_missing_edges(automaton, missing_edges);

            for (auto &[from, symbol] : missing_edges) {
                automaton.transition_function.set_transition(from, symbol, rand() % automaton.num_states);
            }

            bool success = hill_climb<validate_automaton_func>(
                automaton,
                positive_samples,
                negative_samples,
                max_iterations,
                stop_flag
            );

            if (success) {
                result = automaton;
                return true;
            }
        }

        return false;
    }

    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run(
        const AlgorithmInput &input
    ) {
        const auto &[broken_automaton, positive_samples, negative_samples, stop_flag] = input;

        Automaton result = broken_automaton;
        int max_iterations = 300;
        int max_restarts   = 50;

        bool fixable = run_with_random_restarts<validate_automaton_func>
        (
            result,
            broken_automaton,
            positive_samples,
            negative_samples,
            max_iterations,
            max_restarts,
            input.stop_flag
        );

        return {fixable, result};
    }
} 
