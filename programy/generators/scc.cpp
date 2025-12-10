#pragma once

#include <bits/stdc++.h>
#include "../structures/structures.cpp"
#include "./helpers/remove_edges_from_samples.cpp"
using namespace std;

Automaton k_scc_automaton_generate(
    State num_states,
    Alphabet alphabet_size,
    uint k_scc
) {
    if (k_scc == 0 || k_scc > num_states) {
        throw invalid_argument("Invalid number of SCCs requested");
    }

    mt19937 rng(std::random_device{}());

    Automaton automaton(num_states, alphabet_size);
    automaton.start_state = 0;

    vector<vector<State>> components(k_scc);

    vector<State> states;
    states.reserve(num_states);

    states.push_back(0);
    for (State s = 1; s < num_states; s++) {
        states.push_back(s);
    }

    shuffle(states.begin() + 1, states.end(), rng);
    vector<uint> cuts;
    cuts.reserve(k_scc + 1);

    cuts.push_back(0);
    cuts.push_back(num_states);

    for (uint i = 0; i < k_scc - 1; i++) {
        cuts.push_back(1 + (rng() % (num_states - 1)));
    }

    sort(cuts.begin(), cuts.end());

    vector<uint> comp_sizes(k_scc);

    for (uint c = 0; c < k_scc; c++) {
        comp_sizes[c] = cuts[c+1] - cuts[c];
    }

    uint idx = 0;
    for (uint c = 0; c < k_scc; c++) {
        for (uint j = 0; j < comp_sizes[c]; j++) {
            components[c].push_back(states[idx++]);
        }
    }

    for (auto &comp : components) {
        uint sz = comp.size();

        if (sz == 1) {
            State s = comp[0];
            for (Alphabet a = 0; a < alphabet_size; a++)
                automaton.transition_function.set_transition(s, a, s);
        } else {
            for (uint i = 0; i < sz; i++) {
                State u = comp[i];
                State v = comp[(i + 1) % sz];
                for (Alphabet a = 0; a < alphabet_size; a++) {
                    if (a == 0)
                        automaton.transition_function.set_transition(u, a, v);
                    else
                        automaton.transition_function.set_transition(u, a, comp[rng() % sz]);
                }
            }
        }
    }

    for (uint c = 0; c + 1 < k_scc; c++) {

        uint max_edges = components[c].size();     
        uint num_edges = 1 + (rng() % max_edges);   

        for (uint e = 0; e < num_edges; e++) {
            State from_state = components[c][rng() % components[c].size()];
            State to_state   = components[c + 1][rng() % components[c + 1].size()];
            Alphabet symbol = rand() % alphabet_size;
            automaton.transition_function.set_transition(from_state, symbol, to_state);
        }
    }

    for (State s = 0; s < num_states; s++)
        automaton.accepting[s] = (rng() % 2 == 0);

    return automaton;
}
