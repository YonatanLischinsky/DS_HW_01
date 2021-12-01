#include "pair.h"

using namespace ds;

namespace ds {
    ostream& operator << (ostream& os, const Pair& p)
    {
        os << "(" << p.levels << ", " << p.id << ")";
        return os;
    }

    bool operator < (const Pair& a, const Pair& b)
    {
        if (a.levels < b.levels)
            return true;
        if (a.levels == b.levels)
            return (a.id > b.id);
        return false;
    }

    bool operator == (const Pair& a, const Pair& b)
    {
        return (a.id == b.id);
    }
}