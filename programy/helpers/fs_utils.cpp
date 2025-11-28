#pragma once
#include <bits/stdc++.h>
#include <filesystem>
#include <iostream>
using namespace std;

namespace fs = filesystem;

inline void ensureDirectory(const string &path) {
    fs::path p(path);
    if (!fs::exists(p)) {
        fs::create_directories(p);
    }
}

inline void clearDirectory(const string &path) {
    fs::path p(path);
    if (!fs::exists(p)) return;

    for (auto &entry : fs::directory_iterator(p)) {
        fs::remove(entry);
    }
}
