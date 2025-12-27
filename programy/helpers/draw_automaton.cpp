#pragma once
#include <fstream>
#include <string>
#include "../structures/automaton.cpp" 
using namespace std; 

void saveAutomatonAsDot(const Automaton &A, const Automaton &B, const EdgeStats &stats, const string &filename) {
    ofstream out(filename);
    out << "digraph Automaton {\n";
    out << "    rankdir=LR;\n";
    out << "    node [shape=circle];\n";

    // start arrow
    out << "    start [shape=point];\n";
    out << "    start -> " << A.start_state << ";\n";

    // accepting states
    for (State i = 0; i < A.num_states; i++) {
        if (A.accepting[i]) {
            out << "    " << i << " [shape=doublecircle];\n";
        }
    }

    // transitions
    // transitions[from_state][letter] = to_state
    for (State s = 0; s < A.num_states; s++) {
        for (size_t c = 0; c < A.num_alphabet; c++) {
            State t = A.transition_function.get_transition(s,c);
            if (t == A.transition_function.invalid_edge) continue;
            out << "    " << s << " -> " << t 
                << " [label=\"" << c << "\"];\n";
        }
    }

    for (State s = 0; s < B.num_states; s++) {
        for (size_t c = 0; c < B.num_alphabet; c++) {

            State tB = B.transition_function.get_transition(s, c);
            if (tB == B.transition_function.invalid_edge) continue;

            State tA = A.transition_function.get_transition(s, c);
            if (tA != A.transition_function.invalid_edge) continue;

            int S = stats.sample_count[s][c];
            int T = stats.total_count[s][c];
            int M = stats.max_per_word[s][c];

            out << "    " << s << " -> " << tB
                << " [label=\""
                << c
                << "\\nS=" << S
                << "\\nT=" << T
                << "\\nM=" << M
                << "\", color=\"red\", penwidth=2.2];\n";
        }
    }

    out << "}\n";
    out.close();
}

void renderDotToPng(const string &dotfile, const string &pngfile) {
    string cmd = "dot -Tpng " + dotfile + " -o " + pngfile;
    system(cmd.c_str());
}
