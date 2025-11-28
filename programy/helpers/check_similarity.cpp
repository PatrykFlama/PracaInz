#pragma once 
#include <bits/stdc++.h>
using namespace std;

bool check_similarity(
    const vector<AlgorithmRunResult> &testing_results,
    double tolerance = 0.05
) {
    double ref = testing_results[0].runtime_ms;

    for (size_t j = 2; j < testing_results.size(); j++) {
        double diff = fabs(testing_results[j].runtime_ms - ref);
        if (diff / ref > 0.05) {
            return false;
            break;
        }
    }
    return true;
}