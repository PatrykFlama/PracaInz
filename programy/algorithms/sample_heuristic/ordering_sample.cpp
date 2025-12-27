#pragma once

#include <bits/stdc++.h>
using namespace std;
#include "../brute_force.cpp"
#include "../types.cpp"
#include "../helpers/misc.cpp"
#include "./weighted_sample.cpp"

namespace OrderingSamplesAlgorithm {
    int count_missing_edges_used(
        Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        const vector<Alphabet> &sample
    ) {
        int count = 0;
        State current = automaton.start_state;

        for (Alphabet symbol : sample) {
            
            bool is_missing = false;

            for (const auto &e : missing_edges) {
                if (e.first == current && e.second == symbol) {
                    is_missing = true;
                    break;
                }
            }

            if (is_missing) count++;

            State next = automaton.transition_function.get_transition(current, symbol);
            if (next == automaton.transition_function.invalid_edge) break;

            current = next;
        }
        return count;
    }

    void reorder_samples(
        const Automaton &automaton,
        const vector<pair<State, Alphabet>> &missing_edges,
        Samples &positive_samples,
        Samples &negative_samples
    ) {
        vector<WeightedSample> all;

        for (auto &s : positive_samples) {
            WeightedSample ws;
            ws.sample = s;
            ws.is_positive = true;
            ws.length = s.size();
            ws.missing_count = count_missing_edges_used(
                const_cast<Automaton&>(automaton),
                missing_edges,
                s
            );
            all.push_back(ws);
        }

        for (auto &s : negative_samples) {
            WeightedSample ws;
            ws.sample = s;
            ws.is_positive = false;
            ws.length = s.size();
            ws.missing_count = count_missing_edges_used(
                const_cast<Automaton&>(automaton),
                missing_edges,
                s
            );
            all.push_back(ws);
        }

        sort(all.begin(), all.end());

        positive_samples.samples.clear();
        negative_samples.samples.clear();

        for (auto &ws : all) {
            if (ws.is_positive) {
                positive_samples.samples.push_back(ws.sample);
            }
            else {
                negative_samples.samples.push_back(ws.sample);
            }
        }
    }

    AlgorithmInput reorder_input_samples(const AlgorithmInput &input) {
        AlgorithmInput output = input;

        vector<pair<State, Alphabet>> missing_edges;
        get_missing_edges(output.broken_automaton, missing_edges);

        reorder_samples(
            output.broken_automaton,
            missing_edges,
            output.positive_samples,
            output.negative_samples
        );

        return output;
    }


    AlgorithmOutput run_rec(const AlgorithmInput& input) {
        AlgorithmInput reordered = reorder_input_samples(input);
        return BruteForceAlgorithm::run_rec<>(reordered);
    }

    AlgorithmOutput run_iter(const AlgorithmInput& input) {
        AlgorithmInput reordered = reorder_input_samples(input);
        return BruteForceAlgorithm::run_iter<>(reordered);
    }
}

