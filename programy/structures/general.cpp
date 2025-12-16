#pragma once

#include <bits/stdc++.h>
using namespace std;


typedef size_t Alphabet;

typedef size_t State;
const State INVALID_STATE = SIZE_MAX;

class TransitionFunction {
public:
    const State invalid_edge = INVALID_STATE;
    vector<vector<State>> transitions;    //? transitions[from_state][letter] = to_state

    TransitionFunction(State num_states, Alphabet alphabet_size) : 
        transitions(num_states, vector<State>(alphabet_size, invalid_edge)) {}

    void set_transition(State from_state, Alphabet symbol, State to_state) {
        transitions[from_state][symbol] = to_state;
    }

    State get_transition(State from_state, Alphabet symbol) const {
        return transitions[from_state][symbol];
    }

    friend ostream& operator<<(ostream &out_stream, const TransitionFunction &tf) {
        for (State state = 0; state < (State)tf.transitions.size(); state++) {
            out_stream << state << ": { ";
            for (Alphabet symbol = 0; symbol < (Alphabet)tf.transitions[state].size(); symbol++) {
                State to_state = tf.transitions[state][symbol];
                out_stream << (int)symbol << " -> " << to_state << ", ";
            }
            out_stream << "}\n";
        }
        return out_stream;
    }

    TransitionFunction(const TransitionFunction&) = default;

    TransitionFunction& operator=(const TransitionFunction& other) {
        if (this == &other) return *this;
        transitions = other.transitions;
        return *this;
    }
};
