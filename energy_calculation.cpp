#include <bits/stdc++.h>
#include "energy_calculation.hpp"

using namespace std;


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