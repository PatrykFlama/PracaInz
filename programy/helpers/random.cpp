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

    float prob_higher = (float)(first - range1.first) / (range1.second - range1.first);
    float prob_lower = 1.0f - prob_higher;

    float rand_val = randomFloat(0.0f, 1.0f);
    if (rand_val < prob_higher) {
        // make second higher
        second = randomInt(max(second, first), range2.second);
    } else {
        // make second lower
        second = randomInt(range2.first, min(second, first));
    }

    return {first, second};
}

// inversed correlation
pair<int, int> randomIntInversedCorrelated(pair<int, int> range1, pair<int, int> range2) {
    int first = randomInt(range1.first, range1.second);
    int second = randomInt(range2.first, range2.second);

    float prob_higher = (float)(first - range1.first) / (range1.second - range1.first);
    float prob_lower = 1.0f - prob_higher;

    float rand_val = randomFloat(0.0f, 1.0f);
    if (rand_val < prob_higher) {
        // make second lower
        second = randomInt(range2.first, min(second, first));
    } else {
        // make second higher
        second = randomInt(max(second, first), range2.second);
    }

    return {first, second};
}
