#pragma once

#include <bits/stdc++.h>
#include "structures/structures.cpp"
#include "algorithms/types.cpp"
#include "helpers/timer.cpp"
#include "algorithms/helpers/validate_automaton.cpp"
#include "generators/types.cpp"
#include "generators/wrapper.cpp"
using namespace std;


struct AlgorithmRunResult {
    AlgorithmOutput output;
    int64_t runtime_ms;
    Error error;
    AlgorithmInput input;

    AlgorithmRunResult(const AlgorithmOutput &output, int64_t runtime_ms, const AlgorithmInput &algorithm_input)
        : output(output), runtime_ms(runtime_ms), input(algorithm_input) {}
    AlgorithmRunResult(const AlgorithmOutput &output, int64_t runtime_ms, const Error &error, const AlgorithmInput &algorithm_input)
        : output(output), runtime_ms(runtime_ms), error(error), input(algorithm_input) {}
};


vector<AlgorithmRunResult> testAlgorithms(
    const vector<function<AlgorithmOutput(AlgorithmInput)>> algorithms,
    const GenerateAutomatonInput generate_input
) {
    auto [automaton_data, samples] = generateAutomaton(generate_input);

    auto [automaton, automaton_fixable] = automaton_data;
    auto [positive_samples, negative_samples] = samples;

    vector<AlgorithmRunResult> results;
    results.reserve(algorithms.size());

    for (const auto &algorithm : algorithms) {
        Timer timer;
        timer.reset();

        const AlgorithmInput input = {automaton, positive_samples, negative_samples};
        const auto output = algorithm(input);
        const auto elapsed_time = timer.elapsed();

        // results.push_back({
        //     output,
        //     elapsed_time
        // });
        results.emplace_back(
            output,
            elapsed_time,
            input
        );

        if (output.fixable != automaton_fixable) {
            results.back().error.setError("Fixability mismatch");
        } else if (automaton_fixable && !validate_automaton(output.fixed_automaton, positive_samples, negative_samples)) {
            results.back().error.setError("Invalid fixed automaton");
        }
    }

    return results;
}



