/*
Save-prefix-state algorithm: iterative backtracking that caches, for each depth and sample,
the furthest reachable state and position without using not-yet-assigned missing edges.
*/

#pragma once

#include <bits/stdc++.h>
using namespace std;

#include "../structures/structures.cpp"
#include "types.cpp"
#include "helpers/validate_automaton.cpp"
#include "helpers/misc.cpp"

namespace SavePrefixState {

static bool validate_automaton_func(
    const Automaton &automaton,
    const Samples &pos_samples,
    const Samples &neg_samples,
    const vector<State> &final_states,
    const vector<size_t> &final_pos
) {
    for (size_t i = 0; i < pos_samples.samples.size(); ++i) {
        if (final_pos[i] != pos_samples.samples[i].size()) return false;    // probably not needed
        const auto s = final_states[i];
        if (s == automaton.transition_function.invalid_edge) return false;  // probably not needed
        if (automaton.accepting[s] != true) return false;
    }

    for (size_t j = 0; j < neg_samples.samples.size(); ++j) {
        const size_t idx = pos_samples.samples.size() + j;
        if (final_pos[idx] != neg_samples.samples[j].size()) return false;  // probably not needed
        const auto s = final_states[idx];
        if (s == automaton.transition_function.invalid_edge) return false;  // probably not needed
        if (automaton.accepting[s] != false) return false;
    }

    return true;
}

static void walk_samples(
    const Automaton &automaton,
    const Samples &samples,
    const vector<State> &from_states,
    const vector<size_t> &from_pos,
    vector<State> &to_states,
    vector<size_t> &to_pos,
    const size_t samples_pos_offset = 0
) {
    for (size_t i = 0; i < samples.samples.size(); i++) {
        const auto merged_idx = i + samples_pos_offset;
        auto curr_state = from_states[merged_idx];
        auto curr_pos = from_pos[merged_idx];
        const auto &sample = samples.samples[i];

        // walk as far as possible through the automaton with sample
        while (curr_pos < sample.size()) {
            // if (curr_state == automaton.transition_function.invalid_edge) break;

            const auto edge = sample[curr_pos];
            const auto next_state = automaton.transition_function.get_transition(curr_state, edge);
            if (next_state == automaton.transition_function.invalid_edge) break;
            curr_state = next_state;
            curr_pos++;
        }

        to_states[merged_idx] = curr_state;
        to_pos[merged_idx] = curr_pos;
    }
}

static void walk_samples_combined(
    const Automaton &automaton,
    const Samples &pos_samples,
    const Samples &neg_samples,
    const vector<State> &from_states,
    const vector<size_t> &from_pos,
    vector<State> &to_states,
    vector<size_t> &to_pos
) {
    walk_samples(automaton, pos_samples, from_states, from_pos, to_states, to_pos, 0);
    walk_samples(automaton, neg_samples, from_states, from_pos, to_states, to_pos, pos_samples.samples.size());
}

AlgorithmOutput run(const AlgorithmInput &input) {
    const auto &[broken_automaton, positive_samples, negative_samples] = input;

    // we'll mutate automaton in-place: assigned missing edges set to states; deeper edges kept invalid
    Automaton automaton = broken_automaton;

    vector<pair<State, Alphabet>> missing_edges;
    get_missing_edges(automaton, missing_edges);
    const int m = (int)missing_edges.size();

    const size_t samples_size = positive_samples.samples.size() + negative_samples.samples.size();

    // current state/pos for samples at current depth
    vector<State> curr_state(samples_size, automaton.start_state);
    vector<size_t> curr_pos(samples_size, 0);

    // initialize curr_state/curr_pos on base automaton (missing edges still invalid)
    vector<State> tmp_state = curr_state;
    vector<size_t> tmp_pos = curr_pos;
    walk_samples_combined(automaton, positive_samples, negative_samples, tmp_state, tmp_pos, curr_state, curr_pos);

    vector<int> assigned(m, -1);

    // change log for reverting: entries are (sample_idx, old_state, old_pos)
    vector<tuple<int, State, size_t>> changelog;
    vector<int> log_start(m, -1);

    auto revert_depth = [&](int d) {
        if (d < 0 || d >= m) return;
        if (log_start[d] == -1) return;
        int start = log_start[d];
        for (int i = (int)changelog.size() - 1; i >= start; --i) {
            auto [sidx, oldst, oldp] = changelog[i];
            curr_state[sidx] = oldst;
            curr_pos[sidx] = oldp;
        }
        changelog.resize(start);
        log_start[d] = -1;
    };

    int idx = 0;
    while (idx >= 0) {
        if (idx == m) {
            // validate using curr_state/curr_pos
            if (validate_automaton_func(automaton, positive_samples, negative_samples, curr_state, curr_pos)) {
                Automaton final_automaton = broken_automaton;
                for (int k = 0; k < m; ++k) {
                    final_automaton.transition_function.set_transition(missing_edges[k].first, missing_edges[k].second, (State)assigned[k]);
                }
                return {true, final_automaton};
            }

            // backtrack from depth m to m-1: revert changes of depth m-1
            idx--;
            if (idx >= 0) revert_depth(idx);
            continue;
        }

        // before trying next assignment at this depth, revert any previous attempt's changes
        revert_depth(idx);

        assigned[idx]++;

        if (assigned[idx] >= (int)automaton.num_states) {
            // exhausted options -> reset transition to invalid and backtrack
            const auto [from, sym] = missing_edges[idx];
            automaton.transition_function.set_transition(from, sym, automaton.transition_function.invalid_edge);
            assigned[idx] = -1;
            idx--;
            continue;
        }

        // set transition for this missing edge
        const auto [from_state, symbol] = missing_edges[idx];
        automaton.transition_function.set_transition(from_state, symbol, (State)assigned[idx]);

        // ensure deeper missing edges are invalid and unassigned
        for (int j = idx + 1; j < m; ++j) {
            const auto [df, ds] = missing_edges[j];
            automaton.transition_function.set_transition(df, ds, automaton.transition_function.invalid_edge);
            assigned[j] = -1;
            if (log_start[j] != -1) revert_depth(j);
        }

        // record changelog start for this depth
        log_start[idx] = (int)changelog.size();

        // selective update of samples affected by this assignment
        for (size_t s = 0; s < samples_size; ++s) {
            const vector<Alphabet> *sptr;
            size_t len;
            if (s < positive_samples.samples.size()) { sptr = &positive_samples.samples[s]; len = sptr->size(); }
            else { sptr = &negative_samples.samples[s - positive_samples.samples.size()]; len = sptr->size(); }

            size_t pos = curr_pos[s];
            if (pos >= len) continue;
            if (curr_state[s] != from_state) continue;
            if ((*sptr)[pos] != symbol) continue;

            // sample is affected: save old state/pos and advance as far as possible
            changelog.emplace_back((int)s, curr_state[s], curr_pos[s]);

            State cs = automaton.transition_function.get_transition(curr_state[s], symbol);
            size_t p = pos;
            if (cs != automaton.transition_function.invalid_edge) {
                p = pos + 1;
                while (p < len) {
                    const Alphabet a = (*sptr)[p];
                    const State nx = automaton.transition_function.get_transition(cs, a);
                    if (nx == automaton.transition_function.invalid_edge) break;
                    cs = nx;
                    p++;
                }
            }

            curr_state[s] = cs;
            curr_pos[s] = p;
        }

        // move deeper
        idx++;
    }

    // not fixable
    return {false, broken_automaton};
}

} // namespace SavePrefixState
