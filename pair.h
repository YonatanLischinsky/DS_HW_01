#ifndef PAIR_H
#define PAIR_H
#include <stdio.h>
#include <ostream>

namespace ds
{
    class Pair
    {
    public:
        int levels;
        int id;
        Pair() : levels(0), id(0) { }
        Pair(int x) : levels(0), id(0) { }
        Pair(int lvl, int id) : levels(lvl), id(id)
        { }
        friend bool operator < (const Pair& a, const Pair& b);
        friend bool operator == (const Pair& a, const Pair& b);
        friend std::ostream& operator << (std::ostream& os, const Pair& p);
    };
}

#endif