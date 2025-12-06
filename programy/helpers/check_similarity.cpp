#pragma once
#include <bits/stdc++.h>
using namespace std;

bool check_similarity(
    const vector<AlgorithmRunResult> &testing_results,
    const vector<string> &algorithm_names,
    const string &reference_algorithm,      
    const vector<string> &algorithms_to_compare, 
    double tolerance = 0.05
) {
    if (testing_results.empty()) return true;

    unordered_map<string, int> index_of;

    for (int i = 0; i < (int)algorithm_names.size(); i++) {
        index_of[algorithm_names[i]] = i;
    }

    if (!index_of.count(reference_algorithm)) {
        cerr << "[check_similarity] ERROR: reference algorithm '" 
             << reference_algorithm << "' not found.\n";
        return false;
    }

    int ref_idx = index_of[reference_algorithm];
    double ref = testing_results[ref_idx].runtime_ms;

    for (auto &name : algorithms_to_compare) {
        if (!index_of.count(name)) {
            cerr << "[check_similarity] WARNING: algorithm '" 
                 << name << "' not found, skipping.\n";
            continue;
        }

        int idx = index_of[name];
        double diff = fabs(testing_results[idx].runtime_ms - ref);

        if (diff / ref > tolerance) {
            return false;
        }
        else {
            cout << " ✓ Similar:   " << name
                 << " (diff=" << diff << ", ref=" << ref << ")\n";
        }
    }

    return true;
}
