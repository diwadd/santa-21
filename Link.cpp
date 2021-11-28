#include <bits/stdc++.h>
#include "Link.hpp"

using namespace std;


ostream& operator<<(ostream& os, const Link& lk)
{
    os << "(" << lk.permutation_id << "," << lk.string_id << "," << lk.m_left << "," << &lk << "," << lk.m_right << ")";
    return os;
}