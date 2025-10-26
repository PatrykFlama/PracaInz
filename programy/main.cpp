#include <bits/stdc++.h>
#include "structures/structures.cpp"
#include "generators/generators.cpp"
#include "algorithms/algorithms.cpp"
#include "helpers/timer.cpp"
#include "algorithms/helpers/validate_automaton.cpp"
#include "helpers/save_input.cpp"
using namespace std;

/*
In program we will empirically measure efficiency of chosen algorithm, based on chosen input generation method.  
*/

/*
TODO:

now we want to have system for:
1. running algorithms against generated input multiple times, saving time for each config (and informing us of any errors in some logs file)
    - generate input, save it in DB and take its index (deterministic hash)
    - run each of given algorithms and save stats for given input index
    - have some reasonable timeouts option, to not stall long testing run
2. create alternate system for measuring algorithm efficiency - calculate number of operations instead of hardware-biased time
3. probably we should switch to using headers OR (to research) C++20 modules (current approach is not easy/clean to compile)
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

    auto [fixable, fixed_automaton] = BruteForceAlgorithm::run_iter(automaton, positive_samples, negative_samples);

    cout << "Elapsed time: " << timer.elapsed() << " ms\n";

    if (automaton_fixable != fixable || 
        (automaton_fixable && !validate_automaton(fixed_automaton, positive_samples, negative_samples))) {
        cout << "Error in fixing automaton\n";

        ofstream ofile("error_input.txt", ios::app);
        save_input(
            "Input that caused error:",
            automaton,
            positive_samples,
            negative_samples,
            ofile
        );
        ofile.close();
    }
    // else {
    //     cout << "Automaton fixed successfully: " << (fixable ? "Yes" : "No") << "\n";
    // }
}
