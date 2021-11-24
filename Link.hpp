#pragma once

#include <bits/stdc++.h>

using namespace std;

struct Link {
    int permutation_id;
    int string_id;
    int left;
    int right;
    Link *m_left;
    Link *m_right;

    Link(int p, int s, Link *l, Link *r) :
        permutation_id(p),
        string_id(s),
        left(0),
        right(0),
        m_left(l),
        m_right(r)
    {
    }

    Link(int p, int s, int l, int r) :
        permutation_id(p),
        string_id(s),
        left(l),
        right(r),
        m_left(nullptr),
        m_right(nullptr)
    {
    }

    Link(const Link &l1) {
        // cout << "Link copy" << endl;
        permutation_id = l1.permutation_id;
        string_id = l1.string_id;
        // left = l1.left;
        // right = l1.right;
    }

    Link& operator=(Link &l1) {
        // cout << "Link operator=" << endl;
        permutation_id = l1.permutation_id;
        string_id = l1.string_id;
        // left = l1.left;
        // right = l1.right;
        return *this;
    }

    bool is_left_present(){
        if(left >= 0)
            return true;
        else
            return false;
    }

    bool is_right_present(){
        if(right >= 0)
            return true;
        else
            return false;
    }

};

ostream& operator<<(ostream& os, const Link& lk)
{
    os << "(" << lk.permutation_id << "," << lk.string_id << "," << lk.m_left << "," << lk.m_right << ")";
    return os;
}