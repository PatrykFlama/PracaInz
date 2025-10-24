#pragma once

#include <bits/stdc++.h>
#include "general.cpp"
using namespace std;

class Samples {
public:
    vector<vector<Alphabet>> samples;  //? samples[sample_index][position_in_sample] = Alphabet letter

    Samples() {}
    Samples(int num_samples) : samples(num_samples) {}
};
