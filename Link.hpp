#pragma once

#include <bits/stdc++.h>

using namespace std;

struct Link {
    uint32_t permutation_id;
    int string_id;
    Link *m_left;
    Link *m_right;

    Link(int p, int s, Link *l, Link *r) :
        permutation_id(p),
        string_id(s),
        m_left(l),
        m_right(r)
    {
    }

    Link(const Link &l1) {
        // cout << "Link copy" << endl;
        permutation_id = l1.permutation_id;
        string_id = l1.string_id;
        m_left = l1.m_left;
        m_right = l1.m_right;
    }

    Link& operator=(const Link &l1) {
        // cout << "Link operator=" << endl;
        permutation_id = l1.permutation_id;
        string_id = l1.string_id;
        m_left = l1.m_left;
        m_right = l1.m_right;
        return *this;
    }
};

ostream& operator<<(ostream& os, const Link& lk);