#pragma once

#include <bits/stdc++.h>

using namespace std;

int factorial(int n);

void convert_four_char_string_to_four_uint8(string &four_char_string);

uint32_t convert_four_char_string_to_uint32(string &four_char_string);

vector<uint32_t> convert_string(string &s);

void replace_big_ints_to_small_ones(vector<uint32_t> &v1, vector<int> &v2, map<uint32_t, int> &symbol_mapping);

vector<vector<int>> convert_string_permutations_to_int_ones(vector<string> &str_vec, map<uint32_t, int> &symbol_mapping);

vector<string> read_permutations(string filename);
vector<int> get_base_permutation(int n);
vector<vector<int>> get_permutations(int n);

vector<int> get_super_permutation(vector<int> &initial_state,
                                  vector<vector<int>> &distance_matrix,
                                  vector<vector<int>> &permutations);

template<typename T> void print_vector(vector<T> &vec) {

    int n = vec.size();
    for(int i = 0; i < n; i++) {
        if(i == n - 1)
            cout << vec[i];
        else
            cout << vec[i] << " ";
    }
    cout << "\n";
}

template<typename T> void print_matrix(vector<vector<T>> &mat) {
    for(int i = 0; i < mat.size(); i++) {
        print_vector(mat[i]);
    }
}