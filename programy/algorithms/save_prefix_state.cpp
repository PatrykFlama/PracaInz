/*
Save-prefix-state algorithm: iterative backtracking that caches, for each depth and sample,
the furthest reachable state and position without using not-yet-assigned missing edges.
*/

#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../structures/structures.cpp"
#include "types.cpp"
#include "helpers/validate_automaton.cpp"
#include "helpers/misc.cpp"

namespace SavePrefixState {

static bool validate_automaton_func(
    const Automaton &automaton,
    const Samples &pos_samples,
    const Samples &neg_samples,
    const vector<State> &final_states
) {
    for (size_t i = 0; i < pos_samples.samples.size(); ++i) {
        // if (final_pos[i] != pos_samples.samples[i].size()) return false;    // probably not needed
        const auto s = final_states[i];
        // if (s == automaton.transition_function.invalid_edge) return false;  // probably not needed
        if (automaton.accepting[s] != true) return false;
    }

    for (size_t j = 0; j < neg_samples.samples.size(); ++j) {
        const size_t idx = pos_samples.samples.size() + j;
        // if (final_pos[idx] != neg_samples.samples[j].size()) return false;  // probably not needed
        const auto s = final_states[idx];
        // if (s == automaton.transition_function.invalid_edge) return false;  // probably not needed
        if (automaton.accepting[s] != false) return false;
    }

    return true;
}

static void walk_samples(
    const Automaton &automaton,
    const Samples &samples,
    const vector<State> &from_states,
    const vector<size_t> &from_pos,
    vector<State> &to_states,
    vector<size_t> &to_pos,
    const size_t samples_pos_offset = 0
) {
    for (size_t i = 0; i < samples.samples.size(); i++) {
        const auto merged_idx = i + samples_pos_offset;
        auto curr_state = from_states[merged_idx];
        auto curr_pos = from_pos[merged_idx];
        const auto &sample = samples.samples[i];

        // walk as far as possible through the automaton with sample
        while (curr_pos < sample.size()) {
            // if (curr_state == automaton.transition_function.invalid_edge) break;

            const auto edge = sample[curr_pos];
            const auto next_state = automaton.transition_function.get_transition(curr_state, edge);
            if (next_state == automaton.transition_function.invalid_edge) break;
            curr_state = next_state;
            curr_pos++;
        }

        to_states[merged_idx] = curr_state;
        to_pos[merged_idx] = curr_pos;
    }
}

static void walk_samples_combined(
    const Automaton &automaton,
    const Samples &pos_samples,
    const Samples &neg_samples,
    const vector<State> &from_states,
    const vector<size_t> &from_pos,
    vector<State> &to_states,
    vector<size_t> &to_pos
) {
    walk_samples(automaton, pos_samples, from_states, from_pos, to_states, to_pos, 0);
    walk_samples(automaton, neg_samples, from_states, from_pos, to_states, to_pos, pos_samples.samples.size());
}

AlgorithmOutput run(const AlgorithmInput &input) {
    const auto &[broken_automaton, positive_samples, negative_samples] = input;

    Automaton automaton = broken_automaton;

    vector<pair<State, Alphabet>> missing_edges;
    get_missing_edges(automaton, missing_edges);
    const size_t m = missing_edges.size();

    const size_t samples_size = positive_samples.samples.size() + negative_samples.samples.size();

    //? prefix_state[d][s] = state of sample s after walking on automaton with first d missing edges assigned
    vector<vector<State>> prefix_state(missing_edges.size() + 1, vector<State>(samples_size, automaton.start_state));
    //? prefix_pos[d][s] = position of sample s after walking on automaton with first d missing edges assigned
    vector<vector<size_t>> prefix_pos(missing_edges.size() + 1, vector<size_t>(samples_size, 0));
    walk_samples_combined(automaton, positive_samples, negative_samples, prefix_state[0], prefix_pos[0], prefix_state[0], prefix_pos[0]);

    vector<State> assigned(m, SIZE_MAX);

    size_t missing_edges_idx = 0;
    while (missing_edges_idx >= 0) {
        if (missing_edges_idx == m) {
            if (validate_automaton_func(automaton, positive_samples, negative_samples, prefix_state[m])) {
                return {true, automaton};
            }

            missing_edges_idx--;
            continue;
        }

        assigned[missing_edges_idx]++;

        if (assigned[missing_edges_idx] >= automaton.num_states) {
            // exhausted options -> reset transition to invalid and backtrack
            const auto [from, edge] = missing_edges[missing_edges_idx];
            automaton.transition_function.set_transition(from, edge, automaton.transition_function.invalid_edge);
            assigned[missing_edges_idx] = SIZE_MAX;
            missing_edges_idx--;
            continue;
        }

        // set transition for this missing edge
        const auto [from_state, edge] = missing_edges[missing_edges_idx];
        automaton.transition_function.set_transition(from_state, edge, assigned[missing_edges_idx]);

        // update prefix_state/prefix_pos at depth missing_edges_idx+1 for all samples
        walk_samples_combined(
            automaton,
            positive_samples,
            negative_samples,
            prefix_state[missing_edges_idx],
            prefix_pos[missing_edges_idx],
            prefix_state[missing_edges_idx + 1],
            prefix_pos[missing_edges_idx + 1]
        );

        // move deeper
        missing_edges_idx++;
    }

    // not fixable
    return {false, broken_automaton};
}

} // namespace SavePrefixState
