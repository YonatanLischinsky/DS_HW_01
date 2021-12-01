#ifndef PAIR_H
#define PAIR_H

namespace ds
{
    class Pair
    {
        public:
            int levels;
            int id;

            Pair(int lvl, int id) : levels(lvl), id(id)
            { }
            friend bool operator < (const Pair& a, const Pair& b);
            friend bool operator == (const Pair& a, const Pair& b);
            friend ostream& operator << (ostream& os, const Pair& p);
    };
}

#endif