#pragma once

#include <bits/stdc++.h>         
using namespace std;

class JumpEntry {
public:
    State state_reached;
    size_t pos_reached_in_sample;

    JumpEntry() : state_reached(0), pos_reached_in_sample(0) {}
    JumpEntry(State s, size_t p) : state_reached(s), pos_reached_in_sample(p) {}
};

// jump_table_sample[pos][state] = JumpEntry
using JumpTableForSample = vector<vector<JumpEntry>>;

// jump_table[sample_index] = JumpTableForSample
using JumpTable = vector<JumpTableForSample>;

class JumpTables {
public:
    JumpTable negative;
    JumpTable positive;

    JumpTables() = default;

    void clear() {
        negative.clear();
        positive.clear();
    }

    bool empty() const {
        return negative.empty() && positive.empty();
    }
};
