#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../structures/structures.cpp"


void save_input(
    const string description,
    const Automaton &automaton,
    const Samples &positive_samples,
    const Samples &negative_samples,
    ostream &out_stream
) {
    out_stream << description << '\n';
    out_stream << automaton.num_states << " " << (uint)automaton.num_alphabet << '\n';
    out_stream << automaton << '\n';

    out_stream << "Positive samples:\n" << positive_samples << '\n';
    out_stream << "Negative samples:\n" << negative_samples << '\n';
}
