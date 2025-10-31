#pragma once

#include <bits/stdc++.h>
using namespace std;

class FileLogger {
    string filename_;
public:

    FileLogger(const string &filename, const vector<string> &headers) {
        filename_ = filename;
        ofstream logfile_(filename);
        for (size_t i = 0; i < headers.size(); ++i) {
            logfile_ << headers[i];
            if (i + 1 < headers.size()) {
                logfile_ << ", ";
            }
        }
        logfile_ << '\n';
        logfile_.close();
    }

    template <typename T>
    void log(const vector<T> &values) {
        ofstream logfile_(filename_, ios::app);

        if (!logfile_.is_open()) {
            throw runtime_error("Could not open log file: " + filename_);
        }

        for (size_t i = 0; i < values.size(); ++i) {
            logfile_ << values[i];
            if (i + 1 < values.size()) {
                logfile_ << ", ";
            }
        }
        logfile_ << '\n';
        logfile_.close();
    }
};
