#pragma once

#include <bits/stdc++.h>
using namespace std;

class WeightedSample {
public:
    vector<Alphabet> sample;
    bool is_positive;
    int missing_count;
    size_t length;

    bool operator<(const WeightedSample &other) const {
        //negative sample before positive
        if (is_positive != other.is_positive) {
            return !is_positive; 
        }

        //we want more non existing edges
        if (missing_count != other.missing_count) {
            return missing_count > other.missing_count;
        }

        //we want shorter samples
        return length < other.length;
    }
};
