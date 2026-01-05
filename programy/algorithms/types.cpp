#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
using namespace std;


struct AlgorithmOutput {
    bool fixable;
    Automaton fixed_automaton;

    AlgorithmOutput() : fixable(false), fixed_automaton(0, 0) {}
    AlgorithmOutput(bool fixable, const Automaton &fixed_automaton)
        : fixable(fixable), fixed_automaton(fixed_automaton) {}
};

struct AlgorithmInput {
    Automaton broken_automaton;
    Samples positive_samples;
    Samples negative_samples;
    std::atomic<bool>* stop_flag = nullptr;
};

