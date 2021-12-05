#pragma once

#include <bits/stdc++.h>
#include "constants.hpp"

using namespace std;

inline uint32_t ds_index(uint32_t i, uint32_t j) {
    return i + PERMUTATION_OF_SEVEN * j;
}

matrix_data_type hamming_distance(vector<int> &v1, vector<int> &v2);

matrix_data_type offset(vector<int> &v1, vector<int> &v2);

void calculate_distance_matrix(vector<vector<int>> &permutations, matrix_data_type *distance_matrix);

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
                                  matrix_data_type *distance_matrix,
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


template<typename T> void print_chain(vector<T> &state) {
    for(int i = 0; i < state.size(); i++) {
        cout << "i = " << i << " : " << state[i] << endl;
    }
}


template<typename T> void print_chain_sequence(vector<T> &state, int string_id) {

    T *start = nullptr;
    T *stop = nullptr;

    for(int i = 0; i < state.size(); i++) {
        if(state[i].string_id == string_id) {
            if(state[i].m_left == nullptr) {
                start = &state[i];
                break;
            }
        }
    }

    while(start->m_right != nullptr) {
        cout << *start << " ";
        start = start->m_right;
    }
    cout << *start << endl;

}