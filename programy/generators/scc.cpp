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

    Automaton automaton(num_states, alphabet_size);
    automaton.start_state = 0;

    vector<vector<State>> components(k_scc);
    vector<State> states(num_states);
    iota(states.begin(), states.end(), 0);

    shuffle(states.begin(), states.end(), mt19937(random_device{}()));

    for (uint i = 0; i < num_states; i++) {
        components[i % k_scc].push_back(states[i]);
    }

    for (auto &comp : components) {
        uint sz = comp.size();
        if (sz == 1) {
            State s = comp[0];
            for (Alphabet a = 0; a < alphabet_size; a++) {
                automaton.transition_function.set_transition(s, a, s);
            }
        } else {
            for (uint i = 0; i < sz; i++) {
                State u = comp[i];
                State v = comp[(i + 1) % sz];
                for (Alphabet a = 0; a < alphabet_size; a++) {
                    if (a == 0) {
                        automaton.transition_function.set_transition(u, a, v);
                    }
                    else {
                        automaton.transition_function.set_transition(u, a, comp[rand() % sz]);
                    }
                }
            }
        }
    }

    // --- 3. Dodajemy połączenia między SCC, NIE scalając ich ---
    // C0 -> C1 -> C2 -> ... -> C(k-1)
    for (uint c = 0; c + 1 < k_scc; c++) {
        State from_state = components[c][rand() % components[c].size()];
        State to_state   = components[c + 1][rand() % components[c + 1].size()];

        // Nadpisujemy jedno przejście symbolem 0
        // (kompletność zostaje zachowana)
        automaton.transition_function.set_transition(from_state, 0, to_state);
    }

    // --- 4. Losowe stany akceptujące ---
    for (State s = 0; s < num_states; s++)
        automaton.accepting[s] = (rand() % 2 == 0);

    return automaton;
}
