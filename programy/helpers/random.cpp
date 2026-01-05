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
