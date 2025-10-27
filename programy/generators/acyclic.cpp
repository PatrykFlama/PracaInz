#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
#include "./helpers/remove_edges_from_samples.cpp"
using namespace std;

Automaton acyclic_automaton_generate(
    State num_states,
    Alphabet alphabet_size
) {
    // generate acyclic automaton
    Automaton automaton(
        num_states,
        alphabet_size
    );

    automaton.start_state = 0;

    // edges only to higer node

    for (State from_state = 0; from_state < num_states; from_state++) {
        for (Alphabet symbol = 0; symbol < alphabet_size; symbol++) {
            auto to_state = from_state + 1 + rand() % (num_states - from_state);
            if (to_state >= num_states) {
                to_state = num_states - 1;
            }
            automaton.transition_function.set_transition(from_state, symbol, to_state);
        }
        automaton.accepting[from_state] = (rand() % 2 == 0);
    }

    return automaton;
}