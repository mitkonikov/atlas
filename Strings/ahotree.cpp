// Educational Round 16 Problem F
// Add a string s to the set D. It is guaranteed that the string s was not added before.
// Delete a string s from the set D. It is guaranteed that the string s is in the set D.
// For the given string s find the number of occurrences of the strings from the set D.
//   If some string p from D has several occurrences in s you should count all of them.
#include <bits/stdc++.h>
using namespace std;
 
// This version of aho_corasick uses a bitmask of size ALPHABET,
// so it must be modified for ALPHABET > 26.
template<char MIN_CHAR = 'a'>
struct aho_corasick {
    // suff = the index of the node of the longest strict suffix of
    //              the current node that's also in the tree.
    // dict = the index of the node of the longest strict suffix of
    //              the current node that's in the word list.
    // depth = normal trie depth (root is 0). Can be removed to save memory.
    // word_index = the index of the *first* word ending at this node. -1 if none.
    // word_count = the total number of words ending at this node.
    //                  Used in count_total_matches().
    // first_child = the first child of this node
    //                  (children are sequential due to BFS order), -1 if none.
    // child_mask = the bitmask of child keys available from this node.
    //   IMPORTANT:  If ALPHABET > 26, change the type.
    
    struct node {
        int suff = -1, dict = -1, depth = 0;
        int word_index = -1, word_count = 0;
        int first_child = -1;
        unsigned child_mask = 0;
 
        int get_child(char c) const {
            int bit = c - MIN_CHAR;
 
            if ((child_mask >> bit & 1) == 0)
                return -1;
 
            assert(first_child >= 0);
            return first_child + __builtin_popcount(child_mask & ((1 << bit) - 1));
        }
    };
 
    vector<node> nodes;
    int W = 0;
    vector<int> word_location;
    vector<int> word_indices_by_depth;
    vector<int> defer;
 
    aho_corasick(const vector<pair<string, int>> &words = {}) {
        build(words);
    }
 
    // Builds the adj list based on suffix parents.
    // Often we want to perform DP and/or queries on this tree.
    vector<vector<int>> build_suffix_adj() const {
        vector<vector<int>> adj(nodes.size());
 
        for (int i = 1; i < int(nodes.size()); i++)
            adj[nodes[i].suff].push_back(i);
 
        return adj;
    }
 
    int get_or_add_child(int current, char c) {
        int bit = c - MIN_CHAR;
 
        if (nodes[current].child_mask >> bit & 1)
            return nodes[current].get_child(c);
 
        assert(nodes[current].child_mask >> bit == 0);
        int index = int(nodes.size());
        nodes[current].child_mask |= 1 << bit;
 
        if (nodes[current].first_child < 0)
            nodes[current].first_child = index;
 
        nodes.emplace_back();
        nodes.back().depth = nodes[current].depth + 1;
        return index;
    }
 
    // Where in the trie we end up after starting at `location` and adding char `c`.
    // Runs in worst case O(depth) but amortizes to O(1) in most situations.
    int get_suffix_link(int location, char c) const {
        int child;
 
        while (location >= 0 && (child = nodes[location].get_child(c)) < 0)
            location = nodes[location].suff;
 
        return location < 0 ? 0 : child;
    }
 
    void build(const vector<pair<string, int>> &words) {
        nodes = {node()};
        W = int(words.size());
        vector<int> indices(W);
        iota(indices.begin(), indices.end(), 0);
        stable_sort(indices.begin(), indices.end(), [&](int a, int b) {
            return words[a].first < words[b].first;
        });
        word_location.assign(W, 0);
        vector<int> remaining = indices;
        int rem = W;
 
        for (int depth = 0; rem > 0; depth++) {
            int nrem = 0;
 
            for (int i = 0; i < rem; i++) {
                int word = remaining[i];
                int &location = word_location[word];
 
                if (depth >= int(words[word].first.size())) {
                    if (nodes[location].word_index < 0)
                        nodes[location].word_index = word;
 
                    nodes[location].word_count += words[word].second;
                } else {
                    location = get_or_add_child(location, words[word].first[depth]);
                    remaining[nrem++] = word;
                }
            }
 
            rem = nrem;
        }
 
        int max_depth = 0;
        defer.resize(W);
 
        for (int i = 0; i < W; i++) {
            max_depth = max(max_depth, int(words[i].first.size()));
            defer[i] = nodes[word_location[i]].word_index;
        }
 
        // Create a list of word indices in decreasing order of depth, 
        // in linear time via counting sort.
        word_indices_by_depth.resize(W);
        vector<int> depth_freq(max_depth + 1, 0);
 
        for (int i = 0; i < W; i++)
            depth_freq[words[i].first.size()]++;
 
        for (int i = max_depth - 1; i >= 0; i--)
            depth_freq[i] += depth_freq[i + 1];
 
        for (int i = W - 1; i >= 0; i--)
            word_indices_by_depth[--depth_freq[words[i].first.size()]] = i;
 
        // Solve suffix parents by traversing in order of depth (BFS order).
        for (int i = 0; i < int(nodes.size()); i++) {
            unsigned child_mask = nodes[i].child_mask;
 
            while (child_mask != 0) {
                int bit = __builtin_ctz(child_mask);
                char c = char(MIN_CHAR + bit);
                int index = nodes[i].get_child(c);
                child_mask ^= 1 << bit;
 
                // Find index's suffix parent by traversing suffix parents
                // of i until one of them has a child c.
                int suffix_parent = get_suffix_link(nodes[i].suff, c);
                nodes[index].suff = suffix_parent;
                nodes[index].word_count += nodes[suffix_parent].word_count;
                nodes[index].dict = nodes[suffix_parent].word_index < 0 ?
                    nodes[suffix_parent].dict : suffix_parent;
            }
        }
    }
 
    // Counts the number of matches of each word in O(text length + num words).
    vector<int> count_matches(const string &text) const {
        vector<int> matches(W, 0);
        int current = 0;
 
        for (char c : text) {
            current = get_suffix_link(current, c);
            int dict_node = nodes[current].word_index < 0
                ? nodes[current].dict : current;
 
            if (dict_node >= 0)
                matches[nodes[dict_node].word_index]++;
        }
 
        // Iterate in decreasing order of depth.
        for (int word_index : word_indices_by_depth) {
            int location = word_location[word_index];
            int dict_node = nodes[location].dict;
 
            if (dict_node >= 0)
                matches[nodes[dict_node].word_index] += matches[word_index];
        }
 
        for (int i = 0; i < W; i++)
            matches[i] = matches[defer[i]];
 
        return matches;
    }
 
    // Counts the number of matches over all words at
    // each ending position in `text` in O(text length).
    vector<int> count_matches_by_position(const string &text) const {
        vector<int> matches(text.size());
        int current = 0;
 
        for (int i = 0; i < int(text.size()); i++) {
            current = get_suffix_link(current, text[i]);
            matches[i] = nodes[current].word_count;
        }
 
        return matches;
    }
 
    // Counts the total number of matches of all words
    // within `text` in O(text length).
    int64_t count_total_matches(const string &text) const {
        int64_t matches = 0;
        int current = 0;
 
        for (char c : text) {
            current = get_suffix_link(current, c);
            matches += nodes[current].word_count;
        }
 
        return matches;
    }
};
 
// Enables online insertion of words into Aho-Corasick
// by adding an extra log factor to the runtime.
// The main idea is to have a distinct Aho-Corasick trie
// for each 1-bit of n, where n is the current number of words.
template<char MIN_CHAR = 'a'>
struct online_aho_corasick {
    vector<pair<string, int>> words;
    vector<aho_corasick<MIN_CHAR>> ACs;
 
    void add_word(const string &word, int delta) {
        words.emplace_back(word, delta);
        int rebuild = __builtin_ctz(int(words.size()));
 
        for (int i = 0; i < rebuild; i++)
            ACs[i] = aho_corasick<MIN_CHAR>();
 
        if (int(ACs.size()) <= rebuild)
            ACs.emplace_back();
 
        int start = int(words.size()) - (1 << rebuild);
        ACs[rebuild].build(vector<pair<string, int>>(words.begin() + start, words.end()));
    }
 
    vector<int> count_matches(const string &text) const {
        vector<int> matches;
 
        for (int i = int(ACs.size()) - 1; i >= 0; i--) {
            vector<int> ac_matches = ACs[i].count_matches(text);
            matches.insert(matches.end(), ac_matches.begin(), ac_matches.end());
        }
 
        return matches;
    }
 
    // Counts the number of matches over all words at 
    // each ending position in `text` in O(text length).
    vector<int> count_matches_by_position(const string &text) const {
        vector<int> matches(text.size(), 0);
 
        for (int i = int(ACs.size()) - 1; i >= 0; i--) {
            vector<int> ac_matches = ACs[i].count_matches_by_position(text);
 
            for (int t = 0; t < int(text.size()); t++)
                matches[t] += ac_matches[t];
        }
 
        return matches;
    }
 
    int64_t count_total_matches(const string &text) const {
        int64_t matches = 0;
 
        for (const auto &ac : ACs)
            matches += ac.count_total_matches(text);
 
        return matches;
    }
};
 
 
int main() {
    int Q;
    cin >> Q;
    online_aho_corasick AC;
 
    for (int q = 0; q < Q; q++) {
        int t;
        string str;
        cin >> t >> str;
 
        if (t == 1)
            AC.add_word(str, +1);
        else if (t == 2)
            AC.add_word(str, -1);
        else
            cout << AC.count_total_matches(str) << endl;
    }
}
