#include <bits/stdc++.h> 

using namespace std;


constexpr int NUMBER_OF_SYMBOLS = 7;
constexpr u_int32_t PERMUTATION_OF_SEVEN = 5040;


int factorial(int n) {
    int res = 1;
    for(int i = 1; i <= n; i++){
        res *= i;
    }
    return res;
}


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


void convert_four_char_string_to_four_uint8(string &four_char_string) {

    vector<uint8_t> symbol;

    for(const auto c : four_char_string) {
        symbol.push_back(static_cast<uint8_t>(c));
    }

    for(const uint8_t& s : symbol) {
        cout << hex << (int)s << " ";
    }
    cout << endl;

}

uint32_t convert_four_char_string_to_uint32(string &four_char_string) {

    constexpr uint32_t MASK = 0x000000FF;

    uint32_t c0 = static_cast<uint32_t>(MASK & four_char_string[0]);
    uint32_t c1 = static_cast<uint32_t>(MASK & four_char_string[1]);
    uint32_t c2 = static_cast<uint32_t>(MASK & four_char_string[2]);
    uint32_t c3 = static_cast<uint32_t>(MASK & four_char_string[3]);

    uint32_t c = (c0 << 0) | (c1 << 8) | (c2 << 16) | (c3 << 24);

    return c;
}

vector<uint32_t> convert_string(string &s) {

    // cout << "s: " << s << endl;

    constexpr uint32_t NUMBER_OF_CHARS_PER_SYMBOL = 4;

    vector<uint32_t> symbols;
    symbols.reserve(s.size() / NUMBER_OF_CHARS_PER_SYMBOL);
    for(int i = 0; i < s.size(); i += NUMBER_OF_CHARS_PER_SYMBOL) {
        string four_char_string = s.substr(i, NUMBER_OF_CHARS_PER_SYMBOL);
        uint32_t c = convert_four_char_string_to_uint32(four_char_string);
        // convert_four_char_string_to_four_uint8(four_char_string);
        symbols.push_back(c);
    }

    return symbols;
}

void replace_big_ints_to_small_ones(vector<uint32_t> &v1, vector<int> &v2, map<uint32_t, int> &symbol_mapping) {

    for(int i = 0; i < v1.size(); i++) {
        v2[i] = symbol_mapping[v1[i]];
    }
}


vector<vector<int>> convert_string_permutations_to_int_ones(vector<string> &str_vec, map<uint32_t, int> &symbol_mapping) {

    int n = str_vec.size();
    vector<vector<int>> vec(n, vector<int>(NUMBER_OF_SYMBOLS, 0));

    for(int i = 0; i < str_vec.size(); i++) {
        vector<uint32_t> symbols = convert_string(str_vec[i]);
        replace_big_ints_to_small_ones(symbols, vec[i], symbol_mapping);

    }

    return vec;
}

vector<string> read_permutations(string filename) {

    ifstream permutations(filename);


    vector<string> str_vec;
    str_vec.reserve(PERMUTATION_OF_SEVEN);


    string str; 
    while (getline(permutations, str))
    {
        str_vec.push_back(str);
    }
    str_vec.erase(str_vec.begin());

    return str_vec;
}

int hamming_distance(vector<int> &v1, vector<int> &v2) {

    assert(v1.size() == v2.size());

    int res = 0;
    for(int i = 0; i < v1.size(); i++) {
        res += static_cast<int>(v1[i] != v2[i]);
    }
    return res;
}

int offset(vector<int> &v1, vector<int> &v2) {
    
    assert(v1.size() == v2.size());

    int res = v1.size();
    for(int i = 0; i < v1.size(); i++) {
        vector<int> p1 = vector<int>(v1.begin() + i, v1.end());
        vector<int> p2 = vector<int>(v2.begin(), v2.end() - i);
        if(hamming_distance(p1, p2) == 0) {
            res = i;
            break;
        }
    }

    return res;
}

void calculate_distance_matrix(vector<vector<int>> &permutations, vector<vector<int>> &distance_matrix) {

    for(int i = 0; i < permutations.size(); i++) {
        for(int j = 0; j < permutations.size(); j++) {
            distance_matrix[i][j] = offset(permutations[i], permutations[j]);
        }
    }
}

vector<int> get_base_permutation(int n) {

    vector<int> base(n, 0);
    iota(base.begin(), base.end(), 0);
    return base;
}

vector<vector<int>> get_permutations(int n) {

    int fn = factorial(n);
    vector<vector<int>> vec;

    vector<int> base = get_base_permutation(n);

    do {
        vec.push_back(base);
    } while(std::next_permutation(base.begin(), base.end()));

    return vec;
}


int energy_left(vector<int> &initial_state,
           vector<vector<int>> &distance_matrix) {

    int e = 0;
    int n = initial_state.size();
    for(int i = 1; i < n; i++) {
        // cout << "Adding: " << distance_matrix[initial_state[i-1]][initial_state[i]] << " " << initial_state[i-1] << " " << initial_state[i] << endl;
        e += distance_matrix[initial_state[i-1]][initial_state[i]];
    }
    // cout << "Adding: " << distance_matrix[initial_state[0]][initial_state[n-1]] << " " << initial_state[0] << " " << initial_state[n-1] << endl;
    // e += distance_matrix[initial_state[0]][initial_state[n-1]];
    return e;
}

int energy_right(vector<int> &initial_state,
                 vector<vector<int>> &distance_matrix) {

    int e = 0;
    int n = initial_state.size();
    for(int i = n-2; i >= 0; i--) {
        // cout << "Adding: " << distance_matrix[initial_state[i]][initial_state[i+1]] << " " << i << " " << i+1 << endl;
        e += distance_matrix[initial_state[i]][initial_state[i+1]];
    }
    // cout << "Adding: " << distance_matrix[initial_state[0]][initial_state.size()-1] << endl;
    // e += distance_matrix[initial_state[n-1]][initial_state[0]];
    return e;
}

int energy_delta(vector<int> &initial_state,
                 vector<vector<int>> &distance_matrix,
                 pair<int,int> &swaps,
                 int current_energy) {

    int n = initial_state.size();

    int j1 = swaps.first;
    int j2 = swaps.second;

    if(j1 > j2) {
        swap(j1, j2);
    }

    // cout << "j1: " << j1 << " j2: " << j2 << endl;
    int p1 = initial_state[(j1 - 1) % n];
    int p2 = initial_state[j1];
    int p3 = initial_state[(j1 + 1) % n];

    int q1 = initial_state[(j2 - 1) % n];
    int q2 = initial_state[j2];
    int q3 = initial_state[(j2 + 1) % n];

    if( j1 != 0 and j2 != n-1 and j2 - j1 != 1) {

        current_energy -= distance_matrix[p1][p2];
        current_energy -= distance_matrix[p2][p3];

        current_energy -= distance_matrix[q1][q2];
        current_energy -= distance_matrix[q2][q3];

        current_energy += distance_matrix[p1][q2];
        current_energy += distance_matrix[q2][p3];

        current_energy += distance_matrix[q1][p2];
        current_energy += distance_matrix[p2][q3];

    } else if( j1 != 0 and j2 != n-1 and j2 - j1 == 1 ) {

        // cout << "Here 1" << endl;
        current_energy -= distance_matrix[p1][p2];
        current_energy -= distance_matrix[p2][p3];
        current_energy -= distance_matrix[q2][q3];

        current_energy += distance_matrix[p1][q2];
        current_energy += distance_matrix[q2][p2];
        current_energy += distance_matrix[p2][q3];

    } else if (j1 == 0 and j2 == n-1) {

        // cout << "Here 2" << endl;
        current_energy -= distance_matrix[p2][p3];
        current_energy -= distance_matrix[q1][q2];

        current_energy += distance_matrix[q2][p3];
        current_energy += distance_matrix[q1][p2];

    } else if (j1 != 0 and j2 == n-1 and j2 - j1 != 1) {

        // cout << "Here 3" << endl;
        current_energy -= distance_matrix[p1][p2];
        current_energy -= distance_matrix[p2][p3];
        current_energy -= distance_matrix[q1][q2];

        current_energy += distance_matrix[p1][q2];
        current_energy += distance_matrix[q2][p3];
        current_energy += distance_matrix[q1][p2];

    } else if (j1 == 0 and j2 - j1 == 1) {

        // cout << "Here 4" << endl;
        current_energy -= distance_matrix[p2][p3];
        current_energy -= distance_matrix[q2][q3];

        current_energy += distance_matrix[q2][p2];
        current_energy += distance_matrix[p2][q3];
        return current_energy;  

    } else if (j2 == n - 1 and j2 - j1 == 1) {

        // cout << "Here 4.5" << endl;
        current_energy -= distance_matrix[p1][p2];
        current_energy -= distance_matrix[p2][p3];

        current_energy += distance_matrix[p1][q2];
        current_energy += distance_matrix[q2][p2];

    } else if (j1 == 0 and j2 != n-1) {

        // cout << "Here 5" << endl;
        current_energy -= distance_matrix[p2][p3];
        current_energy -= distance_matrix[q1][q2];
        current_energy -= distance_matrix[q2][q3];

        current_energy += distance_matrix[q2][p3];
        current_energy += distance_matrix[q1][p2];
        current_energy += distance_matrix[p2][q3];

    } else {

        // cout << "Here 6" << endl;
        // printf("p1: %d p2 %d p3 %d q1 %d q2 %d q3 %d\n", p1, p2, p3, q1, q2, q3);

        // printf("p1 -> p2 => %d -> %d: %d\n", p1, p2, distance_matrix[p1][p2]);
        // printf("p2 -> p3 => %d -> %d: %d\n", p2, p3, distance_matrix[p2][p3]);
        // printf("q1 -> q2 => %d -> %d: %d\n", q1, q2, distance_matrix[q1][q2]);
        // printf("q2 -> q3 => %d -> %d: %d\n", q2, q3, distance_matrix[q2][q3]);

        // printf("p1 -> q2 => %d -> %d: %d\n", p1, q2, distance_matrix[p1][q2]);
        // printf("q2 -> p3 => %d -> %d: %d\n", q2, p3, distance_matrix[q2][p3]);
        // printf("q1 -> p2 => %d -> %d: %d\n", q1, p2, distance_matrix[q1][p2]);
        // printf("p1 -> q2 => %d -> %d: %d\n", p2, q3, distance_matrix[p2][q3]);

        cout << "Something went very wrong!" << endl;
        assert(false);
    }



    return current_energy;
}

void run_metropolis(vector<int> &initial_state,
                    vector<vector<int>> &distance_matrix,
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
            long double d = exp( - (double(ed) - double(e)) / T );
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

int main() {

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

    verify_energy();

    int n = 4;
    vector<vector<int>> permutations = get_permutations(n);
    vector<vector<int>> distance_matrix(permutations.size(), vector<int>(permutations.size() , 0));
    calculate_distance_matrix(permutations, distance_matrix);
    
    int m = factorial(n);
    vector<int> initial_state(m, 0);
    iota(initial_state.begin(), initial_state.end(), 0);

    int k_max = 50000000;

    cout << "Running for n = " << n << endl;

    chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    run_metropolis(initial_state, distance_matrix, k_max);
    chrono::steady_clock::time_point stop = std::chrono::steady_clock::now();

    cout << "Elapsed time: " << chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " [µs]" << std::endl;

    // print_matrix(permutations);

}