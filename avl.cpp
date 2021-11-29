
#include <stdio.h>
#include "library1.h"
#include <iostream>
#include "avl.h"

using namespace std;
using namespace ds;



/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv)
{
    cout << "Main started !" << endl;

    Avl<int> tree = Avl<int>();
    tree.insert(1, 1);
    tree.insert(2, 2);
    tree.insert(2, 2);

    cout << "Inserted 3 items !" << endl;

    cout << "Main Finished !" << endl;
    
	return 0;
}