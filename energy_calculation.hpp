#pragma once

#include <bits/stdc++.h>

using namespace std;

int hamming_distance(vector<int> &v1, vector<int> &v2);

int offset(vector<int> &v1, vector<int> &v2);

void calculate_distance_matrix(vector<vector<int>> &permutations, vector<vector<int>> &distance_matrix);


int energy_left(vector<int> &initial_state, vector<vector<int>> &distance_matrix);
int energy_right(vector<int> &initial_state,vector<vector<int>> &distance_matrix);

int energy_delta(vector<int> &initial_state,
                 vector<vector<int>> &distance_matrix,
                 pair<int,int> &swaps,
                 int current_energy);