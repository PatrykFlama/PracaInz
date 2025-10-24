#pragma once

#include <bits/stdc++.h>
#include "../../structures/structures.cpp"
using namespace std;


namespace ValidateAutomaton {
    bool validate_automaton_helper(
        const Automaton &automaton,
        const Samples &samples,
        const bool expected_accepting
    ) {
        for (const auto &sample : samples.samples) {
            auto current_state = automaton.start_state;

            for (const auto &symbol : sample) {
                if (current_state == automaton.transition_function.invalid_edge) {
                    return false;
                }

                current_state = automaton.transition_function.get_transition(current_state, symbol);
            }

            if (automaton.accepting[current_state] != expected_accepting) {
                return false;
            }
        }

        return true;
    }
};


bool validate_automaton(
    const Automaton &automaton,
    const Samples &positive_samples,
    const Samples &negative_samples
) {
    const bool positive_valid = ValidateAutomaton::validate_automaton_helper(
        automaton,
        positive_samples,
        true
    );

    if (!positive_valid) return false;

    const bool negative_valid = ValidateAutomaton::validate_automaton_helper(
        automaton,
        negative_samples,
        false
    );

    return negative_valid;
}
