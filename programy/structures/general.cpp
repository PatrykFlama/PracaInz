#pragma once

#include <bits/stdc++.h>
using namespace std;


typedef char Alphabet;

typedef int State;
const State INVALID_STATE = -1;

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
};
