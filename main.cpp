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

    Pair(int x) : levels(0), id(0) { }
    Pair(int lvl, int id) : levels(lvl), id(id) { }
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


    int numOfItems = 10;
    Avl<int, Pair> tree = Avl<int, Pair>();

    for (int i = 1; i <= numOfItems; i++)
    {
        cout << "#" << i << " = " << (tree.insert(i * 7, Pair(i % (numOfItems / 2), i)) == 0 ? "OK" : "Fail") << endl;
    }
    cout << endl << "Inserted " << numOfItems << " items !" << endl;

    cout << "Print tree keys in-order: (Ascending)" << endl;
    tree.printInOrder(false);

    cout << "Max key is: " << tree.GetMaxKey() << endl;
    cout << "Data of " << tree.GetMaxKey() << " is: " << tree.getData(tree.GetMaxKey()) << endl;

    cout << "Main Finished !" << endl;
    return 0;
}