#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
using namespace std;


struct AlgorithmOutput {
    bool fixable;
    Automaton fixed_automaton;
};

struct AlgorithmInput {
    Automaton broken_automaton;
    Samples positive_samples;
    Samples negative_samples;
};

