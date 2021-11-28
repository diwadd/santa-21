#include <bits/stdc++.h> 
#include "helper_functions.hpp"
#include "constants.hpp"
#include "energy_calculation.hpp"
#include "metropolis_super_perm.hpp"
#include "Link.hpp"
#include "MarkedPermutationsLimits.hpp"
#include "chain_energy_functions.hpp"

// g++ main.cpp helper_functions.cpp energy_calculation.cpp metropolis_super_perm.cpp Link.cpp MarkedPermutationsLimits.cpp chain_energy_functions.cpp -o temp

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

int get_max_enegry(vector<int> &energy) {

    int max_e = 0;

    for(int i = 1; i < energy.size(); i++) {
        if(energy[i] > max_e) {
            max_e = energy[i];
        }
    }

    return max_e;
}


pair<int,int> get_max_enegry_and_string_id(vector<int> &energy) {

    int max_e = 0;
    int max_id = 0;

    for(int i = 1; i < energy.size(); i++) {
        if(energy[i] > max_e) {
            max_e = energy[i];
            max_id = i;
        }
    }

    return pair<int,int>(max_e, max_id);
}


void run_metropolis_on_chain(vector<Link> &initial_state,
                             vector<vector<int>> &distance_matrix,
                             MarkedPermutationsLimits &mpl,
                             int number_of_sub_chains,
                             uint64_t k_max = 100,
                             double coin_prob = 0.5) {

    int n = initial_state.size();
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uniform_integers(0, n-1);
    uniform_real_distribution<long double> uniform_reals(0.0, 1.0);
    bernoulli_distribution coin_toss(coin_prob);

    long double d_k_max = static_cast<long double>(k_max);
    vector<int> energy = chain_energy_left(initial_state, distance_matrix, number_of_sub_chains);
    energy.shrink_to_fit();
    // int current_max_energy = *max_element(energy.begin(), energy.end());

    // auto [current_max_energy, current_max_energy_string_id] = get_max_enegry_and_string_id(energy);
    pair<int, int> p = get_max_enegry_and_string_id(energy);
    int current_max_energy = p.first;
    int starting_energy = p.first;

    cout << "Current max energy: " << current_max_energy << endl;
    cout << "Total energy: " << accumulate(energy.begin(), energy.end(), 0) << endl;
    for(uint64_t k = 0; k < k_max; k++) {

        if(k % 1000'000 == 0) {
            cout << "k : " << k << " current_max_energy: " << current_max_energy << endl;
        }

        int i = 0;
        int j = 0;

        while(i == j) {
            i = uniform_integers(gen);
            j = uniform_integers(gen);
        }

        pair<int, int> p = {i, j};
        OperationType op = get_operation_type(initial_state, i, j);


        if(op == OperationType::swap) {

            // cout << "OperationType::swap" << endl;
            int current_string_id = initial_state[i].string_id;
            
            // print_chain(initial_state);
            
            int ed = energy_delta_for_swap(initial_state, distance_matrix, energy[current_string_id], p);

            // cout << "--- k = " << k << " i = " << i << " j = " << j << " current_string_id:  " << current_string_id << " ed: " << ed <<  " ---" << endl;

            //swap(initial_state[i].permutation_id, initial_state[j].permutation_id);

            if(ed < 0) {
                // cout << "Energy is wrong!" << endl;
                print_chain(initial_state);
                assert(false);
            }

            vector<int> new_energy = energy;
            new_energy[current_string_id] = ed;
            // cout << "Old energy vector: ";
            // print_vector(energy);
            // cout << "New energy vector: ";
            // print_vector(new_energy);

            int new_max_energy = get_max_enegry(new_energy); 

            if( new_max_energy < current_max_energy ) {

                // cout << " --- Before swap --- " << endl;
                // print_chain(initial_state);
                // print_chain_sequence(initial_state, current_string_id);
                // cout << " --- --- --- " << endl;

                swap(initial_state[i].permutation_id, initial_state[j].permutation_id);


                energy[current_string_id] = ed;
                current_max_energy = new_max_energy;

                // perform_link_swap(initial_state, p);

                // cout << " --- After swap --- " << endl;
                // print_chain(initial_state);
                // print_chain_sequence(initial_state, current_string_id);
                // cout << " --- --- --- " << endl;


            } else {

                long double T = 1 - static_cast<long double>(k)/d_k_max;
                long double d = exp( - (static_cast<long double>(new_max_energy) - static_cast<long double>(current_max_energy)) / T );
                long double r = uniform_reals(gen);

                if(r < d) {

                    // cout << " --- Before swap --- " << endl;
                    // print_chain(initial_state);
                    // print_chain_sequence(initial_state, current_string_id);
                    // cout << " --- --- --- " << endl;

                    // perform_link_swap(initial_state, p);
                    swap(initial_state[i].permutation_id, initial_state[j].permutation_id);

                    // cout << " --- After swap --- " << endl;
                    // print_chain(initial_state);
                    // print_chain_sequence(initial_state, current_string_id);
                    // cout << " --- --- --- " << endl;

                    energy[current_string_id] = ed;
                    current_max_energy = new_max_energy;
                }

            }


        } else if(op == OperationType::transfer) {

            if( (mpl.start <= initial_state[j].permutation_id && initial_state[j].permutation_id <= mpl.stop) ) {
                    continue;
            }

            // cout << "OperationType::transfer" << endl;

            int si = initial_state[i].string_id;
            int sj = initial_state[j].string_id;

            pair<int, int> ep = {energy[si], energy[sj]};
            pair<int, int> epd = energy_delta_for_transfer(initial_state, distance_matrix, ep, p);

            vector<int> new_energy = energy;
            new_energy[si] = epd.first;
            new_energy[sj] = epd.second;

            // cout << "Old energy vector: ";
            // print_vector(energy);
            // cout << "New energy vector: ";
            // print_vector(new_energy);

            int new_max_energy = get_max_enegry(new_energy); 

            if(new_max_energy < current_max_energy) {
                // cout << "Taken" << endl;

                energy[si] = epd.first;
                energy[sj] = epd.second;
                current_max_energy = new_max_energy;

                make_transfer(initial_state, p);

            } else {

                long double T = 1 - static_cast<long double>(k)/d_k_max;
                long double d = exp( - (static_cast<long double>(new_max_energy) - static_cast<long double>(current_max_energy)) / T );
                long double r = uniform_reals(gen);

                if(r < d) {

                    // cout << "Taken" << endl;

                    energy[si] = epd.first;
                    energy[sj] = epd.second;
                    current_max_energy = new_max_energy;

                    make_transfer(initial_state, p);

                }

            }

        }

    }

    cout << "Starting energy was: " << starting_energy << " Final state has energy: " << current_max_energy << " and is equal to:" << endl;
    // print_vector(initial_state);

}


vector<int> get_string_with_specific_id(vector<Link> &state, int string_id) {

    Link *start = nullptr;
    for(int i = 0; i < state.size(); i++) {
        if(state[i].string_id == string_id) {
            if(state[i].m_left == nullptr) {
                start = &state[i];
                break;
            }
        }
    }

    vector<int> permutation_string_rep;
    while(start->m_right != nullptr) {
        permutation_string_rep.push_back(start->permutation_id);
        start = start->m_right;
    }
    permutation_string_rep.push_back(start->permutation_id);

    return permutation_string_rep;
}

vector<vector<int>> validate_state(vector<Link> &state, MarkedPermutationsLimits &mpl, int number_of_sub_strings, int n_permutations) {

    vector<vector<int>> permutation_strings(number_of_sub_strings+1, vector<int>());

    // for(int i = 1; i <= number_of_sub_strings; i++) {
    //     // permutation_strings[i] = get_string_with_specific_id(state, i);
    //     // sort(permutation_strings[i].begin(), permutation_strings[i].end());
    //     // print_vector(permutation_strings[i]);
    // }

    vector<int> permutations(n_permutations, 0);

    for(int i = 1; i <= number_of_sub_strings; i++) {
        permutation_strings[i] = get_string_with_specific_id(state, i);
        for(int m = mpl.start; m <= mpl.stop; m++) {
            if( find(permutation_strings[i].begin(), permutation_strings[i].end(), m) == permutation_strings[i].end() ) {
                assert(false);
            }
        }

        for(int j = 0; j < permutation_strings[i].size(); j++) {
            permutations[permutation_strings[i][j]]++;
        }

    }


    for(int i = 0; i < permutations.size(); i++) {
        if(permutations[i] == 0)
            assert(false);
    }


    return permutation_strings;
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


    int n = 7;
    vector<vector<int>> permutations = get_permutations(n);
    vector<vector<int>> distance_matrix(permutations.size(), vector<int>(permutations.size() , 0));
    calculate_distance_matrix(permutations, distance_matrix);
    
    cout << "Distance ok" << endl;

    int m = factorial(n);
    vector<int> initial_state(m, 0);
    iota(initial_state.begin(), initial_state.end(), 0);


    print_matrix(permutations);

    vector<int> condition = {4, 3};
    MarkedPermutationsLimits mpl = mark_permutations(permutations, condition);

    int number_of_sub_chains = 3;
    vector<Link> chain = create_permutation_chain_ptr(permutations, mpl, number_of_sub_chains);

    uint64_t k_max = 1000'000'000'000'000;
    run_metropolis_on_chain(chain, distance_matrix, mpl, number_of_sub_chains, k_max);


    vector<int> stats(number_of_sub_chains + 1, 0);
    for(int i = 0; i < chain.size(); i++) {
        // cout << "i: " << i << " " << chain[i] << endl;
        stats[chain[i].string_id]++;
    }

    print_vector(stats);

    cout << "Mpl: " << mpl << endl;
    vector<vector<int>> permutation_strings = validate_state(chain, mpl, number_of_sub_chains, m);

    vector<vector<int>> super_permutations(number_of_sub_chains+1, vector<int>());

    for(int i = 1; i < permutation_strings.size(); i++) {
        vector<int> sp = get_super_permutation(permutation_strings[i], distance_matrix, permutations);
        cout << "Super permutation length: " << sp.size() << endl;
        // print_vector(sp);
        super_permutations[i] = sp;
    }

    ofstream solution_raw;
    solution_raw.open("solution_raw.txt");

    for(int i = 1; i < super_permutations.size(); i++) {
        string s = "";
        for(int j = 0; j < super_permutations[i].size(); j++) {
            s += to_string(super_permutations[i][j]);
        }
        s += "\n";
        // cout << s;
        solution_raw << s;
    }
    solution_raw.close();

}