#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../generators/types.cpp"

class FileLogger {
    string filename_;
public:

    FileLogger(
        const string &filename, 
        const vector<string> &algorithm_names
    ) {
        filename_ = filename;
        ofstream logfile_(filename);
        logfile_ << "Num States, Alphabet Size, Missing Edges, Num Samples, Sample Length, Length Variance, Automaton Type, ";
        
        for (size_t i = 0; i < algorithm_names.size(); ++i) {
            logfile_ << algorithm_names[i];
            if (i + 1 < algorithm_names.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << ", ";
        for (size_t i = 0; i < algorithm_names.size(); ++i) {
            logfile_ << "ERROR_" + algorithm_names[i];
            if (i + 1 < algorithm_names.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << '\n';
        logfile_.close();
    }

    FileLogger(
        const string &filename, 
        const vector<string> &algorithm_names,
        int missing_edges
    ) {
        filename_ = filename;
        ofstream logfile_(filename);
        logfile_ << "Num States, Alphabet Size, Missing Edges, Num Samples, Sample Length, Length Variance, Automaton Type, ";
        
        for (size_t i = 0; i < algorithm_names.size(); ++i) {
            logfile_ << algorithm_names[i];
            if (i + 1 < algorithm_names.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << ", ";
        for (size_t i = 0; i < algorithm_names.size(); ++i) {
            logfile_ << "ERROR_" + algorithm_names[i];
            if (i + 1 < algorithm_names.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << ", ";
        for (int i = 0; i < missing_edges; ++i) {
            logfile_ <<"from, symbol, S, M, T";
            if (i + 1 < missing_edges) {
                logfile_ << ", ";
            }
        }

        logfile_ << '\n';
        logfile_.close();
    }

    void log(
        const vector<AlgorithmRunResult> &results,
        const GenerateAutomatonInput &generate_input
    ) {
        ofstream logfile_(filename_, ios::app);

        if (!logfile_.is_open()) {
            throw runtime_error("Could not open log file: " + filename_);
        }

        logfile_ << generate_input.num_states << ", "
                 << (int)generate_input.alphabet_size << ", "
                 << generate_input.missing_edges << ", "
                 << generate_input.num_samples << ", "
                 << generate_input.sample_length << ", "
                 << generate_input.length_variance << ", "
                 << generate_input.type << ", ";

        for (size_t i = 0; i < results.size(); ++i) {
            logfile_ << results[i].runtime_ms;
            if (i + 1 < results.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << ", ";
        for (size_t i = 0; i < results.size(); ++i) {
            logfile_ << results[i].error.message;
            if (i + 1 < results.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << '\n';
        logfile_.close();
    }

    void log(
        const vector<AlgorithmRunResult> &results,
        const GenerateAutomatonInput &generate_input,
        vector<MissingEdgeStat> &missing_stats
    ) {
        ofstream logfile_(filename_, ios::app);

        if (!logfile_.is_open()) {
            throw runtime_error("Could not open log file: " + filename_);
        }

        logfile_ << generate_input.num_states << ", "
                 << (int)generate_input.alphabet_size << ", "
                 << generate_input.missing_edges << ", "
                 << generate_input.num_samples << ", "
                 << generate_input.sample_length << ", "
                 << generate_input.length_variance << ", "
                 << generate_input.type << ", ";

        for (size_t i = 0; i < results.size(); ++i) {
            logfile_ << results[i].runtime_ms;
            if (i + 1 < results.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << ", ";
        for (size_t i = 0; i < results.size(); ++i) {
            logfile_ << results[i].error.message;
            if (i + 1 < results.size()) {
                logfile_ << ", ";
            }
        }
        logfile_ << ", ";
        for (size_t i = 0; i < missing_stats.size(); ++i) {
            logfile_ << missing_stats[i].from;
            logfile_ << ", ";
            logfile_ << missing_stats[i].symbol;
            logfile_ << ", ";
            logfile_ << missing_stats[i].S;
            logfile_ << ", ";
            logfile_ << missing_stats[i].M;
            logfile_ << ", ";
            logfile_ << missing_stats[i].T;
            if (i + 1 < missing_stats.size()) {
                logfile_ << ", ";
            }
        }

        logfile_ << '\n';
        logfile_.close();
    }
};
