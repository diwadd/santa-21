#include <bits/stdc++.h> 
#include "helper_functions.hpp"
#include "constants.hpp"
#include "energy_calculation.hpp"
#include "metropolis_super_perm.hpp"
#include "Link.hpp"
#include "MarkedPermutationsLimits.hpp"

using namespace std;

void assert_energy_calculation(vector<int> &state,
                               vector<vector<int>> &matrix,
                               pair<int,int> &p) {

    cout << " --- --- --- " << endl;
    print_vector(state);

    int el = energy_left(state, matrix);
    int er = energy_right(state, matrix);

    cout << "el = " << el << " er = " << er << endl;

    assert(el == er);

    int ed = energy_delta(state, matrix, p, el);
    swap(state[p.first], state[p.second]);

    el = energy_left(state, matrix);

    cout << "el = " << el << " ed = " << ed << endl;


    assert(el == ed);
}

void verify_energy() {

    int n = 3;
    vector<vector<int>> permutations = get_permutations(n);

    print_matrix(permutations);

    vector<int> v1 = {1,2,3,4};
    vector<int> v2 = {4,3,2,1};

    int o = offset(v1, v2);
    cout << "o: " << o << endl;

    vector<vector<int>> distance_matrix(permutations.size(), vector<int>(permutations.size() , 0));

    calculate_distance_matrix(permutations, distance_matrix);

    print_matrix(distance_matrix);

    vector<int> initial_state = {0,3,4,2,1,5};
    pair<int,int> p = {0, 5};
    assert_energy_calculation(initial_state, distance_matrix, p);
    
    p = {1, 4};
    assert_energy_calculation(initial_state, distance_matrix, p);

    p = {4, 1};
    assert_energy_calculation(initial_state, distance_matrix, p);

    p = {1, 2};
    assert_energy_calculation(initial_state, distance_matrix, p);

    p = {2, 1};
    assert_energy_calculation(initial_state, distance_matrix, p);


    initial_state = {0, 2, 1, 3, 4, 5};
    p = {3, 5};
    assert_energy_calculation(initial_state, distance_matrix, p);

    p = {0, 5};
    assert_energy_calculation(initial_state, distance_matrix, p);

    initial_state = {0, 3, 1, 2, 4, 5};
    p = {5, 3};
    assert_energy_calculation(initial_state, distance_matrix, p);

    p = {0, 3};
    assert_energy_calculation(initial_state, distance_matrix, p);

    initial_state = {2, 5, 1, 3, 4, 0};
    p = {1, 0};
    assert_energy_calculation(initial_state, distance_matrix, p);

    initial_state = {3, 5, 2, 4, 1, 0};
    p = {0, 5};
    assert_energy_calculation(initial_state, distance_matrix, p);

    initial_state = {0, 3, 1, 4, 5, 2};
    p = {4 ,5};
    assert_energy_calculation(initial_state, distance_matrix, p);

}


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


void mark_left_neighbour_in_first_chain_link(vector<Link> &chain, int string_id, int value) {
    
    for(int i = 0; i < chain.size(); i++) {
        if(chain[i].string_id == string_id) {
            chain[i].left = value;
            break;
        }
    }
}

void mark_right_neighbour_in_last_chain_link(vector<Link> &chain, int string_id, int value) {
    
    for(int i = chain.size()-1; i >= 0 ; i--) {
        if(chain[i].string_id == string_id) {
            chain[i].right = value;
            break;
        }
    }
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

vector<Link> create_permutation_chain(vector<vector<int>> &permutations,
                                      MarkedPermutationsLimits &mpl,
                                      int number_of_sub_chains) {

    int n = permutations.size();
    vector<Link> chain;
    chain.reserve(n);

    for(int i = 1; i <= number_of_sub_chains-1; i++) {
        for(int j = mpl.start; j <= mpl.stop; j++) {

            chain.push_back(Link{j, i, -1, -1});

        }
    }


    for(int i = 0; i < n; i++) {
        chain.push_back(Link{i, number_of_sub_chains, -1, -1});
    }

    chain.shrink_to_fit();

    for(int i = 1; i <= number_of_sub_chains; i++) {
        for(int j = 0; j < chain.size(); j++) {

            if(chain[j].string_id != i)
                continue;

            chain[j].left = j - 1;
            chain[j].right = j + 1;
        }
    }

    for(int i = 1; i <= number_of_sub_chains; i++) {
        mark_left_neighbour_in_first_chain_link(chain, i, -1);
        mark_right_neighbour_in_last_chain_link(chain, i, -1);
    }

    return chain;
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


// bool swap_links(vector<Link> &chain, MarkedPermutationsLimits &mpl, int l1, int l2) {
    
//     if(chain[l1].string_id == chain[l2].string_id) {
//         swap(chain[l1].permutation_id, chain[l2].permutation_id);
//         return true;
//     } else {

//         if( (mpl.start <= chain[l1].permutation_id and chain[l1].permutation_id <= mpl.stop) or 
//             (mpl.start <= chain[l2].permutation_id and chain[l2].permutation_id <= mpl.stop) ) {
//                 return false;
//         }

//         swap(chain[l1].permutation_id, chain[l2].permutation_id);
//         swap(chain[l1].string_id, chain[l2].string_id);

        

//     }
// }

int sub_chain_energy_left(vector<Link> &state,
                          vector<vector<int>> &distance_matrix,
                          int chain_id) {

    int e = 0;
    for(int i = 0; i < state.size(); i++) {

        Link *central = &state[i];

        // Link *left = nullptr;
        // if(i - 1 >= 0 && central->is_left_present()) {
        //     left = &state[ distance_matrix[state[i-1]] ];
        // }


    }

    return e;
}

int chain_energy_left(vector<Link> &initial_state,
                      vector<vector<int>> &distance_matrix,
                      int number_of_sub_chains) {

    
    int e = 0;

    for(int i = 1; i <= number_of_sub_chains; i++) {

    }


    return 0;

}


void run_metropolis_on_chain(vector<Link> &initial_state,
                             vector<vector<int>> &distance_matrix,
                             int number_of_sub_chains,
                             int k_max = 100) {

    int n = initial_state.size();
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uniform_integers(0, n-1);
    uniform_real_distribution<long double> uniform_reals(0.0, 1.0);

    long double d_k_max = static_cast<long double>(k_max);
    int e = chain_energy_left(initial_state, distance_matrix, number_of_sub_chains);
    cout << "Starting energy: " << e << endl;
    for(int k = 0; k < k_max; k++) {

    }

    cout << "Final state has energy: " << e << " and is equal to:" << endl;
    print_vector(initial_state);

}



int main() {

    // verify_energy();

    // int n = 4;
    // vector<vector<int>> permutations = get_permutations(n);
    // vector<vector<int>> distance_matrix(permutations.size(), vector<int>(permutations.size() , 0));
    // calculate_distance_matrix(permutations, distance_matrix);
    
    // int m = factorial(n);
    // vector<int> initial_state(m, 0);
    // iota(initial_state.begin(), initial_state.end(), 0);

    // int k_max = 20000000;

    // cout << "Running for n = " << n << endl;

    // chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    // run_metropolis(initial_state, distance_matrix, k_max);
    // chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

    // cout << "Elapsed time: " << chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " [s]" << std::endl;

    // // print_matrix(permutations);

    // vector<int> sp = get_super_permutation(initial_state, distance_matrix, permutations);
    // print_vector(sp);

    // cout << "Super permutation size: " << sp.size() << endl;


    //----------------------------------------------------------------


    // vector<string> str_vec = read_permutations("permutations.csv");

    // map<uint32_t, int> symbol_mapping = { {2240716784, 1},    // Santa
    //                                       {3064242160, 2},    // His wife
    //                                       {2359730160, 3},    // Reindeer
    //                                       {2645008368, 4},    // Elf
    //                                       {2223939568, 5},    // Christmas tree
    //                                       {2173607920, 6},    // Present
    //                                       {2156830704, 7} };  // Ribon

    // vector<uint32_t> symbols = convert_string(str_vec.front());
    // for(auto & s : symbols) {
    //     cout << dec << s << endl;
    // }

    // vector<vector<int>> permutations = convert_string_permutations_to_int_ones(str_vec, symbol_mapping);

    // assert(permutations.size() == PERMUTATION_OF_SEVEN);

    // print_vector(permutations[0]);
    // print_vector(permutations[1]);

    // vector<vector<int>> distance_matrix(permutations.size(), vector<int>(permutations.size() , 0));
    // calculate_distance_matrix(permutations, distance_matrix);


    // -----------------------------------------------------------------------------------------------


    int n = 4;
    vector<vector<int>> permutations = get_permutations(n);
    vector<vector<int>> distance_matrix(permutations.size(), vector<int>(permutations.size() , 0));
    calculate_distance_matrix(permutations, distance_matrix);
    
    int m = factorial(n);
    vector<int> initial_state(m, 0);
    iota(initial_state.begin(), initial_state.end(), 0);


    print_matrix(permutations);

    vector<int> condition = {2, 1};
    MarkedPermutationsLimits mpl = mark_permutations(permutations, condition);

    cout << "Mpl: " << mpl << endl;

    int number_of_sub_chains = 3;
    vector<Link> chain = create_permutation_chain_ptr(permutations, mpl, number_of_sub_chains);

    
    run_metropolis_on_chain(chain, distance_matrix, number_of_sub_chains);

    
    for(int i = 0; i < chain.size(); i++) {
        cout << "i: " << i << " " << chain[i] << endl;
    }

    // int x = 2;
    // int y = 4;
    // // swap_links(chain, mpl, x, y);

    // cout << " --- " << endl;

    // for(int i = 0; i < chain.size(); i++) {
    //     cout << "i: " << i << " " << chain[i] << endl;
    // }


}