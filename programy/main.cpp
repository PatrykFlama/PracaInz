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
#include "helpers/draw_automaton.cpp"
#include "helpers/fs_utils.cpp"
#include "helpers/check_similarity.cpp"
#include "helpers/check_timeout.cpp"
#include "helpers/edge_calc.cpp"
using namespace std;

/*
In this program we will empirically measure efficiency of chosen algorithm, based on chosen input generation method.  
*/

//TODO: figure out how what the tolerance should be in the check_similarity and check_timeout
void init() {
    srand(static_cast<unsigned>(time(0)));
}

void initFilesystem() {
    ensureDirectory("automata");
    ensureDirectory("automata/dot_files");
    ensureDirectory("automata/png_files");

    clearDirectory("automata/dot_files");
    clearDirectory("automata/png_files");
}

int main() {
    init();
    initFilesystem();

    const int TEST_RUNS = 10;

    GenerateAutomatonInput generate_input;
    generate_input.num_states = 20;
    generate_input.alphabet_size = 4;
    generate_input.num_samples = 10;
    generate_input.missing_edges = 5;
    generate_input.sample_length = 50;
    generate_input.length_variance = 0.2f;
    generate_input.type = AUTOMATON_SIMPLE;
    

    const vector<pair<string, function<AlgorithmOutput(AlgorithmInput)>>> algorithms = {
        {"Brute Force Iterative", {BruteForceAlgorithm::run_iter<>}},
        {"Brute Force Recursive", {BruteForceAlgorithm::run_rec<>}},
        {"Brute Force With Jumps Iterative", PreprocessJumpsAlgorithm::run_iter},
        {"Brute Force With Jumps Recursive", PreprocessJumpsAlgorithm::run_rec},
        {"Brute Force With Edge Heuristic", {EdgeOrderingAlgorithm::run_backtracking_dynamic<>}},
        {"Brute Force With Sample Heuristic Iterative", OrderingSamplesAlgorithm::run_iter},
        {"Brute Force With Sample Heuristic Recursive", OrderingSamplesAlgorithm::run_rec},
        {"Brute Force With Locally Repair", HeuristicIterativeRepairAlgorithm::run<>},
        {"Save Prefix State Iterative", SavePrefixState::run},
    };


    vector<string> algorithm_names;
    vector<function<AlgorithmOutput(AlgorithmInput)>> algorithms_to_test;
    for (const auto &[name, func] : algorithms) {
        algorithm_names.push_back(name);
        algorithms_to_test.push_back(func);
    }

    vector<vector<int64_t>> testing_times(TEST_RUNS, vector<int64_t>(algorithms_to_test.size(), 0));
    vector<int64_t> testing_times_sum(algorithms_to_test.size(), 0);
    vector<int> testing_errors_count(algorithms_to_test.size(), 0);

    FileLogger logger(
        "testing_times.csv",
        algorithm_names
    );

    for (int i : tq::trange(TEST_RUNS)) {
        const auto &testing_results = testAlgorithms(
            algorithms_to_test,
            generate_input
        );

        bool similar = check_similarity(testing_results);
        bool long_runtime_detected = check_timeout(testing_results);

        if (similar || long_runtime_detected) {

            const Automaton &A = testing_results[0].input.broken_automaton;
            const Automaton &B = testing_results[0].output.fixed_automaton;

            if(similar){
                cout<<"similar executing time automata \n";
            }
            else{
                cout<<"timeout \n";
            }
            Samples p = testing_results[0].input.positive_samples;
            Samples n = testing_results[0].input.negative_samples;
            EdgeStats stats = computeEdgeStats(B, p, n);

            string dot = "automata/dot_files/automaton_run_" + to_string(i) + ".dot";
            string png = "automata/png_files/automaton_run_" + to_string(i) + ".png";

            saveAutomatonAsDot(A, B, stats, dot);
            renderDotToPng(dot, png);
        }

        for (size_t j = 0; j < testing_results.size(); ++j) {
            testing_times_sum[j] += testing_results[j].runtime_ms;
            testing_times[i][j] = testing_results[j].runtime_ms;

            if (testing_results[j].error.has_error()) {
                testing_errors_count[j]++;
            }
        }

        logger.log(
            testing_results,
            generate_input
        );
    }

    cout << '\n';
    for (size_t j = 0; j < testing_times_sum.size(); ++j) {
        cout << "Algorithm " << algorithm_names[j] << ": " << (testing_times_sum[j] / TEST_RUNS) << " ms (avg over " << TEST_RUNS << " runs). ";

        if (testing_errors_count[j] > 0) {
            cout << "Errors: " << testing_errors_count[j] << "/" << TEST_RUNS;
        }
        
        cout << '\n';
    }
}
