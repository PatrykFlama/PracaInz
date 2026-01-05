#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../../structures/structures.cpp"
#include "../types.cpp"

void get_missing_edges(
    Automaton& automaton,
    vector<pair<State, Alphabet>>& missing_edges
) {
    for (State state = 0; state < automaton.num_states; state++) {
        for (Alphabet symbol = 0; symbol < automaton.num_alphabet; symbol++) {
            if (automaton.transition_function.get_transition(state, symbol) == automaton.transition_function.invalid_edge) {
                missing_edges.push_back({state, symbol});
            }
        }
    }
}

inline bool should_stop(const std::atomic<bool>* stop_flag) {
    return stop_flag && stop_flag->load(std::memory_order_relaxed);
}
