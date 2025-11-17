#pragma once
#include <bits/stdc++.h>
#include "./stats.cpp"
using namespace std;

bool check_timeout(
    int run_index,
    const vector<AlgorithmRunResult> &testing_results,
    const vector<vector<int64_t>> &testing_times,
    size_t num_algorithms
) {
    //TODO: fix it, do faster
    for (size_t alg = 0; alg < num_algorithms; alg++) {
        vector<int64_t> prev_times;
        prev_times.reserve(run_index);

        for (int k = 0; k < run_index; k++) {
            prev_times.push_back(testing_times[k][alg]);
        }

        if (prev_times.size() < 5) {
            continue;
        }

        double m = mean(prev_times);
        double sd = stdev(prev_times);
        if (sd == 0) {
            sd = 1; 
        }

        double this_time = testing_results[alg].runtime_ms;

        if (this_time > m + 3 * sd) {
            return true;
        }
    }

    return false;
}