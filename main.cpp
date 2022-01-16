#include <bits/stdc++.h> 
#include "helper_functions.hpp"
#include "constants.hpp"
#include "energy_calculation.hpp"
#include "metropolis_super_perm.hpp"
#include "Link.hpp"
#include "MarkedPermutationsLimits.hpp"
#include "chain_energy_functions.hpp"
#include <emmintrin.h>


// g++ main.cpp helper_functions.cpp energy_calculation.cpp metropolis_super_perm.cpp Link.cpp MarkedPermutationsLimits.cpp chain_energy_functions.cpp -o temp

using namespace std;


int get_max_enegry(vector<int> &energy, int &number_of_sub_chains) {

    int max_e = 0;

    for(int i = 1; i <= number_of_sub_chains; i++) {
        if(energy[i] > max_e) {
            max_e = energy[i];
        }
    }

    return max_e;
}


pair<int,int> get_max_enegry_and_string_id(vector<int> &energy, int &number_of_sub_chains) {

    int max_e = 0;
    int max_id = 0;

    for(int i = 1; i <= number_of_sub_chains; i++) {
        if(energy[i] > max_e) {
            max_e = energy[i];
            max_id = i;
        }
    }

    return pair<int,int>(max_e, max_id);
}


inline double fast_pow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

inline double temperature(uint64_t k, long double d_k_max)
{
    constexpr double xf = 0.1;

    return 1 - pow(static_cast<double>(k)/d_k_max, xf);
    // return 1 - fast_pow(static_cast<double>(k)/d_k_max, xf);
    // return 1 - pow(static_cast<double>(k)/d_k_max, xf;
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

vector<vector<int>> validate_state_and_get_permutation_strings(vector<Link> &state,
                                                               MarkedPermutationsLimits &mpl,
                                                               int number_of_sub_strings,
                                                               int n_permutations) {

    vector<vector<int>> permutation_strings(number_of_sub_strings+1, vector<int>());
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


void read_submission_and_prepare_chain(string filename_chain_raw,
                                       vector<Link> &chain,
                                       int number_of_sub_chains) {

    ifstream permutations(filename_chain_raw);

    if(permutations.is_open() == false) {
        cout << "No file found" << endl;
        return;
    }


    vector<string> str_vec;

    string str; 
    while (getline(permutations, str))
    {
        str_vec.push_back(str);
    }

    assert(str_vec.size() == number_of_sub_chains);

    vector<vector<int>> simple_permutation_ids_chains(number_of_sub_chains, vector<int>());

    // Convert vector of strings 12,13,14,2423,124,1,2 to a normal vector of vectors of int.
    for(int i = 0; i < str_vec.size(); i++) {
        string s = "";
        for(int j = 0; j < str_vec[i].size(); j++) {
            if(str_vec[i][j] != ','){
                s += str_vec[i][j];
            } else if(str_vec[i][j] == ',' || str_vec[i][j] == '\n') {
                simple_permutation_ids_chains[i].push_back(stoi(s));
                s = "";
            }
        }
        simple_permutation_ids_chains[i].push_back(stoi(s));
    }

    // print_matrix(simple_permutation_ids_chains);

    // print_vector(str_vec);
    // cout << "str_vec.size(): " << str_vec.size() << endl;

    chain.clear();
    cout << "chain.size(): " << chain.size() << endl;

    for(int i = 1; i <= number_of_sub_chains; i++) {
        int p = simple_permutation_ids_chains[i-1][0];
        chain.push_back(Link{p, i, nullptr, nullptr});
        for(int j = 1; j < simple_permutation_ids_chains[i-1].size(); j++) {
            p = simple_permutation_ids_chains[i-1][j];
            chain.push_back(Link{p, i, nullptr, nullptr});
        }
    }

    chain.shrink_to_fit();

    for(int i = 1; i <= number_of_sub_chains; i++) {

        int start = 0;
        int stop = 0;

        for(int j = 0; j < chain.size(); j++) {
            if(chain[j].string_id == i) {
                start = j;
                break;
            }
        }

        for(int j = chain.size()-1; j >= 0; j--) {
            if(chain[j].string_id == i) {
                stop = j;
                break;
            }
        }

        for(int j = start+1; j <= stop; j++) {
            // cout << "Setting pointers..." << endl;
            chain[j].m_left = &chain[j-1];
            chain[j-1].m_right = &chain[j];
        }
    }

    cout << "Chain from file" << endl;
}


void prepare_and_save_submission(string filename_chain_raw,
                                 string filename_solution_raw,
                                 vector<vector<int>> &permutations,
                                 vector<Link> &chain,
                                 matrix_data_type *distance_matrix,
                                 MarkedPermutationsLimits &mpl,
                                 int number_of_sub_chains,
                                 int m) {

    vector<vector<int>> permutation_strings = validate_state_and_get_permutation_strings(chain, mpl, number_of_sub_chains, m);

    vector<vector<int>> super_permutations(number_of_sub_chains+1, vector<int>());

    ofstream chain_raw;
    chain_raw.open(filename_chain_raw);

    for(int i = 1; i < permutation_strings.size(); i++) {

        string s = "";
        for(int j = 0; j < permutation_strings[i].size(); j++) {
            if(j != permutation_strings[i].size() -1 )
                s += to_string(permutation_strings[i][j]) + ",";
            else
                s += to_string(permutation_strings[i][j]);
        }
        s += "\n";
        chain_raw << s;

        vector<int> sp = get_super_permutation(permutation_strings[i], distance_matrix, permutations);
        cout << "Super permutation length: " << sp.size() << endl;
        super_permutations[i] = sp;
    }

    chain_raw.close();

    ofstream solution_raw;
    solution_raw.open(filename_solution_raw);

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

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static uint64_t s4[4];

uint64_t next_xoshiro256_plus(void) {
	const uint64_t result = s4[0] + s4[3];

	const uint64_t t = s4[1] << 17;

	s4[2] ^= s4[0];
	s4[3] ^= s4[1];
	s4[1] ^= s4[2];
	s4[0] ^= s4[3];

	s4[2] ^= t;

	s4[3] = rotl(s4[3], 45);

	return result;
}

inline double uint64_to_double(const uint64_t x)
{
    return (x >> 11) * 0x1.0p-53;
}

void seed_xoshiro256_plus() {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint64_t> uniform_integers;

    s4[0] = uniform_integers(gen);
    s4[1] = uniform_integers(gen);
    s4[2] = uniform_integers(gen);
    s4[3] = uniform_integers(gen);
}

void copy_energy(vector<int> &new_e, vector<int> &old_e, int &number_of_sub_chains) {
    for(int i = 1; i <= number_of_sub_chains; i++) {
        new_e[i] = old_e[i];
    }
}

__m128 fast_exp (__m128 x)
{
    __m128 a = _mm_set1_ps (12102203.0f); /* (1 << 23) / log(2) */
    __m128i b = _mm_set1_epi32 (127 * (1 << 23) - 298765);
    __m128i t = _mm_add_epi32 (_mm_cvtps_epi32 (_mm_mul_ps (a, x)), b);
    return _mm_castsi128_ps (t);
}

inline double fast_exp_v2(double a)
{
   union { double d; long long x; } u;
   u.x = (long long)(6497320848556798LL * a + 0x3fef127e83d16f12LL);
   return u.d;
}

void run_metropolis_on_chain(vector<vector<int>> &permutations,
                             vector<Link> &initial_state,
                             matrix_data_type *distance_matrix,
                             MarkedPermutationsLimits &mpl,
                             int number_of_sub_chains,
                             int mm,
                             uint64_t k_max = 100,
                             double coin_prob = 0.95) {

    int n = initial_state.size();
    int n1 = n-1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uniform_integers(0, n1);
    uniform_real_distribution<long double> uniform_reals(0.0, 1.0);
    bernoulli_distribution coin_toss(coin_prob);

    long double d_k_max = static_cast<long double>(k_max);
    vector<int> energy = chain_energy_left(initial_state, distance_matrix, number_of_sub_chains);
    energy.shrink_to_fit();
    // int current_max_energy = *max_element(energy.begin(), energy.end());

    // auto [current_max_energy, current_max_energy_string_id] = get_max_enegry_and_string_id(energy);
    pair<int, int> p = get_max_enegry_and_string_id(energy, number_of_sub_chains);
    int current_max_energy = p.first;
    int starting_energy = p.first;

    cout << "Current max energy: " << current_max_energy << endl;
    cout << "Total energy: " << accumulate(energy.begin(), energy.end(), 0) << endl;

    int best_energy = numeric_limits<int>::max();

    vector<int> new_energy(number_of_sub_chains, 0);
    new_energy.shrink_to_fit();

    for(uint64_t k = 0; k < k_max; k++) {

        // if(k % 50'000'000'000 == 0) {
        //     cout << "k : " << k << " current_max_energy: " << current_max_energy << endl;
        
        //     prepare_and_save_submission("chain_raw_" + to_string(current_max_energy) + ".txt",
        //                                 "submission_raw_" + to_string(current_max_energy) + ".txt",
        //                                 permutations,
        //                                 initial_state,
        //                                 distance_matrix,
        //                                 mpl,
        //                                 number_of_sub_chains,
        //                                 mm);
        // }

        uint64_t i = 0;
        uint64_t j = 0;

        while(i == j) {
            // i = uniform_integers(gen);
            // j = uniform_integers(gen);
            i = next_xoshiro256_plus() % n; // cruel but suffices
            j = next_xoshiro256_plus() % n;
        
        }

        pair<int, int> p = {i, j};
        OperationType op = get_operation_type(initial_state, i, j);

        if(op == OperationType::swap) {

            int current_string_id = initial_state[i].string_id;
            int ed = energy_delta_for_swap(initial_state, distance_matrix, energy[current_string_id], p);

            copy_energy(new_energy, energy, number_of_sub_chains);
            new_energy[current_string_id] = ed;

            int new_max_energy = get_max_enegry(new_energy, number_of_sub_chains); 

            if( new_max_energy < current_max_energy ) {

                swap(initial_state[i].permutation_id, initial_state[j].permutation_id);


                energy[current_string_id] = ed;
                current_max_energy = new_max_energy;

                if(current_max_energy < best_energy) {
                    best_energy = current_max_energy;
                }

            } else {

                double T = temperature(k, d_k_max);
                

                double x = - (static_cast<double>(new_max_energy) - static_cast<double>(current_max_energy)) / T;
                // double d = fast_exp_v2(x);
                double d = exp(x);
                cout << "x new_max_energy: " << new_max_energy << " current_max_energy: " << current_max_energy << " x: " << x << " d: " << d << endl;
                // This is simple exp(x) but faster.
                // float xf[4] = {x, 0.0, 0.0, 0.0};
                // __m128 m128xf = _mm_load_ps(xf);
                // float d = _mm_cvtss_f32(fast_exp(m128xf));

                double r = uint64_to_double(next_xoshiro256_plus());

                // cout << "r = " << r << endl;

                if(r < d) {

                    swap(initial_state[i].permutation_id, initial_state[j].permutation_id);

                    energy[current_string_id] = ed;
                    current_max_energy = new_max_energy;
                }

            }


        } else if(op == OperationType::transfer) {

            if( (mpl.start <= initial_state[j].permutation_id && initial_state[j].permutation_id <= mpl.stop) ) {
                    continue;
            }

            int si = initial_state[i].string_id;
            int sj = initial_state[j].string_id;

            pair<int, int> ep = {energy[si], energy[sj]};
            pair<int, int> epd = energy_delta_for_transfer(initial_state, distance_matrix, ep, p);

            copy_energy(new_energy, energy, number_of_sub_chains);
            new_energy[si] = epd.first;
            new_energy[sj] = epd.second;

            int new_max_energy = get_max_enegry(new_energy, number_of_sub_chains); 

            if(new_max_energy < current_max_energy) {

                energy[si] = epd.first;
                energy[sj] = epd.second;
                current_max_energy = new_max_energy;

                make_transfer(initial_state, p);

                if(current_max_energy < best_energy) {
                    best_energy = current_max_energy;
                }

            } else {

                double T = temperature(k, d_k_max);
                

                double x = - (static_cast<double>(new_max_energy) - static_cast<double>(current_max_energy)) / T;
                
                double d = exp(x);
                // double d = fast_exp_v2(x);
                // This is simple exp(x) but faster.
                // float xf[4] = {x, 0.0, 0.0, 0.0};
                // __m128 m128xf = _mm_load_ps(xf);
                // float d = _mm_cvtss_f32(fast_exp(m128xf));

                cout << "y new_max_energy: " << new_max_energy << " current_max_energy: " << current_max_energy << " x: " << x << " d: " << d << endl;

                double r = uint64_to_double(next_xoshiro256_plus());

                if(r < d) {

                    energy[si] = epd.first;
                    energy[sj] = epd.second;
                    current_max_energy = new_max_energy;

                    make_transfer(initial_state, p);

                }

            }

        }
        // } else if (op == OperationType::transfer_swap) {

        //     if( (mpl.start <= initial_state[i].permutation_id && initial_state[i].permutation_id <= mpl.stop) || 
        //         (mpl.start <= initial_state[j].permutation_id && initial_state[j].permutation_id <= mpl.stop) ) {
        //             continue;
        //     }

        //     int si = initial_state[i].string_id;
        //     int sj = initial_state[j].string_id;

        //     pair<int, int> ep = {energy[si], energy[sj]};
        //     pair<int, int> epd = energy_delta_for_transfer_swap(initial_state, distance_matrix, ep, p);

        //     vector<int> new_energy = energy;
        //     new_energy[si] = epd.first;
        //     new_energy[sj] = epd.second;

        //     int new_max_energy = get_max_enegry(new_energy); 

        //     if(new_max_energy < current_max_energy) {

        //         energy[si] = epd.first;
        //         energy[sj] = epd.second;
        //         current_max_energy = new_max_energy;

        //         swap(initial_state[i].permutation_id, initial_state[j].permutation_id);

        //     } else {

        //         long double T = 1 - static_cast<long double>(k)/d_k_max;
        //         long double d = exp( - (static_cast<long double>(new_max_energy) - static_cast<long double>(current_max_energy)) / T );
        //         long double r = uniform_reals(gen);

        //         if(r < d) {

        //             energy[si] = epd.first;
        //             energy[sj] = epd.second;
        //             current_max_energy = new_max_energy;

        //             swap(initial_state[i].permutation_id, initial_state[j].permutation_id);

        //         }

        //     }

        // }

    }

    cout << "Starting energy was: " << starting_energy << " Final state has energy: " << current_max_energy << " and is equal to:" << endl;
    cout << "Best energy: " << best_energy << endl; 

    // print_vector(initial_state);
}


void distance_matrix_stats(matrix_data_type *distance_matrix) {

    vector<int> res(NUMBER_OF_SYMBOLS+1, 0);

    for(int i = 0; i < PERMUTATION_OF_SEVEN; i++) {
        for(int j = 0; j < PERMUTATION_OF_SEVEN; j++) {
            res[distance_matrix[ ds_index(i, j) ]]++;
        }
    }

    int n = res.size();
    int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += res[i];
    }

    cout << "Distance matrix statistics: " << endl;
    for(int i = 0; i < n; i++) {
        cout << "i = " << i << " : " << res[i] << " %: " << (static_cast<double>(res[i])/static_cast<double>(sum)) << endl;
    }

}


int main() {
    seed_xoshiro256_plus();

    vector<vector<int>> permutations = get_permutations(NUMBER_OF_SYMBOLS);
    matrix_data_type *distance_matrix = new matrix_data_type[PERMUTATION_OF_SEVEN * PERMUTATION_OF_SEVEN];
    calculate_distance_matrix(permutations, distance_matrix);
    distance_matrix_stats(distance_matrix);

    int m = factorial(NUMBER_OF_SYMBOLS);
    vector<int> initial_state(m, 0);
    iota(initial_state.begin(), initial_state.end(), 0);


    // // print_matrix(permutations);

    vector<int> condition = {4, 3};
    MarkedPermutationsLimits mpl = mark_permutations(permutations, condition);

    constexpr int number_of_sub_chains = 3;

    string filename_chain_raw = "chain_raw.txt";
    string filename_solution_raw = "solution_raw.txt";
 
    vector<Link> chain = create_permutation_chain_ptr(permutations, mpl, number_of_sub_chains);

    cout << "Chain length: " << chain.size() << endl;

    // read_submission_and_prepare_chain(filename_chain_raw, chain, number_of_sub_chains);

    // print_chain(chain);

    constexpr uint64_t k_max = 1'0'000'000;

    chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    run_metropolis_on_chain(permutations, chain, distance_matrix, mpl, number_of_sub_chains, m, k_max);
    chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

    cout << "Elapsed time: " << chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " [s]" << std::endl;

    vector<int> stats(number_of_sub_chains + 1, 0);
    for(int i = 0; i < chain.size(); i++) {
        // cout << "i: " << i << " " << chain[i] << endl;
        stats[chain[i].string_id]++;
    }

    print_vector(stats);

    cout << "Mpl: " << mpl << endl;

    prepare_and_save_submission(filename_chain_raw,
                                filename_solution_raw,
                                permutations,
                                chain,
                                distance_matrix,
                                mpl,
                                number_of_sub_chains,
                                m);

}