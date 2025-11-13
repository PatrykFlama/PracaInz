#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../structures/structures.cpp"
#include "types.cpp"
#include "helpers/validate_automaton.cpp"
#include "helpers/misc.cpp"

namespace SavePrefixState {

bool validate_automaton_func(
    const Automaton &automaton,
    const Samples &positive_samples,
    const Samples &negative_samples,
    const vector<State> &final_prefix_states
) {
    for (size_t samples_idx = 0; samples_idx < positive_samples.samples.size(); samples_idx++) {
        if (automaton.accepting[final_prefix_states[samples_idx]] != true) {
            return false;
        }
    }

    for (size_t samples_idx = 0; samples_idx < negative_samples.samples.size(); samples_idx++) {
        if (automaton.accepting[final_prefix_states[positive_samples.samples.size() + samples_idx]] != false) {
            return false;
        }
    }

    return true;
}


void walk_samples(
    const Automaton &automaton,
    const Samples &samples,
    const vector<State> &from_states,
    vector<State> &to_states,
    const size_t samples_pos_offset=0
) {
    for (size_t samples_idx_ = 0; samples_idx_ <  samples.samples.size(); samples_idx_++) {
        const size_t samples_idx = samples_idx_ + samples_pos_offset;
        auto curr_state = from_states[samples_idx];
        const auto &sample = samples.samples[samples_idx_];

        for (size_t sample_ptr = 0; sample_ptr < sample.size(); sample_ptr++) {
            const auto edge = sample[sample_ptr];
            const auto next_state = automaton.transition_function.get_transition(curr_state, edge);

            if (next_state == automaton.transition_function.invalid_edge) {
                break;
            }

            curr_state = next_state;
        }

        to_states[samples_idx] = curr_state;
    }
}

void walk_samples(
    const Automaton &automaton,
    const Samples &positive_samples,
    const Samples &negative_samples,
    const vector<State> &from_states,
    vector<State> &to_states
) {
    walk_samples(automaton, positive_samples, from_states, to_states, 0);
    walk_samples(automaton, negative_samples, from_states, to_states, positive_samples.samples.size());
}


AlgorithmOutput run(
    const AlgorithmInput &input
) {
    const auto &[broken_automaton, positive_samples, negative_samples] = input;

    Automaton automaton = broken_automaton;

    vector<pair<State, Alphabet>> missing_edges;
    get_missing_edges(automaton, missing_edges);
    
    vector<State> missing_edges_to_state(missing_edges.size(), 0);
    int missing_edges_idx = 0;
    bool automata_fixed = false;

    //? prefix_state[mising edge][sample] = furthest reachable state without <missing edge>
    const auto samples_size = positive_samples.samples.size() + negative_samples.samples.size();
    vector<vector<State>> prefix_state(missing_edges.size() + 1, vector<State>(samples_size, automaton.start_state));


    walk_samples(automaton, positive_samples, negative_samples, prefix_state[0], prefix_state[0]);

    while (!automata_fixed) {
        // we set all edges - validate and if not valid we have to update previous configuration
        if (missing_edges_idx == (int)missing_edges.size()) {
            bool valid = true;

            for (size_t samples_idx = 0; samples_idx < positive_samples.samples.size(); samples_idx++) {
                if (automaton.accepting[prefix_state[missing_edges.size()][samples_idx]] != true) {
                    valid = false;
                    break;
                }
            }

            for (size_t samples_idx = 0; samples_idx < negative_samples.samples.size(); samples_idx++) {
                if (automaton.accepting[prefix_state[missing_edges.size()][positive_samples.samples.size() + samples_idx]] != false) {
                    valid = false;
                    break;
                }
            }

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

        // set edge
        const auto [from_state, symbol] = missing_edges[missing_edges_idx];
        const State to_state = missing_edges_to_state[missing_edges_idx];
        automaton.transition_function.set_transition(from_state, symbol, to_state);

        // update prefix states
        const auto &from_states = prefix_state[missing_edges_idx];
        auto &to_states = prefix_state[missing_edges_idx + 1];
        walk_samples(automaton, positive_samples, negative_samples, from_states, to_states);

        // move forward
        missing_edges_idx++;
    }

    return {automata_fixed, automaton};
}

}; // SavePrefixState
