#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
#include "./helpers/remove_edges_from_samples.cpp"
using namespace std;

Automaton disjoint_cycles_automaton_generate(
    State num_states,
    Alphabet alphabet_size
) {
    // generate automaton with disjoint cycles
    Automaton automaton(
        num_states,
        alphabet_size
    );

    automaton.start_state = 0;

    vector<vector<State>> cycles;
    vector<State> unused(num_states);
    iota(unused.begin(), unused.end(), 0);

    // divide nodes to cycles
    while (!unused.empty()) {
        int cycle_size = 2 + rand() % min(4, (int)unused.size());
        vector<State> cycle;
        for (int i = 0; i < cycle_size && !unused.empty(); i++) {
            cycle.push_back(unused.back());
            unused.pop_back();
        }
        cycles.push_back(cycle);
    }

    // create cycles in automaton basend on previous division

    for (auto &cycle : cycles) {
        for (int i = 0; i < (int)cycle.size(); i++) {
            State from_state = cycle[i];
            State to_state = cycle[(i + 1) % cycle.size()];
            for (Alphabet symbol = 0; symbol < alphabet_size; symbol++) {
                automaton.transition_function.set_transition(from_state, symbol, to_state);
            }
            automaton.accepting[from_state] = (rand() % 2 == 0);
        }
    }

    return automaton;
}