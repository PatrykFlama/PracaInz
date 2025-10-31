#include <bits/stdc++.h>
#include "structures/structures.cpp"
#include "generators/wrapper.cpp"
#include "algorithms/algorithms.cpp"
#include "helpers/timer.cpp"
#include "algorithms/helpers/validate_automaton.cpp"
#include "helpers/save_input.cpp"
#include "./tester.cpp"
using namespace std;

/*
In this program we will empirically measure efficiency of chosen algorithm, based on chosen input generation method.  
*/

const int num_states = 20;
const int alphabet_size = 6;
const int num_samples = 10;
const int missing_edges = 5;
const int sample_length = 10;
const float length_variance = 0.2f;


void init() {
    srand((uint)time(0));
}


int main() {
    init();

    const auto &testing_results = testAlgorithms(
        {BruteForceAlgorithm::run_iter},
        {
            AUTOMATON_SIMPLE,
            num_states,
            alphabet_size,
            missing_edges,
            num_samples,
            sample_length,
            length_variance
        }
    );

    for (const auto &result : testing_results) {
        if (result.error.has_error()) {
            cout << "Error: " << result.error.message << '\n';
        } else {
            cout << "Algorithm ran in " << result.runtime_ms << " ms\n";
        }
    }
}
