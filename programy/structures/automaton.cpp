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
};
