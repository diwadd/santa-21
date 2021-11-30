#pragma once

#include <bits/stdc++.h>
#include "Link.hpp"
#include "MarkedPermutationsLimits.hpp"

using namespace std;

enum class OperationType {
    swap,
    transfer,
    transfer_swap
};

MarkedPermutationsLimits mark_permutations(vector<vector<int>> &permutations,
                                           vector<int> &condition);

void mark_left_neighbour_in_first_chain_link_ptr(vector<Link> &chain, int string_id, Link *left);

void mark_right_neighbour_in_last_chain_link_ptr(vector<Link> &chain, int string_id, Link *right);

vector<Link> create_permutation_chain_ptr(vector<vector<int>> &permutations,
                                          MarkedPermutationsLimits &mpl,
                                          int number_of_sub_chains);

int sub_chain_energy_right(vector<Link> &state,
                          vector<vector<int>> &distance_matrix,
                          int chain_id);

int sub_chain_energy_left(vector<Link> &state,
                          vector<vector<int>> &distance_matrix,
                          int chain_id);

OperationType get_operation_type(vector<Link> &initial_state, int i, int j, bool transfer);

vector<int> chain_energy_left(vector<Link> &state,
                              vector<vector<int>> &distance_matrix,
                              int number_of_sub_chains);

vector<int> chain_energy_right(vector<Link> &state,
                               vector<vector<int>> &distance_matrix,
                               int number_of_sub_chains);

int energy_delta_for_swap(vector<Link> &state,
                          vector<vector<int>> &distance_matrix,
                          int current_energy,
                          pair<int,int> p);

void perform_link_swap(vector<Link> &state, pair<int, int> p);

pair<int, int> energy_delta_for_transfer(vector<Link> &state,
                                         vector<vector<int>> &distance_matrix,
                                         pair<int, int> e,
                                         pair<int,int> p);

pair<int, int> energy_delta_for_transfer_swap(vector<Link> &state,
                                              vector<vector<int>> &distance_matrix,
                                              pair<int, int> e, 
                                              pair<int,int> p);

void make_transfer(vector<Link> &state, pair<int, int> p);