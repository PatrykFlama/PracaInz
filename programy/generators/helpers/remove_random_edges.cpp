#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../../structures/structures.cpp"
#include "../../helpers/pair_hash.cpp"

void remove_random_edges(Automaton &automaton, int num_edges) {
    if (num_edges <= 0 || automaton.num_states * automaton.num_alphabet < num_edges) {
        throw invalid_argument("Invalid number of edges to remove");
    }

    // TODO: begin with quick random shots, then after X failures use set method

    unordered_set<pair<State, Alphabet>, pair_hash> edges;
    for (State from_state = 0; from_state < automaton.num_states; from_state++) {
        for (Alphabet symbol = 0; symbol < automaton.num_alphabet; symbol++) {
            if (automaton.transition_function.get_transition(from_state, symbol) != automaton.transition_function.invalid_edge) {
                edges.insert({from_state, symbol});
            }
        }
    }

    if (edges.size() == 0 || edges.size() < (int)num_edges) {
        throw underflow_error("Invalid automata - there are too few edges in order to remove them");
    }

    while (num_edges--) {
        int index = rand() % edges.size();
        auto it = edges.begin();
        advance(it, index);
        auto [from_state, symbol] = *it;

        automaton.transition_function.set_transition(from_state, symbol, automaton.transition_function.invalid_edge);
        edges.erase(it);
    }
}
