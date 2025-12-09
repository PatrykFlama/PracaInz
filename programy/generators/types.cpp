#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
using namespace std;



enum AutomatonType {
    AUTOMATON_SIMPLE = 0,
    AUTOMATON_SCC,
};


struct GenerateAutomatonInput {
    AutomatonType type;
    State num_states;
    Alphabet alphabet_size;
    uint missing_edges;
    uint num_samples;
    uint sample_length;
    float length_variance;
    uint k_scc = 0;
};

struct GenerateAutomatonOutput {
    pair<Automaton, bool> automaton_data;
    pair<Samples, Samples> samples;
};

