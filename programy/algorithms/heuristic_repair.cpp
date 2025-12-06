#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp"
using namespace std;

namespace HeuristicIterativeRepairAlgorithm {
    size_t hash_automaton(const Automaton &automaton) {
        size_t h = 0;
        for (State s = 0; s < automaton.num_states; s++) {
            for (Alphabet a = 0; a < automaton.num_alphabet; a++) {
                State to = automaton.transition_function.get_transition(s, a);
                h ^= hash<size_t>()(((size_t)s << 32) ^ ((size_t)a << 16) ^ (size_t)to)
                     + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
            }
        }
        return h;
    }

    void random_perturbation(Automaton &automaton, int perturb_strength = 3) {
        for (int i = 0; i < perturb_strength; i++) {
            State from = rand() % automaton.num_states;
            Alphabet symbol = rand() % automaton.num_alphabet;
            State to = rand() % automaton.num_states;
            automaton.transition_function.set_transition(from, symbol, to);
        }
    }


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

        unordered_set<size_t> visited_hashes;
        visited_hashes.reserve(max_iterations * 2);

        const int max_restarts = 5;  
        int restarts = 0;

        RestartSearch:

        visited_hashes.clear();

        for (int iter = 0; iter < max_iterations; iter++) {

            size_t h = hash_automaton(automaton);
            if (visited_hashes.count(h)) {
                if (restarts >= max_restarts) {
                    cerr << "Local minimum (cycle) even after restarts.\n";
                    return false;
                }

                cerr << "Cycle detected — performing random perturbation.\n";
                random_perturbation(automaton, 3);
                restarts++;
                goto RestartSearch;
            }
            visited_hashes.insert(h);

            bool any_change = false;

            vector<vector<Alphabet>> invalid_samples;
            for (const auto &s : positive_samples)
                if (!simulate_sample(automaton, s, true)) invalid_samples.push_back(s);
            for (const auto &s : negative_samples)
                if (!simulate_sample(automaton, s, false)) invalid_samples.push_back(s);

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

                        int errors = 0;
                        for (const auto &s2 : positive_samples)
                            if (!simulate_sample(automaton, s2, true)) errors++;
                        for (const auto &s2 : negative_samples)
                            if (!simulate_sample(automaton, s2, false)) errors++;

                        if (errors < best_score) {
                            best_score = errors;
                            best_to = candidate;
                        }
                    }

                    automaton.transition_function.set_transition(current, symbol, best_to);
                    if (best_to != old_to) any_change = true;
                    current = best_to;
                }
            }

            if (!any_change) {
                if (restarts >= max_restarts) {
                    cerr << "Local minimum (plateau) even after restarts.\n";
                    return false;
                }
                cerr << "No improvement detected — performing random perturbation.\n";
                random_perturbation(automaton, 3);
                restarts++;
                goto RestartSearch;
            }
        }

        return false;
    }

    template<auto validate_automaton_func = validate_automaton>
    AlgorithmOutput run(const AlgorithmInput &input) {
        const auto &[broken_automaton, positive_samples, negative_samples] = input;
        Automaton automaton = broken_automaton;

        int num_samples =
            positive_samples.samples.size() +
            negative_samples.samples.size();

        int max_iterations = 10 * automaton.num_states * automaton.num_alphabet;

        const bool fixable =
            run_heuristic_repair<validate_automaton_func>(
                automaton,
                positive_samples,
                negative_samples,
                max_iterations
            );

        return {fixable, automaton};
    }
};
