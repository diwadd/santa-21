#pragma once

#include <bits/stdc++.h>
#include "constants.hpp"

using namespace std;

int energy_left(vector<int> &initial_state, matrix_data_type (*distance_matrix)[PERMUTATION_OF_SEVEN][PERMUTATION_OF_SEVEN]);
int energy_right(vector<int> &initial_state,matrix_data_type (*distance_matrix)[PERMUTATION_OF_SEVEN][PERMUTATION_OF_SEVEN]);

int energy_delta(vector<int> &initial_state,
                 matrix_data_type (*distance_matrix)[PERMUTATION_OF_SEVEN][PERMUTATION_OF_SEVEN],
                 pair<int,int> &swaps,
                 int current_energy);