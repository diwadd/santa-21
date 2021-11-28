#include <bits/stdc++.h>
#include "MarkedPermutationsLimits.hpp"

using namespace std;

ostream& operator<<(ostream& os, const MarkedPermutationsLimits & mpl)
{
    os << "[ " << mpl.start << " , " << mpl.stop << " ]" ;
    return os;
}