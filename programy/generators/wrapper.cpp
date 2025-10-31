#pragma once

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wcatch-value="
#pragma GCC diagnostic ignored "-Wconversion"

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
#include "./helpers/remove_edges_from_samples.cpp"
#include "./generators.cpp"
#include "./types.cpp"
using namespace std;


static const function<Automaton(State, Alphabet)> AutomatonGenerateFunctions[] = {
    simple_automaton_generate,
    acyclic_automaton_generate,
    disjoint_cycles_automaton_generate
};


GenerateAutomatonOutput generateAutomaton(GenerateAutomatonInput input) {
    auto [type, num_states, alphabet_size, missing_edges, num_samples, sample_length, length_variance] = input;

    if (num_states * alphabet_size < missing_edges) {
        throw invalid_argument("Invalid number of missing edges");
    }

    Automaton automaton = AutomatonGenerateFunctions[type](num_states, alphabet_size);

    // generate samples from automaton at random
    bool generation_success = false;
    Samples positive_samples, negative_samples;

    while (!generation_success) {
        positive_samples.samples.clear();
        negative_samples.samples.clear();

        const auto length_variance_size = (uint)(length_variance * sample_length);
        for (uint i = 0; i < num_samples; i++) {
            auto length = sample_length + (rand() % (2 * length_variance_size + 1)) - length_variance_size;
            
            vector<Alphabet> sample;
            State current_state = automaton.start_state;
            while (length--) {
                auto symbol = (Alphabet)(rand() % alphabet_size);
                sample.push_back(symbol);
                current_state = automaton.transition_function.get_transition(current_state, symbol);
            }

            if (automaton.accepting[current_state])
                positive_samples.samples.push_back(sample);
            else
                negative_samples.samples.push_back(sample);
        }

        // remove edges from automaton randomly
        generation_success = true;
        try {
            remove_edges_from_samples(
                automaton, 
                positive_samples, 
                negative_samples, 
                missing_edges);
        } catch (underflow_error) {
            generation_success = false;
        }
    }

    return {{automaton, true}, {positive_samples, negative_samples}};
}

#pragma GCC diagnostic pop 
