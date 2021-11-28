#include "gtest/gtest.h"
#include <bits/stdc++.h>
#include "chain_energy_functions.hpp"
#include "helper_functions.hpp"
#include "Link.hpp"
#include "MarkedPermutationsLimits.hpp"

// g++ chain_energy_functions_test.cpp helper_functions.cpp Link.cpp  MarkedPermutationsLimits.cpp chain_energy_functions.cpp  -lgtest   -pthread  -o tempt

using namespace std;

constexpr int DEFAULT_N = 4;
constexpr int DEFAULT_NUMBER_OF_SUB_CHAINS = 3;
vector<int> DEFAULT_CONDITION = {1, 2};

class ChainEnergyTest : public ::testing::Test {

    protected:
        ChainEnergyTest() {}

        virtual ~ChainEnergyTest() {};

        virtual void SetUp() {}

        virtual void TearDown() {}

        void prepare_chain(int n, int number_of_sub_chains, vector<int> condition) {

            m_number_of_chains = number_of_sub_chains;
            m_permutations = get_permutations(n);

            m_distance_matrix = vector<vector<int>>(m_permutations.size(), vector<int>(m_permutations.size() , 0));
            calculate_distance_matrix(m_permutations, m_distance_matrix);

            int m = factorial(n);
            vector<int> initial_state(m, 0);
            iota(initial_state.begin(), initial_state.end(), 0);

            m_condition = condition;
            MarkedPermutationsLimits m_mpl = mark_permutations(m_permutations, m_condition);

            m_initial_state = create_permutation_chain_ptr(m_permutations, m_mpl, number_of_sub_chains);
        }

        void print_permutations() {
            for(int i = 0; i < m_permutations.size(); i++) {
                cout << "i = " << i << " : ";
                print_vector<int>(m_permutations[i]);
            }
        }

    int m_number_of_chains;
    vector<vector<int>> m_permutations;
    vector<vector<int>> m_distance_matrix;
    MarkedPermutationsLimits m_mpl;
    vector<int> m_condition;
    vector<Link> m_initial_state;


};


class ChainEnergyForDifferentNTest :
    public ChainEnergyTest,
    public ::testing::WithParamInterface<int> {};


TEST_P(ChainEnergyForDifferentNTest, RightLeftEnergiesShouldBeEqual) {

    int n = GetParam();
    prepare_chain(n, DEFAULT_NUMBER_OF_SUB_CHAINS, DEFAULT_CONDITION);

    if(n == 4)
        print_chain(m_initial_state);

    vector<int> left_energy = chain_energy_left(m_initial_state, m_distance_matrix, m_number_of_chains);
    vector<int> right_energy = chain_energy_right(m_initial_state, m_distance_matrix, m_number_of_chains);

    EXPECT_EQ(left_energy.size(), right_energy.size());
    EXPECT_TRUE(left_energy == right_energy);

}


INSTANTIATE_TEST_CASE_P(
        DifferentNEnegryTests,
        ChainEnergyForDifferentNTest,
        ::testing::Values(3, 4, 5));


class ChainEnergyWithDeltaCalculationSameSubChainTest :
    public ChainEnergyTest,
    public ::testing::WithParamInterface<pair<int,int>> {};



TEST_P(ChainEnergyWithDeltaCalculationSameSubChainTest, EnergyAfterDeltaCalculationShouldBeProper) {

    cout << "--- --- --- New test --- --- ---" << endl;

    prepare_chain(DEFAULT_N, DEFAULT_NUMBER_OF_SUB_CHAINS, DEFAULT_CONDITION);

    vector<int> pre_energy = chain_energy_left(m_initial_state, m_distance_matrix, m_number_of_chains);

    int i = GetParam().first;
    int j = GetParam().second;

    ASSERT_EQ( m_initial_state[i].string_id, m_initial_state[j].string_id );

    pair<int,int> p = {i, j};
    int string_id = m_initial_state[i].string_id;

    // print_chain(m_initial_state);
    // print_chain_sequence(m_initial_state, string_id);

    // cout << "Before delta " << i << " " << j << endl;

    int ed = energy_delta_for_swap(m_initial_state,
                                   m_distance_matrix,
                                   pre_energy[string_id],
                                   p);

    swap(m_initial_state[i].permutation_id, m_initial_state[j].permutation_id);

    vector<int> post_energy = chain_energy_left(m_initial_state, m_distance_matrix, m_number_of_chains);  

    // cout << "After delta ed: " << ed << " post_energy[string_id]: " << post_energy[string_id] << endl;

    // print_chain(m_initial_state);
    // print_chain_sequence(m_initial_state, string_id);

    ASSERT_TRUE( ed >= 0 );
    ASSERT_TRUE( ed == post_energy[string_id] );
    // ASSERT_TRUE( pre_energy[string_id] == post_energy[string_id] );
}


INSTANTIATE_TEST_CASE_P(
        DeltaEnergyInSameSubChainTests,
        ChainEnergyWithDeltaCalculationSameSubChainTest,
        ::testing::Values(
            make_pair<int,int>(4, 5),
            make_pair<int,int>(5, 4),
            make_pair<int,int>(4, 6),
            make_pair<int,int>(4, 7),
            make_pair<int,int>(4, 8),
            make_pair<int,int>(4, 9),
            make_pair<int,int>(4, 10),
            make_pair<int,int>(4, 11),
            make_pair<int,int>(4, 12),
            make_pair<int,int>(4, 13),
            make_pair<int,int>(4, 14),
            make_pair<int,int>(4, 15),
            make_pair<int,int>(4, 16),
            make_pair<int,int>(4, 17),
            make_pair<int,int>(4, 18),
            make_pair<int,int>(4, 19),
            make_pair<int,int>(4, 20),
            make_pair<int,int>(4, 21),
            make_pair<int,int>(4, 22),
            make_pair<int,int>(4, 23),
            make_pair<int,int>(4, 24),
            make_pair<int,int>(4, 25),
            make_pair<int,int>(4, 26),
            make_pair<int,int>(4, 27),

            make_pair<int,int>(27, 4),
            make_pair<int,int>(27, 5),
            make_pair<int,int>(27, 6),
            make_pair<int,int>(27, 7),
            make_pair<int,int>(27, 8),
            make_pair<int,int>(27, 9),
            make_pair<int,int>(27, 10),
            make_pair<int,int>(27, 11),
            make_pair<int,int>(27, 12),
            make_pair<int,int>(27, 13),
            make_pair<int,int>(27, 14),
            make_pair<int,int>(27, 15),
            make_pair<int,int>(27, 16),
            make_pair<int,int>(27, 17),
            make_pair<int,int>(27, 18),
            make_pair<int,int>(27, 19),
            make_pair<int,int>(27, 20),
            make_pair<int,int>(27, 21),
            make_pair<int,int>(27, 22),
            make_pair<int,int>(27, 23),
            make_pair<int,int>(27, 24),
            make_pair<int,int>(27, 25),
            make_pair<int,int>(27, 26),

            make_pair<int,int>(11, 4),
            make_pair<int,int>(11, 5),
            make_pair<int,int>(11, 6),
            make_pair<int,int>(11, 7),
            make_pair<int,int>(11, 8),
            make_pair<int,int>(11, 9),
            make_pair<int,int>(11, 10),
            make_pair<int,int>(11, 12),
            make_pair<int,int>(11, 13),
            make_pair<int,int>(11, 14),
            make_pair<int,int>(11, 15),
            make_pair<int,int>(11, 16),
            make_pair<int,int>(11, 17),
            make_pair<int,int>(17, 11),
            make_pair<int,int>(11, 18),
            make_pair<int,int>(11, 19),
            make_pair<int,int>(11, 20),
            make_pair<int,int>(11, 21),
            make_pair<int,int>(11, 22),
            make_pair<int,int>(11, 23),
            make_pair<int,int>(11, 24),
            make_pair<int,int>(11, 25),
            make_pair<int,int>(11, 26)


        ));


// class ChainEnergySingleSubChainSwapTest :
//     public ChainEnergyTest,
//     public ::testing::WithParamInterface<pair<int,int>> {};


// TEST_P(ChainEnergySingleSubChainSwapTest, EnergyAfterTwoSwapsShouldBeEqual) {

//     prepare_chain(DEFAULT_N, DEFAULT_NUMBER_OF_SUB_CHAINS, DEFAULT_CONDITION);

//     vector<int> pre_left_energy = chain_energy_left(m_initial_state, m_distance_matrix, m_number_of_chains);
//     vector<int> pre_right_energy = chain_energy_right(m_initial_state, m_distance_matrix, m_number_of_chains);

//     int i = GetParam().first;
//     int j = GetParam().second;

//     ASSERT_EQ( m_initial_state[i].string_id, m_initial_state[j].string_id );

//     pair<int,int> p1 = {i, j};
//     pair<int,int> p2 = {j, i};

//     perform_link_swap(m_initial_state, p1);
//     perform_link_swap(m_initial_state, p2);

//     vector<int> post_left_energy = chain_energy_left(m_initial_state, m_distance_matrix, m_number_of_chains);  
//     vector<int> post_right_energy = chain_energy_right(m_initial_state, m_distance_matrix, m_number_of_chains);  

//     ASSERT_TRUE( pre_left_energy == post_left_energy );
//     ASSERT_TRUE( pre_right_energy == post_right_energy );
// }

// INSTANTIATE_TEST_CASE_P(
//         SingleSubChainSwapTests,
//         ChainEnergySingleSubChainSwapTest,
//         ::testing::Values(
//             // First element vs others
//             make_pair<int,int>(4, 4)
//         ));


int main() {

  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();

}