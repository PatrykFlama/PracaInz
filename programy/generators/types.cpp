#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
using namespace std;



enum AutomatonType {
    AUTOMATON_SIMPLE = 0,
    AUTOMATON_SCC,
};


struct GenerateAutomatonInput {
    AutomatonType type = AUTOMATON_SIMPLE;
    State num_states = 0;
    Alphabet alphabet_size = 0;
    uint missing_edges = 0;
    uint num_samples = 0;
    uint sample_length = 0;
    float length_variance = 0.0f;
    uint k_scc = 0;
};

struct GenerateAutomatonOutput {
    pair<Automaton, bool> automaton_data;
    pair<Samples, Samples> samples;
};

