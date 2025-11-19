#pragma once

EdgeStats computeEdgeStats(const Automaton &B, const Samples &positive, const Samples &negative) {
    EdgeStats stats;
    stats.sample_count.assign(B.num_states, vector<int>(B.num_alphabet, 0));
    stats.total_count.assign(B.num_states, vector<int>(B.num_alphabet, 0));
    stats.max_per_word.assign(B.num_states, vector<int>(B.num_alphabet, 0));

    auto process = [&](const Samples &S) {
        for (const auto &word : S) {

            unordered_set<long long> visited_once;
            unordered_map<long long, int> local_count;   

            int state = B.start_state;

            for (auto c : word) {
                int next = B.transition_function.get_transition(state, c);

                if (next == B.transition_function.invalid_edge)
                    break;

                long long id = ((long long)state << 32) | c;

                if (!visited_once.count(id)) {
                    stats.sample_count[state][c]++;
                    visited_once.insert(id);
                }

                stats.total_count[state][c]++;
                local_count[id]++;
                state = next;
            }

            for (auto &[id, cnt] : local_count) {
                int s = id >> 32;
                int c = id & 0xFFFFFFFF;
                stats.max_per_word[s][c] = max(stats.max_per_word[s][c], cnt);
            }
        }
    };

    process(positive);
    process(negative);

    return stats;
}