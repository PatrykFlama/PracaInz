#pragma once
#include <bits/stdc++.h>
#include "./stats.cpp"
using namespace std;

bool check_timeout(
    const vector<AlgorithmRunResult> &testing_results,
    double tolerance = 0.05
) {
    double brute = testing_results[0].runtime_ms;

    //we are comparing to iterative brute force
    for (size_t j = 2; j < testing_results.size(); j++) {
        double t = testing_results[j].runtime_ms;

        double ratio = t / brute;

        if (ratio < (1.0 - tolerance)) {
            return false;
        }
    }

    return true;
}