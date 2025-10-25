#pragma once

#include <bits/stdc++.h>
#include "../../structures/structures.cpp"
#include "../../helpers/pair_hash.cpp"
using namespace std;

void remove_edges_from_samples(
    Automaton &automaton,
    const Samples &samples,
    int num_edges
) {
    if (num_edges <= 0 || automaton.num_states * automaton.num_alphabet < num_edges) {
        throw invalid_argument("Invalid number of edges to remove");
    }

    unordered_set<pair<State, Alphabet>, pair_hash> edges;
    for (auto &sample : samples.samples) {
        State current_state = automaton.start_state;
        for (auto symbol : sample) {
            edges.insert({current_state, symbol});
            current_state = automaton.transition_function.get_transition(current_state, symbol);
        }
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
