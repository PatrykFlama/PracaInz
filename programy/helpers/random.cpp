#pragma once

#include <bits/stdc++.h>
using namespace std;


int randomInt(int min_inclusive, int max_inclusive) {
    if (min_inclusive == max_inclusive) {
        return min_inclusive;
    }
    
    return min_inclusive + (rand() % (max_inclusive - min_inclusive + 1));
}

float randomFloat(float min_inclusive, float max_inclusive) {
    float scale = rand() / (float) RAND_MAX; // [0, 1.0]
    return min_inclusive + scale * (max_inclusive - min_inclusive); // [min, max]
}

// returns random pair of integers, where if first is higher than second is more likely to be higher as well, and vice versa
pair<int, int> randomIntCorrelated(pair<int, int> range1, pair<int, int> range2) {
    int first = randomInt(range1.first, range1.second);
    int second = randomInt(range2.first, range2.second);

    const int range1_span = range1.second - range1.first;
    float prob_higher = range1_span > 0
        ? static_cast<float>(first - range1.first) / range1_span
        : 0.5f; // avoid division by zero when range is a single value
    prob_higher = max(0.0f, min(1.0f, prob_higher));

    float rand_val = randomFloat(0.0f, 1.0f);
    if (rand_val < prob_higher) {
        // make second higher
        const int low = max(second, first);
        second = (low > range2.second) ? range2.second : randomInt(low, range2.second);
    } else {
        // make second lower
        const int high = min(second, first);
        second = (high < range2.first) ? range2.first : randomInt(range2.first, high);
    }

    return {first, second};
}

// inversed correlation
pair<int, int> randomIntInversedCorrelated(pair<int, int> range1, pair<int, int> range2) {
    int first = randomInt(range1.first, range1.second);
    int second = randomInt(range2.first, range2.second);

    const int range1_span = range1.second - range1.first;
    float prob_higher = range1_span > 0
        ? static_cast<float>(first - range1.first) / range1_span
        : 0.5f; // avoid division by zero when range is a single value
    prob_higher = max(0.0f, min(1.0f, prob_higher));

    float rand_val = randomFloat(0.0f, 1.0f);
    if (rand_val < prob_higher) {
        // make second lower
        const int high = min(second, first);
        second = (high < range2.first) ? range2.first : randomInt(range2.first, high);
    } else {
        // make second higher
        const int low = max(second, first);
        second = (low > range2.second) ? range2.second : randomInt(low, range2.second);
    }

    return {first, second};
}

// generate num_states first, then choose missing_edges such that larger num_states are more likely to get smaller missing_edges
// skew parameter controls how strongly missing_edges is reduced for larger num_states
// (skew > 1 = stronger bias towards smaller missing_edges when num_states is large)
pair<int, int> randomNumStatesAndScaledMissing(pair<int, int> num_range, pair<int, int> missing_range, float skew = 2.0f) {
    int num = randomInt(num_range.first, num_range.second);

    int missing_min = missing_range.first;
    int missing_max = missing_range.second;

    int num_span = num_range.second - num_range.first;
    float frac = (num_span > 0) ? static_cast<float>(num - num_range.first) / static_cast<float>(num_span) : 0.5f;
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;

    // val in [0,1], where larger frac (bigger num) produces smaller val -> smaller allowed max
    float val = 1.0f - std::pow(frac, skew);

    int missing_span = missing_max - missing_min;
    int max_allowed = missing_min + static_cast<int>(std::floor(val * static_cast<float>(missing_span) + 1e-6f));
    if (max_allowed < missing_min) max_allowed = missing_min;
    if (max_allowed > missing_max) max_allowed = missing_max;

    int missing = randomInt(missing_min, max_allowed);
    return {num, missing};
}
