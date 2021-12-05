#include <bits/stdc++.h>
#include "chain_energy_functions.hpp"
#include "constants.hpp"
#include "helper_functions.hpp"

using namespace std;

MarkedPermutationsLimits mark_permutations(vector<vector<int>> &permutations,
                                           vector<int> &condition) {

    vector<int> necessary_permutations(permutations.size(), 0);

    assert(permutations.front().size() > condition.size());

    int ones = 0;
    for(int i = 0; i < permutations.size(); i++) {

        bool is_ok = true;
        for(int j = 0; j < condition.size(); j++) {
            if(permutations[i][j] != condition[j]) {
                is_ok = false;
                break;
            }
        }

        if(is_ok == true){
            necessary_permutations[i] = 1;
            ones++;
        }
    }

    bool is_contigous = false;
    
    int start = -1;
    int stop = -1;
    for(int i = 0; i < necessary_permutations.size(); i++) {
        if(necessary_permutations[i] == 1) {
            start = i;
            break;
        }
    }

    for(int i = necessary_permutations.size()-1; i >= 0; i--) {
        if(necessary_permutations[i] == 1) {
            stop = i;
            break;
        }
    }

    printf("start: %d stop %d ones: %d\n", start, stop, ones);

    if(stop - start + 1 == ones){
        is_contigous = true;
    }

    assert(is_contigous == true);

       
    return MarkedPermutationsLimits{start, stop};
}


void mark_left_neighbour_in_first_chain_link_ptr(vector<Link> &chain, int string_id, Link *left) {
    
    for(int i = 0; i < chain.size(); i++) {
        if(chain[i].string_id == string_id) {
            chain[i].m_left = left;
            break;
        }
    }
}


void mark_right_neighbour_in_last_chain_link_ptr(vector<Link> &chain, int string_id, Link *right) {
    
    for(int i = chain.size()-1; i >= 0 ; i--) {
        if(chain[i].string_id == string_id) {
            chain[i].m_right = right;
            break;
        }
    }
}


vector<Link> create_permutation_chain_ptr(vector<vector<int>> &permutations,
                                          MarkedPermutationsLimits &mpl,
                                          int number_of_sub_chains) {

    int n = permutations.size();
    vector<Link> chain;
    chain.reserve(n);

    for(int i = 1; i <= number_of_sub_chains-1; i++) {
        for(int j = mpl.start; j <= mpl.stop; j++) {

            chain.push_back(Link{j, i, nullptr, nullptr});

        }
    }


    for(int i = 0; i < n; i++) {
        chain.push_back(Link{i, number_of_sub_chains, nullptr, nullptr});
    }

    chain.shrink_to_fit();

    for(int i = 1; i <= number_of_sub_chains; i++) {
        for(int j = 0; j < chain.size(); j++) {

            if(chain[j].string_id != i)
                continue;

            chain[j].m_left = &chain[j - 1];
            chain[j].m_right = &chain[j + 1];
        }
    }

    for(int i = 1; i <= number_of_sub_chains; i++) {
        mark_left_neighbour_in_first_chain_link_ptr(chain, i, nullptr);
        mark_right_neighbour_in_last_chain_link_ptr(chain, i, nullptr);
    }

    return chain;
} 

int sub_chain_energy_right(vector<Link> &state,
                           matrix_data_type *distance_matrix,
                           int chain_id) {

    int e = 0;
    Link *stop = nullptr;

    for(int i = 0; i < state.size(); i++) {
        if(state[i].string_id == chain_id) {
            if(state[i].m_right == nullptr) {
                stop = &state[i];
                break;
            }
        }
    }

    while(stop->m_left != nullptr) {
        // e += distance_matrix[ds_index( stop->permutation_id , stop->m_left->permutation_id ];
        e += distance_matrix[ ds_index(stop->permutation_id , stop->m_left->permutation_id) ];
        stop = stop->m_left;
    }


    return e;
}

int sub_chain_energy_left(vector<Link> &state,
                          matrix_data_type *distance_matrix,
                          int chain_id) {

    int e = 0;
    Link *start = nullptr;

    for(int i = 0; i < state.size(); i++) {
        if(state[i].string_id == chain_id) {
            if(state[i].m_left == nullptr) {
                start = &state[i];
                break;
            }
        }
    }

    while(start->m_right != nullptr) {
        // e += distance_matrix[ds_index( start->permutation_id , start->m_right->permutation_id ];
        e += distance_matrix[ ds_index(start->permutation_id, start->m_right->permutation_id) ];
        start = start->m_right;
    }

    return e;
}

vector<int> chain_energy_left(vector<Link> &state,
                              matrix_data_type *distance_matrix,
                              int number_of_sub_chains) {

    vector<int> energy(number_of_sub_chains+1, 0);

    for(int i = 1; i <= number_of_sub_chains; i++) {
        energy[i] += sub_chain_energy_left(state, distance_matrix, i);
    }

    return energy;
}

vector<int> chain_energy_right(vector<Link> &state,
                               matrix_data_type *distance_matrix,
                               int number_of_sub_chains) {

    vector<int> energy(number_of_sub_chains+1, 0);

    for(int i = 1; i <= number_of_sub_chains; i++) {
        energy[i] += sub_chain_energy_right(state, distance_matrix, i);
    }

    return energy;
}

int energy_delta_for_swap(vector<Link> &state,
                          matrix_data_type *distance_matrix,
                          int current_energy,
                          pair<int,int> &p) {

    // i must not be equal to j.

    int i = p.first;
    int j = p.second;

    Link* p1 = state[i].m_left;
    Link* p2 = &state[i];
    Link* p3 = state[i].m_right;

    Link* q1 = state[j].m_left;
    Link* q2 = &state[j];
    Link* q3 = state[j].m_right;

    // cout << p1 << " " << p2 << " " << p3 << " " << q1 << " " << q2 << " " << q3 << endl;
    // cout << (p1 != nullptr) << (p3 != nullptr) << (q1 != nullptr) << (q3 != nullptr) << (p3 != q2) << (p2 != q1) << endl;

    if(p1 != nullptr && p3 != nullptr && q1 != nullptr && q3 != nullptr) {

        if( p3 != q2 && p2 != q1 && p1 != q2 && q3 != p2 ) { // 1

            // cout << "Case 1" << endl;
            current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
            current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

            current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
            current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

            current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
            current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];

            current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
            current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
            return current_energy;

        } else if( p3 != q2 && q1 != p2 && p1 == q2 && q3 == p2 ) { // 2 prime

            // cout << "Case 2 prime" << endl;
            current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
            current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
            current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

            current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
            current_energy += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
            current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
            return current_energy;

        } else if( p3 == q2 && q1 == p2 && p1 != q2 && q3 != p2 ) { // 2

            // cout << "Case 2" << endl;
            current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
            current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];
            current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

            current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
            current_energy += distance_matrix[ds_index(q2->permutation_id,p2->permutation_id)];
            current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
            return current_energy;

        }


    // if( p1 != nullptr && p3 != nullptr && q1 != nullptr && q3 != nullptr && p3 != q2 && p2 != q1 && p1 != q2 && q3 != p2 ) { // 1

    //     // cout << "Case 1" << endl;
    //     current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
    //     current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

    //     current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
    //     current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

    //     current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
    //     current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];

    //     current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
    //     current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
    //     return current_energy;

    // } else if( p1 != nullptr && p3 != nullptr && q1 != nullptr && q3 != nullptr && p3 != q2 && q1 != p2 && p1 == q2 && q3 == p2 ) { // 2 prime

    //     // cout << "Case 2 prime" << endl;
    //     current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
    //     current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
    //     current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

    //     current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
    //     current_energy += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
    //     current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
    //     return current_energy;

    // } else if( p1 != nullptr && p3 != nullptr && q1 != nullptr && q3 != nullptr && p3 == q2 && q1 == p2 && p1 != q2 && q3 != p2 ) { // 2

    //     // cout << "Case 2" << endl;
    //     current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
    //     current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];
    //     current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

    //     current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
    //     current_energy += distance_matrix[ds_index(q2->permutation_id,p2->permutation_id)];
    //     current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
    //     return current_energy;

    } else if( p1 != nullptr && p3 == nullptr && q1 == nullptr && q3 != nullptr && p3 != q2 && p2 != q1 && p1 != q2 && q3 != p2 ) { // 7 prime

        // cout << "Case 7 prime" << endl;
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
        current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];

        current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
        current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
        return current_energy;

    } else if( p1 == nullptr && p3 != nullptr && q1 != nullptr && q3 == nullptr && p3 != q2 && p2 != q1 && p1 != q2 && q3 != p2 ) { // 7

        // cout << "Case 7" << endl;
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];
        current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];

        current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
        current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
        return current_energy;

    } else if( q1 != nullptr && q3 != nullptr && p1 != nullptr && p3 == nullptr && q3 != p2 && p1 != q2 ) { // 5 prime

        // cout << "Case 5 prime" << endl;
        current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
        current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];

        current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
        current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
        current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
        return current_energy;

    } else if ( p1 != nullptr && p3 != nullptr && q1 != nullptr && q3 == nullptr && p3 != q2 && q1 != p2 ) { // 5

        // cout << "Case 5" << endl;
        current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];
        current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];

        current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
        current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
        current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
        return current_energy;
    
    } else if( q1 == nullptr && q3 != nullptr && p1 != nullptr && p3 != nullptr && q3 == p2 && p1 == q2 ) { // 3 prime

        // cout << "Case 3 prime" << endl;
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        current_energy += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
        current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
        return current_energy;

    } else if (p1 == nullptr && p3 != nullptr && q1 != nullptr && q3 != nullptr && p3 == q2 && q1 == p2 ) { // 3
        
        // cout << "Case 3" << endl;
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

        current_energy += distance_matrix[ds_index(q2->permutation_id,p2->permutation_id)];
        current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
        return current_energy;  

    } else if (p1 != nullptr && p3 == nullptr && q1 != nullptr && q3 != nullptr && q3 == p2 && p1 == q2) { // 6 prime

        // cout << "Case 6 prime" << endl;
        current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

        current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
        current_energy += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
        return current_energy;

    } else if (p1 != nullptr && p3 != nullptr && q1 != nullptr && q3 == nullptr && p3 == q2 && q1 == p2) { // 6

        // cout << "Case 6" << endl;
        current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
        current_energy += distance_matrix[ds_index(q2->permutation_id,p2->permutation_id)];
        return current_energy;

    } else if( q1 == nullptr && q3 != nullptr && p1 != nullptr && p3 != nullptr && q3 != p2 && p1 != q2 ) { // 4 prime

        // cout << "Case 4 prime" << endl;
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
        current_energy -= distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
        current_energy += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
        current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
        return current_energy;

    } else if(p1 == nullptr && p3 != nullptr && q1 != nullptr && q3 != nullptr && p3 != q2 && q1 != p2 ) { // 4

        // cout << "Case 4" << endl;
        current_energy -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];
        current_energy -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
        current_energy -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

        current_energy += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
        current_energy += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
        current_energy += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
        return current_energy;

    } else {

        cout << "Something went terribly wrong!" << endl;
        assert(false);

    }


    return current_energy;
}


void perform_link_swap(vector<Link> &state,
                       pair<int, int> p) {

    int i = p.first;
    int j = p.second;

    Link* p1 = state[i].m_left;
    Link* p2 = &state[i];
    Link* p3 = state[i].m_right;

    Link* q1 = state[j].m_left;
    Link* q2 = &state[j];
    Link* q3 = state[j].m_right;


    // cout << state[i].m_right << " == " << &state[j] << " && " << state[j].m_left <<  "==" << &state[i] << endl;
    // cout << state[i].m_left << " == " << &state[j] << " && " << state[j].m_right <<  "==" << &state[i] << endl;
    if(state[i].m_right == &state[j] && state[j].m_left == &state[i]) {

        if(p1 != nullptr)
            p1->m_right = &state[j];

        if(q3 != nullptr)
            q3->m_left = &state[i];

        state[i].m_right = state[j].m_right;
        state[j].m_left = state[i].m_left;

        state[i].m_left = &state[j];
        state[j].m_right = &state[i];

    } else if (state[i].m_left == &state[j] && state[j].m_right == &state[i]) {

        cout << " ---------------------------------------------------------------------------------------- " << endl;

        if(p3 != nullptr)
            p3->m_left = &state[j];

        if(q1 != nullptr)
            q1->m_right = &state[i];

        state[i].m_left = state[j].m_left;
        state[j].m_right = state[i].m_right;

        state[j].m_left = &state[i];
        state[i].m_right = &state[j];

    } else {

        if(p1 != nullptr)
            p1->m_right = &state[j];

        if(p3 != nullptr)
            p3->m_left = &state[j];

        if(q1 != nullptr)
            q1->m_right = &state[i];
        
        if(q3 != nullptr)
            q3->m_left = &state[i];

        state[i].m_left = state[j].m_left;
        state[i].m_right = state[j].m_right;

        state[j].m_left = p1;
        state[j].m_right = p3;    
    }

    int permutation_id = state[i].permutation_id;
    state[i].permutation_id = state[j].permutation_id;
    state[j].permutation_id = permutation_id;

}

pair<int, int> energy_delta_for_transfer_swap(vector<Link> &state,
                                              matrix_data_type *distance_matrix,
                                              pair<int, int> e, 
                                              pair<int,int> p) {
    int current_energy_i = e.first;
    int current_energy_j = e.second;

    int i = p.first;
    int j = p.second;

    Link* p1 = state[i].m_left;
    Link* p2 = &state[i];
    Link* p3 = state[i].m_right;

    Link* q1 = state[j].m_left;
    Link* q2 = &state[j];
    Link* q3 = state[j].m_right;

    int delta_i_minus = 0;
    int delta_j_minus = 0;

    int delta_i_plus = 0;
    int delta_j_plus = 0;

    if(p1 != nullptr && p3 != nullptr) {
        delta_i_minus += distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];
        delta_i_minus += distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        delta_j_plus += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
        delta_j_plus += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
    } else if(p1 == nullptr && p3 != nullptr) {
        delta_i_minus += distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        delta_j_plus += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
    } else if(p1 != nullptr && p3 == nullptr) {
        delta_i_minus += distance_matrix[ds_index(p1->permutation_id,p2->permutation_id)];

        delta_j_plus += distance_matrix[ds_index(p1->permutation_id,q2->permutation_id)];
    }

    if(q1 != nullptr && q3 != nullptr) {
        delta_j_minus += distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
        delta_j_minus += distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

        delta_i_plus += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
        delta_i_plus += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
    } else if(q1 == nullptr && q3 != nullptr) {
        delta_j_minus += distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];

        delta_i_plus += distance_matrix[ds_index(p2->permutation_id,q3->permutation_id)];
    } else if(q1 != nullptr && q3 == nullptr) {
        delta_j_minus += distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];

        delta_i_plus += distance_matrix[ds_index(q1->permutation_id,p2->permutation_id)];
    }

    current_energy_i -= delta_i_minus;
    current_energy_j -= delta_j_minus;

    current_energy_i += delta_i_plus;
    current_energy_j += delta_j_plus;

    return pair<int,int>(current_energy_i, current_energy_j);
}

pair<int, int> energy_delta_for_transfer(vector<Link> &state,
                                         matrix_data_type *distance_matrix,
                                         pair<int, int> e,
                                         pair<int,int> &p) {

    int current_energy_i = e.first;
    int current_energy_j = e.second;

    int i = p.first;
    int j = p.second;


    Link* p1 = state[i].m_left;
    Link* p2 = &state[i];
    Link* p3 = state[i].m_right;

    Link* q1 = state[j].m_left;
    Link* q2 = &state[j];
    Link* q3 = state[j].m_right;

    if(p1 != nullptr && p3 != nullptr) {
        current_energy_i -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        current_energy_i += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
        current_energy_i += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
    } else if(p1 != nullptr && p3 == nullptr) {
        current_energy_i += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
    } else if(p1 == nullptr && p3 != nullptr) {
        current_energy_i -= distance_matrix[ds_index(p2->permutation_id,p3->permutation_id)];

        current_energy_i += distance_matrix[ds_index(p2->permutation_id,q2->permutation_id)];
        current_energy_i += distance_matrix[ds_index(q2->permutation_id,p3->permutation_id)];
    }

    if(q1 != nullptr && q3 != nullptr) {
        current_energy_j -= distance_matrix[ds_index(q1->permutation_id,q2->permutation_id)];
        current_energy_j -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
        
        current_energy_j += distance_matrix[ds_index(q1->permutation_id,q3->permutation_id)];
    } else if(q1 == nullptr && q3 != nullptr) {
        current_energy_j -= distance_matrix[ds_index(q2->permutation_id,q3->permutation_id)];
    } else if(q1 != nullptr && q3 == nullptr) {
        current_energy_j -= distance_matrix[ds_index(q1->permutation_id,q2 ->permutation_id)];
    }

    return pair<int,int>(current_energy_i, current_energy_j);
}

void make_transfer(vector<Link> &state, pair<int, int> &p) {

    int i = p.first;
    int j = p.second;

    Link* p1 = state[i].m_left;
    Link* p2 = &state[i];
    Link* p3 = state[i].m_right;

    Link* q1 = state[j].m_left;
    Link* q2 = &state[j];
    Link* q3 = state[j].m_right;

    if(q1 != nullptr && q3 != nullptr) {
        q1->m_right = q2->m_right;
        q3->m_left = q2->m_left;
    } else if(q1 == nullptr && q3 != nullptr) {
        q3->m_left = nullptr;
    } else if(q1 != nullptr && q3 == nullptr) {
        q1->m_right = nullptr;
    }

    if(p1 != nullptr && p3 != nullptr) {

        q2->m_right = p2->m_right;
        p3->m_left = q2;

        p2->m_right = q2;
        q2->m_left = p2;

    } else if(p1 != nullptr && p3 == nullptr) {

        q2->m_right = nullptr;

        p2->m_right = q2;
        q2->m_left = p2;

    } else if(p1 == nullptr && p3 != nullptr) {

        q2->m_right = p2->m_right;
        p3->m_left = q2;

        p2->m_right = q2;
        q2->m_left = p2;

    }

    state[j].string_id = state[i].string_id;

}