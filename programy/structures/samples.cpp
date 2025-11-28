#pragma once

#include <bits/stdc++.h>
#include "general.cpp"
using namespace std;

class Samples {
public:
    vector<vector<Alphabet>> samples;  //? samples[sample_index][position_in_sample] = Alphabet letter

    auto begin() { return samples.begin(); }
    auto end() { return samples.end(); }
    auto begin() const { return samples.begin(); }
    auto end() const { return samples.end(); }
    
    Samples() {}
    Samples(int num_samples) : samples(num_samples) {}

    friend ostream& operator<<(ostream &out_stream, const Samples &samples) {
        for (int i = 0; i < (int)samples.samples.size(); i++) {
            for (const auto &symbol : samples.samples[i]) {
                out_stream << (int)symbol << " ";
            }
            out_stream << '\n';
        }
        return out_stream;
    }
};
