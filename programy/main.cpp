#include <bits/stdc++.h>
#include "structures/structures.cpp"
#include "generators/generators.cpp"
#include "algorithms/algorithms.cpp"
#include "helpers/timer.cpp"
#include "algorithms/helpers/validate_automaton.cpp"
using namespace std;

/*
In program we will empirically measure efficiency of chosen algorithm, based on chosen input generation method.  
*/

/*
TODO:

repeat XXX times
    1. generate inputs (separate lib)
    2. run algorithm (separate lib) on generated inputs and measure time
    3. validate the output results

save results as time paired with generator parameters (for future analysis, such as plotting)
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

    auto [automaton_data, samples] = simple_generate(
        num_states,
        alphabet_size,
        missing_edges,
        num_samples,
        sample_length,
        length_variance
    );

    auto [automaton, automaton_fixable] = automaton_data;
    auto [positive_samples, negative_samples] = samples;


    Timer timer;
    timer.reset();

    auto [fixable, fixed_automaton] = BruteForceAlgorithm::run(automaton, positive_samples, negative_samples);

    cout << "Elapsed time: " << timer.elapsed() << " ms\n";

    if (automaton_fixable != fixable || 
        (automaton_fixable && !validate_automaton(fixed_automaton, positive_samples, negative_samples))) {
        cout << "Error in fixing automaton\n";
    } 
    else {
        cout << "Automaton fixed successfully: " << (fixable ? "Yes" : "No") << "\n";
    }
}
