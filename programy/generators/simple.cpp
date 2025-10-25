#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
#include "./helpers/remove_edges_from_samples.cpp"
using namespace std;

pair<pair<Automaton, bool>, pair<Samples, Samples>> simple_generate(
    int num_states,
    int alphabet_size,
    int missing_edges,
    int num_samples,
    int sample_length,
    float length_variance
) {
    // generate automaton
    Automaton automaton(
        num_states,
        alphabet_size
    );

    automaton.start_state = 0;

    for (State from_state = 0; from_state < num_states; from_state++) {
        for (Alphabet symbol = 0; symbol < alphabet_size; symbol++) {
            State to_state = rand() % num_states;
            automaton.transition_function.set_transition(from_state, symbol, to_state);
        }

        automaton.accepting[from_state] = (rand() % 2 == 0);
    }
    
    // generate samples from automaton at random
    Samples positive_samples;
    Samples negative_samples;

    const int length_variance_size = (int)(length_variance * sample_length);
    for (int i = 0; i < num_samples; i++) {
        int length = sample_length + (rand() % (2 * length_variance_size + 1)) - length_variance_size;

        vector<Alphabet> sample;
        State current_state = automaton.start_state;
        while (length--) {
            Alphabet symbol = rand() % alphabet_size;
            sample.push_back(symbol);
            current_state = automaton.transition_function.get_transition(current_state, symbol);
        }

        if (automaton.accepting[current_state]) {
            positive_samples.samples.push_back(sample);
        } else {
            negative_samples.samples.push_back(sample);
        }
    }

    // remove edges from automaton randomly
    remove_edges_from_samples(
        automaton,
        positive_samples,
        missing_edges
    );

    return {{automaton, true}, {positive_samples, negative_samples}};
}
