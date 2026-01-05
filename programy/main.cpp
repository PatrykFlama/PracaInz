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
#include "helpers/edge_calc.cpp"
#include "helpers/random.cpp"
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

    const bool SAVE_AUTOMATA_TO_FILES = false;
    const size_t TEST_RUNS = 10;

    GenerateAutomatonInput generate_input_from, generate_input_to;

    generate_input_from.num_states = 10;
    generate_input_to.num_states = 150;

    generate_input_from.alphabet_size = 4;
    generate_input_to.alphabet_size = 4;

    generate_input_from.num_samples = 10;
    generate_input_to.num_samples = 1000;

    generate_input_from.missing_edges = 2;
    generate_input_to.missing_edges = 10;

    generate_input_from.sample_length = 10;
    generate_input_to.sample_length = 200;

    generate_input_from.length_variance = 0.2f;
    generate_input_from.type = AUTOMATON_SIMPLE;

    generate_input_from.k_scc = 3;
    generate_input_to.k_scc = 3;


    const vector<pair<string, function<AlgorithmOutput(AlgorithmInput)>>> algorithms = {
        {"Brute Force Iterative", {BruteForceAlgorithm::run_iter<>}},
        // {"Brute Force Recursive", {BruteForceAlgorithm::run_rec<>}},
        {"Brute Force With Jumps Iterative", PreprocessJumpsAlgorithm::run_iter},
        // {"Brute Force With Jumps Recursive", PreprocessJumpsAlgorithm::run_rec},
        // {"Brute Force With Edge Heuristic", {EdgeOrderingAlgorithm::run_backtracking_dynamic<>}},
        // {"Brute Force With Sample Heuristic Iterative", OrderingSamplesAlgorithm::run_iter},
        // {"Brute Force With Sample Heuristic Recursive", OrderingSamplesAlgorithm::run_rec},
        {"Heuristic Repair", HeuristicIterativeRepairAlgorithm::run<>},
        // {"Save Prefix State Iterative", SavePrefixState::run},
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

    for (size_t i : tq::trange(TEST_RUNS)) {
        GenerateAutomatonInput generate_input;

        generate_input.num_states = randomInt(generate_input_from.num_states, generate_input_to.num_states);
        generate_input.alphabet_size = randomInt(generate_input_from.alphabet_size, generate_input_to.alphabet_size);
        generate_input.num_samples = randomInt(generate_input_from.num_samples, generate_input_to.num_samples);
        generate_input.missing_edges = randomInt(generate_input_from.missing_edges, generate_input_to.missing_edges);
        generate_input.sample_length = randomInt(generate_input_from.sample_length, generate_input_to.sample_length);
        generate_input.length_variance = generate_input_from.length_variance; // keep constant
        generate_input.type = generate_input_from.type; // keep constant
        generate_input.k_scc = randomInt(generate_input_from.k_scc, generate_input_to.k_scc);

        const auto &testing_results = testAlgorithms(
            algorithms_to_test,
            generate_input
        );

        bool similar = check_similarity(
            testing_results,
            algorithm_names,
            "Brute Force Iterative",
            {"Brute Force Iterative"},
            0.05
        );


        if (similar) {
            const Automaton &A = testing_results[0].input.broken_automaton;
            const Automaton &B = testing_results[0].output.fixed_automaton;
   
            Samples p = testing_results[0].input.positive_samples;
            Samples n = testing_results[0].input.negative_samples;
            EdgeStats stats = computeEdgeStats(B, p, n);

            string dot = "automata/dot_files/automaton_run_" + to_string(i) + ".dot";
            string png = "automata/png_files/automaton_run_" + to_string(i) + ".png";

            if (SAVE_AUTOMATA_TO_FILES) {
                saveAutomatonAsDot(A, B, stats, dot);
                renderDotToPng(dot, png);
            }
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
        long long avg_time = testing_times_sum[j] / static_cast<long long>(TEST_RUNS);
        cout << "Algorithm " << algorithm_names[j] << ": " << avg_time << " ms (avg over " << TEST_RUNS << " runs). ";

        if (testing_errors_count[j] > 0) {
            cout << "Errors: " << testing_errors_count[j] << "/" << TEST_RUNS;
        }
        
        cout << '\n';
    }
}
