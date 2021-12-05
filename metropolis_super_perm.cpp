#include <bits/stdc++.h> 
#include "energy_calculation.hpp"
#include "helper_functions.hpp"

using namespace std;

void run_metropolis(vector<int> &initial_state,
                    matrix_data_type (*distance_matrix)[PERMUTATION_OF_SEVEN][PERMUTATION_OF_SEVEN],
                    int k_max = 100) {

    int n = initial_state.size();
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uniform_integers(0, n-1);
    uniform_real_distribution<long double> uniform_reals(0.0, 1.0);

    long double d_k_max = static_cast<long double>(k_max);
    int e = energy_left(initial_state, distance_matrix);
    cout << "Starting energy: " << e << endl;
    for(int k = 0; k < k_max; k++) {
        // cout << " --- --- --- --- --- " << endl;

        int i = 0;
        int j = 0;

        while(i == j) {
            i = uniform_integers(gen);
            j = uniform_integers(gen);
        }

        pair<int, int> p = {i, j};

        int ed = energy_delta(initial_state, distance_matrix, p, e);

        // cout << "i = " << i << " j = " << j << " k = " << k << " e = " << e << " ed = " << ed << endl;

        if( ed < e) {
            swap(initial_state[i], initial_state[j]);
            e = ed;
            // int ev = energy_left(initial_state, distance_matrix);

            // cout << "State accepted - energy validation: " << ev << endl;
            // assert(e == ev);

            // print_vector(initial_state);
        } else {

            long double T = 1 - static_cast<long double>(k)/d_k_max;
            long double d = exp( - (static_cast<long double>(ed) - static_cast<long double>(e)) / T );
            long double r = uniform_reals(gen);

            // cout << "T = " << T << " d = " << d << " r = " << r << endl;

            if(r < d) {
                swap(initial_state[i], initial_state[j]);
                e = ed;
                // cout << "State accepted - energy validation: " << energy_left(initial_state, distance_matrix) << endl;
                // print_vector(initial_state);
            }

        }
    }

    cout << "Final state has energy: " << e << " and is equal to:" << endl;
    print_vector(initial_state);

}