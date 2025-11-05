#include <bits/stdc++.h>
#include "structures/structures.cpp"
#include "generators/wrapper.cpp"
#include "algorithms/algorithms.cpp"
#include "helpers/timer.cpp"
#include "algorithms/helpers/validate_automaton.cpp"
#include "helpers/save_input.cpp"
#include "./tester.cpp"
#include "helpers/tqdm.hpp"
#include "helpers/file_logger.cpp"
using namespace std;

/*
In this program we will empirically measure efficiency of chosen algorithm, based on chosen input generation method.  
*/


void init() {
    srand(static_cast<unsigned>(time(0)));
}


int main() {
    init();

    const int num_states = 20;
    const int alphabet_size = 6;
    const int num_samples = 10;
    const int missing_edges = 5;
    const int sample_length = 10;
    const float length_variance = 0.2f;
    
    const vector<string> algorithm_names = {
        "Brute Force Iterative",
        "Brute Force Recursive",
        "Brute Force With Jumps Iterative",
        "Brute Force With Jumps Recursive"
    };
    const vector<function<AlgorithmOutput(AlgorithmInput)>> algorithms_to_test = {
        BruteForceAlgorithm::run_iter<>,
        BruteForceAlgorithm::run_rec<>,
        PreprocessJumpsAlgorithm::run_iter,
        PreprocessJumpsAlgorithm::run_rec
    };
    
    
    const AutomatonType automaton_type = AUTOMATON_SIMPLE;

    const int TEST_RUNS = 10;

    vector<vector<int64_t>> testing_times(TEST_RUNS, vector<int64_t>(algorithms_to_test.size(), 0));
    vector<int64_t> testing_times_sum(algorithms_to_test.size(), 0);

    FileLogger logger(
        "testing_times.csv",
        algorithm_names
    );

    for (int i : tq::trange(TEST_RUNS)) {
        const auto &testing_results = testAlgorithms(
            algorithms_to_test,
            {
                automaton_type,
                num_states,
                alphabet_size,
                missing_edges,
                num_samples,
                sample_length,
                length_variance
            }
        );

        for (size_t j = 0; j < testing_results.size(); ++j) {
            testing_times_sum[j] += testing_results[j].runtime_ms;
            testing_times[i][j] = testing_results[j].runtime_ms;
        }

        logger.log(testing_times[i]);
    }

    cout << '\n';
    for (size_t j = 0; j < testing_times_sum.size(); ++j) {
        cout << "Algorithm " << algorithm_names[j] << ": " << (testing_times_sum[j] / TEST_RUNS) << " ms (avg over " << TEST_RUNS << " runs)\n";
    }
}
