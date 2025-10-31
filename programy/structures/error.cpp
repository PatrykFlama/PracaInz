#pragma once

#include <bits/stdc++.h>
using namespace std;


struct Error {
    string message;

    Error() : message("") {}
    Error(const string &message) : message(message) {}

    void setError(const string &msg) {
        message = msg;
    }

    bool has_error() const {
        return !message.empty();
    }
};

