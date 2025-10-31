#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
using namespace std;



enum AutomatonType {
    AUTOMATON_SIMPLE = 0,
    AUTOMATON_ACYCLIC,
    AUTOMATON_DISJOINT,
    // AUTOMATON_OVERLAPPING
};


struct GenerateAutomatonInput {
    AutomatonType type;
    State num_states;
    Alphabet alphabet_size;
    uint missing_edges;
    uint num_samples;
    uint sample_length;
    float length_variance;
};

struct GenerateAutomatonOutput {
    pair<Automaton, bool> automaton_data;
    pair<Samples, Samples> samples;
};

