#pragma once

#include <bits/stdc++.h>
#include "general.cpp"
using namespace std;


/*
States are numbers from 0 to n-1
Alphabets are numbers from 0 to m-1
*/

class Automaton {
public:
    State num_states;
    Alphabet num_alphabet;
    TransitionFunction transition_function;
    State start_state;
    vector<bool> accepting;

    Automaton(State num_states, Alphabet num_alphabet) : 
        num_states(num_states), 
        num_alphabet(num_alphabet), 
        transition_function(num_states, num_alphabet), 
        start_state(0),
        accepting(num_states, false)
    {}

    friend ostream& operator<<(ostream &out_stream, const Automaton &automaton) {
        out_stream << "Automaton:\n";
        out_stream << "Num states: " << automaton.num_states << ", ";
        out_stream << "Num alphabet: " << (int)automaton.num_alphabet << ", ";
        out_stream << "Start state: " << automaton.start_state << "\n";
        out_stream << "States: ";

        for (State state = 0; state < automaton.num_states; state++) {
            out_stream << state << (automaton.accepting[state] ? " (accepting)" : " (non-accepting)") << ", ";
        }

        out_stream << '\n' << automaton.transition_function << '\n';

        return out_stream;
    }

    Automaton& operator=(const Automaton& other) {
        if (this == &other) return *this;
        num_states = other.num_states;
        num_alphabet = other.num_alphabet;
        start_state = other.start_state;
        transition_function = other.transition_function;
        accepting = other.accepting;
        return *this;
    }
};
