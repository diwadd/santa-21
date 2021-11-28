#pragma once

#include <bits/stdc++.h>

using namespace std;

int energy_left(vector<int> &initial_state, vector<vector<int>> &distance_matrix);
int energy_right(vector<int> &initial_state,vector<vector<int>> &distance_matrix);

int energy_delta(vector<int> &initial_state,
                 vector<vector<int>> &distance_matrix,
                 pair<int,int> &swaps,
                 int current_energy);