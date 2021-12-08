#ifndef PAIR_H
#define PAIR_H
#include <stdio.h>
#include <ostream>

// namespace ds
// {
    class Pair
    {
    public:
        int levels;
        int id;
        Pair() : levels(-1), id(-1) { }
        Pair(int x) : levels(-1), id(-1) { }
        Pair(int lvl, int id) : levels(lvl), id(id)
        { }
        void operator = (const Pair& b)
        {
            this->id = b.id;
            this->levels = b.levels;
        }
        friend bool operator < (const Pair& a, const Pair& b);
        friend bool operator == (const Pair& a, const Pair& b);
        friend std::ostream& operator << (std::ostream& os, const Pair& p);
    };
//}

#endif