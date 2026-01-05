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
    const GenerateAutomatonInput generate_input,
    int64_t timeout_ms = -1
) {
    auto [automaton_data, samples] = generateAutomaton(generate_input);

    auto [automaton, automaton_fixable] = automaton_data;
    auto [positive_samples, negative_samples] = samples;

    vector<AlgorithmRunResult> results;
    results.reserve(algorithms.size());

    for (const auto &algorithm : algorithms) {
        Timer timer;
        timer.reset();

        std::atomic<bool> stop_flag{false};
        AlgorithmInput input = {automaton, positive_samples, negative_samples};
        input.stop_flag = &stop_flag;

        auto future_output = async(launch::async, algorithm, input);

        const auto wait_limit = timeout_ms >= 0 ? chrono::milliseconds(timeout_ms) : chrono::milliseconds::max();
        const auto status = future_output.wait_for(wait_limit);

        if (status == future_status::timeout) {
            stop_flag.store(true, std::memory_order_relaxed);
            future_output.wait();

            AlgorithmRunResult timeout_result({false, input.broken_automaton}, timer.elapsed(), input);
            timeout_result.error.setError("Timeout after " + to_string(timeout_ms) + " ms");
            results.push_back(timeout_result);
            continue;
        }

        const auto output = future_output.get();
        const auto elapsed_time = timer.elapsed();

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



