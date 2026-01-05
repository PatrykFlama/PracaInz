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
