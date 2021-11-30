#include <stdio.h>
#include "library1.h"
#include <iostream>
#include "avl.h"

using namespace std;
using namespace ds;



/***************************************************************************/
/* main                                                                    */
/***************************************************************************/
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



int main(int argc, const char** argv)
{
    cout << "Main started !" << endl;


    int numOfItems = 3;
    Avl<int,Pair> tree = Avl<int, Pair>();

    //for (int i = numOfItems; i > 0; i--)
    //{
    //    cout << "#" << i << " = " << (tree.insert(i, i) == 0 ? "OK" : "Fail")<< endl;
    //}

    tree.insert(55, Pair(4, 1));
    tree.insert(109, Pair(4, 2));
    tree.insert(203, Pair(4, 3));

    cout << endl << "Inserted " << numOfItems << " items !" << endl;

    cout << "Print tree keys in-order: (Ascending)" << endl;
    tree.printInOrder(false);

    cout << endl << "Print tree keys in-order: (Descending)" << endl;
    tree.printInOrder(true);

    cout << "Main Finished !" << endl;
    return 0;
}