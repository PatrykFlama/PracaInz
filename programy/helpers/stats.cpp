#pragma once
#include <bits/stdc++.h>
using namespace std;

double mean(const vector<int64_t> &v) {
    if (v.empty()) return 0;
    double s = 0;
    for (auto x : v) s += x;
    return s / v.size();
}

double stdev(const vector<int64_t> &v) {
    if (v.size() < 2) return 0;
    double m = mean(v);
    double s = 0;
    for (auto x : v) s += (x - m) * (x - m);
    return sqrt(s / (v.size() - 1));
}
