#pragma once
#include <fstream>
#include <string>
#include "../structures/automaton.cpp" 
using namespace std; 

void saveAutomatonAsDot(const Automaton &A, const string &filename) {
    ofstream out(filename);
    out << "digraph Automaton {\n";
    out << "    rankdir=LR;\n";
    out << "    node [shape=circle];\n";

    // start arrow
    out << "    start [shape=point];\n";
    out << "    start -> " << A.start_state << ";\n";

    // accepting states
    for (int i = 0; i < A.num_states; i++) {
        if (A.accepting[i]) {
            out << "    " << i << " [shape=doublecircle];\n";
        }
    }

    // transitions
    for (int s = 0; s < A.num_states; s++) {
        for (int c = 0; c < A.num_alphabet; c++) {
            int t = A.transition_function.get_transition(s,c);
            if (t == A.transition_function.invalid_edge) continue;
            out << "    " << s << " -> " << t 
                << " [label=\"" << c << "\"];\n";
        }
    }

    out << "}\n";
    out.close();
}

void renderDotToPng(const string &dotfile, const string &pngfile) {
    string cmd = "dot -Tpng " + dotfile + " -o " + pngfile;
    system(cmd.c_str());
}
