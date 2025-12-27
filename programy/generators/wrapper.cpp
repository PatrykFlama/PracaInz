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

struct AutomatonGenerator {
    virtual Automaton operator()(State n, Alphabet a) const = 0;
    virtual ~AutomatonGenerator() = default;
};

struct SimpleGenerator : AutomatonGenerator {
    Automaton operator()(State n, Alphabet a) const override {
        return simple_automaton_generate(n, a);
    }
};

struct KSCCGenerator : AutomatonGenerator {
    uint k;

    KSCCGenerator(uint k) : k(k) {}

    Automaton operator()(State n, Alphabet a) const override {
        return k_scc_automaton_generate(n, a, k);
    }
};

vector<unique_ptr<AutomatonGenerator>> init_generators(uint k_scc) {
    vector<unique_ptr<AutomatonGenerator>> G;
    G.emplace_back(make_unique<SimpleGenerator>());
    G.emplace_back(make_unique<KSCCGenerator>(k_scc));
    return G;
}


GenerateAutomatonOutput generateAutomaton(GenerateAutomatonInput input) {
    auto [type, num_states, alphabet_size, missing_edges, num_samples, sample_length, length_variance, k_scc] = input;

    auto Generators = init_generators(k_scc);

    if (num_states * alphabet_size < missing_edges) {
        throw invalid_argument("Invalid number of missing edges");
    }

    if (type >= Generators.size()) {
        throw out_of_range("Unknown automaton generator type");
    }

    Automaton automaton = (*Generators[type])(num_states, alphabet_size);

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
