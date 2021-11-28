#include <bits/stdc++.h> 
#include "helper_functions.hpp"
#include "constants.hpp"

using namespace std;

int factorial(int n) {
    int res = 1;
    for(int i = 1; i <= n; i++){
        res *= i;
    }
    return res;
}

int hamming_distance(vector<int> &v1, vector<int> &v2) {

    assert(v1.size() == v2.size());

    int res = 0;
    for(int i = 0; i < v1.size(); i++) {
        res += static_cast<int>(v1[i] != v2[i]);
    }
    return res;
}


int offset(vector<int> &v1, vector<int> &v2) {
    
    assert(v1.size() == v2.size());

    int res = v1.size();
    for(int i = 0; i < v1.size(); i++) {
        vector<int> p1 = vector<int>(v1.begin() + i, v1.end());
        vector<int> p2 = vector<int>(v2.begin(), v2.end() - i);
        if(hamming_distance(p1, p2) == 0) {
            res = i;
            break;
        }
    }

    return res;
}


void calculate_distance_matrix(vector<vector<int>> &permutations, vector<vector<int>> &distance_matrix) {

    for(int i = 0; i < permutations.size(); i++) {
        
        if(i % 100 == 0) {
            cout << "In distance matrix, i = " << i << endl;
        }

        for(int j = 0; j < permutations.size(); j++) {
            distance_matrix[i][j] = offset(permutations[i], permutations[j]);
        }
    }
}


void convert_four_char_string_to_four_uint8(string &four_char_string) {

    vector<uint8_t> symbol;

    for(const auto c : four_char_string) {
        symbol.push_back(static_cast<uint8_t>(c));
    }

    for(const uint8_t& s : symbol) {
        cout << hex << (int)s << " ";
    }
    cout << endl;

}


uint32_t convert_four_char_string_to_uint32(string &four_char_string) {

    constexpr uint32_t MASK = 0x000000FF;

    uint32_t c0 = static_cast<uint32_t>(MASK & four_char_string[0]);
    uint32_t c1 = static_cast<uint32_t>(MASK & four_char_string[1]);
    uint32_t c2 = static_cast<uint32_t>(MASK & four_char_string[2]);
    uint32_t c3 = static_cast<uint32_t>(MASK & four_char_string[3]);

    uint32_t c = (c0 << 0) | (c1 << 8) | (c2 << 16) | (c3 << 24);

    return c;
}


vector<uint32_t> convert_string(string &s) {

    // cout << "s: " << s << endl;

    constexpr uint32_t NUMBER_OF_CHARS_PER_SYMBOL = 4;

    vector<uint32_t> symbols;
    symbols.reserve(s.size() / NUMBER_OF_CHARS_PER_SYMBOL);
    for(int i = 0; i < s.size(); i += NUMBER_OF_CHARS_PER_SYMBOL) {
        string four_char_string = s.substr(i, NUMBER_OF_CHARS_PER_SYMBOL);
        uint32_t c = convert_four_char_string_to_uint32(four_char_string);
        // convert_four_char_string_to_four_uint8(four_char_string);
        symbols.push_back(c);
    }

    return symbols;
}


void replace_big_ints_to_small_ones(vector<uint32_t> &v1, vector<int> &v2, map<uint32_t, int> &symbol_mapping) {

    for(int i = 0; i < v1.size(); i++) {
        v2[i] = symbol_mapping[v1[i]];
    }
}


vector<vector<int>> convert_string_permutations_to_int_ones(vector<string> &str_vec, map<uint32_t, int> &symbol_mapping) {

    int n = str_vec.size();
    vector<vector<int>> vec(n, vector<int>(NUMBER_OF_SYMBOLS, 0));

    for(int i = 0; i < str_vec.size(); i++) {
        vector<uint32_t> symbols = convert_string(str_vec[i]);
        replace_big_ints_to_small_ones(symbols, vec[i], symbol_mapping);

    }

    return vec;
}


vector<string> read_permutations(string filename) {

    ifstream permutations(filename);


    vector<string> str_vec;
    str_vec.reserve(PERMUTATION_OF_SEVEN);


    string str; 
    while (getline(permutations, str))
    {
        str_vec.push_back(str);
    }
    str_vec.erase(str_vec.begin());

    return str_vec;
}


vector<int> get_base_permutation(int n) {

    vector<int> base(n, 0);
    iota(base.begin(), base.end(), 0);
    return base;
}


vector<vector<int>> get_permutations(int n) {

    int fn = factorial(n);
    vector<vector<int>> vec;

    vector<int> base = get_base_permutation(n);

    do {
        vec.push_back(base);
    } while(std::next_permutation(base.begin(), base.end()));

    return vec;
}


vector<int> get_super_permutation(vector<int> &initial_state,
                                  vector<vector<int>> &distance_matrix,
                                  vector<vector<int>> &permutations) {

    // Converts the initital_state into a human readable super permutation.

    vector<int> sp = permutations[initial_state.front()];
    // print_vector(sp);
    for(int i = 1; i < initial_state.size(); i++) {

        vector<int> &p = permutations[initial_state[i]];
        int offset = distance_matrix[initial_state[i-1]][initial_state[i]];

        for(int j = p.size() - offset; j < p.size(); j++) {
            sp.push_back(p[j]);
        }
    }

    return sp;
}
