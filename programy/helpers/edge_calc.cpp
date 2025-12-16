#pragma once
using namespace std;

EdgeStats computeEdgeStats(const Automaton &B, const Samples &positive, const Samples &negative) {
    EdgeStats stats;
    stats.sample_count.assign(B.num_states, vector<int>(B.num_alphabet, 0));
    stats.total_count.assign(B.num_states, vector<int>(B.num_alphabet, 0));
    stats.max_per_word.assign(B.num_states, vector<int>(B.num_alphabet, 0));

    auto process = [&](const Samples &S) {
        for (const auto &word : S) {

            unordered_set<string> visited_once;
            unordered_map<string, int> local_count;   

            State state = B.start_state;

            for (auto c : word) {
                State next = B.transition_function.get_transition(state, c);

                if (next == B.transition_function.invalid_edge)
                    break;

                string id = to_string(state) + "_" + to_string(c);

                if (!visited_once.count(id)) {
                    stats.sample_count[state][c]++;
                    visited_once.insert(id);
                }

                stats.total_count[state][c]++;
                local_count[id]++;
                state = next;
            }

            for (auto &[id, cnt] : local_count) {
                auto pos = id.find('_');
                size_t s = stoul(id.substr(0, pos));
                size_t c = stoul(id.substr(pos + 1));

                stats.max_per_word[s][c] = max(stats.max_per_word[s][c], cnt);
            }
        }
    };

    process(positive);
    process(negative);

    return stats;
}

vector<MissingEdgeStat> extractMissingEdgeStats(
    const Automaton &A,
    const Automaton &B,
    const EdgeStats &stats
) {
    vector<MissingEdgeStat> result;

    size_t n = stats.sample_count.size();
    Alphabet k = stats.sample_count[0].size();

    for (State u = 0; u < n; u++) {
        for (Alphabet c = 0; c < k; c++) {

            if (A.transition_function.get_transition(u, c) == A.transition_function.invalid_edge
                && B.transition_function.get_transition(u, c) != B.transition_function.invalid_edge) {
                result.push_back({
                    static_cast<int>(u),
                    static_cast<int>(c),
                    stats.sample_count[u][c],
                    stats.max_per_word[u][c],
                    stats.total_count[u][c]
                });
            }
        }
    }
    return result;
}
