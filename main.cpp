#include <stdio.h>
#include <iostream>
#include "playersmanager.h"

using namespace std;
using namespace ds;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char** argv)
{
    
    cout << "Main started !" << endl;


    int numOfItems = 20;
    Avl<int, Pair> tree = Avl<int, Pair>();


    for (int i = 1; i <= numOfItems; i++)
    {
       cout << "#" << i << " = " << (tree.insert(i * 7, Pair(i, i)) == 0 ? "OK" : "Fail") << endl;
    }
    cout << endl << "Inserted " << numOfItems << " items !" << endl;

    cout << "Print tree keys in-order: (Ascending) before removing:" << endl;
    tree.printInOrder(false);

    cout << (tree.remove(Pair(2, 2)) == 0 ? "Removed !" : "Fail") << endl; //remove root check
    cout << (tree.remove(Pair(7, 7)) == 0 ? "Removed !" : "Fail") << endl; //remove root check


    cout << "Print tree keys in-order: (Ascending)" << endl;
    tree.printInOrder(false);

    //cout << "Max key is: " << tree.GetMaxKey() << endl;
    //cout << "Data of " << tree.GetMaxKey() << " is: " << tree.getData(tree.GetMaxKey()) << endl;

    cout << "Main Finished !" << endl;
    
    
    
    
    return 0;
}