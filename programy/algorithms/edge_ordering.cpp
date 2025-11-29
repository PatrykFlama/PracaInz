#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../structures/structures.cpp"
#include "./helpers/validate_automaton.cpp"
#include "./types.cpp"
#include "./helpers/misc.cpp" 


vector<pair<State, Alphabet>> order_missing_edges_by_frequency(
    const Automaton &automaton,
    const Samples &positive,
    const Samples &negative,
    const vector<pair<State, Alphabet>> &missing_edges
) {
    unordered_map<long long, int> usage;  

    auto encode = [&](State s, Alphabet a) {
        return (static_cast<long long>(s) << 32) |
            static_cast<long long>(a);
    };

    auto process_samples = [&](const Samples &S) {
        for (const auto &sample : S.samples) {
            State current = automaton.start_state;

            for (Alphabet sym : sample) {
                long long key = encode(current, sym);
                if (usage.count(key)) usage[key]++;

                State next = automaton.transition_function.get_transition(current, sym);
                if (next == automaton.transition_function.invalid_edge) break;
                current = next;
            }
        }
    };

    for (auto &edge : missing_edges) {
        usage[encode(edge.first, edge.second)] = 0;
    }

    process_samples(positive);
    process_samples(negative);

    vector<pair<State, Alphabet>> ordered = missing_edges;

    sort(ordered.begin(), ordered.end(),
        [&](auto &e1, auto &e2) {
            long long k1 = encode(e1.first, e1.second);
            long long k2 = encode(e2.first, e2.second);
            return usage[k1] < usage[k2];   
        });

    return ordered;
}
