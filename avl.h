#ifndef AVL.H
#define AVL.H
#include <memory>


namespace ds
{
    template<class T>
    class Avl
    {
    public:
        Avl(); //constructor
        ~Avl(); //destructor

        void insert(T data, int key);

        //Avl(const Avl&); //copy constructor

    private:
        class Node;
        std::shared_ptr<Node> root;

        void DeleteAllNodesPostOrder(std::shared_ptr< Avl<T>::Node > r);
    };



#pragma region  Node


    /* *************************** Node Implementation Below *****************************/
    template<class T>
    class Avl<T>::Node
    {
    public:
        //template<class K>
        Node(T& data, int key); //Constructor
        Node(const Node& node); //copy constructor
        ~Node(); //destructor

        T data;
        //template<class K>
        int key;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> father;
    };

    /* Node Constructor */
    template<class T>
    //template<class K>
    Avl<T>::Node::Node(T& data, int key) : data(data), key(key), left(nullptr), right(nullptr), father(nullptr)
    {
    }

    /* Node Copy constructor */
    template<class T>
    Avl<T>::Node::Node(const Node& node) : data(node.data), key(node.key), left(node.left), right(node.right), father(node.father)
    {
    }

    /* Destructor */
    template<class T>
    Avl<T>::Node::~Node()
    {
    }
#pragma endregion
    
    
/* *************************** Avl Implementation Below *****************************/

    /* AVL Constructor */
    template<class T>
    Avl<T>::Avl() : root(nullptr)
    {
    }

    /* Destructor */
    template<class T>
    Avl<T>::~Avl()
    {
        DeleteAllNodesPostOrder(this->root);
    }

    template<class T>
    void Avl<T>::insert(T data, int key)
    {

        std::shared_ptr< Avl<T>::Node > nodeToInsert ( new Node(data, key));


        std::shared_ptr <Avl<T>::Node> ptr = this->root;

        if (ptr != nullptr)
        {
            while (ptr->left != nullptr)
            {
                ptr = ptr->left; //The visual studio didnt auto complete here... check if works???
            }
            ptr->left = nodeToInsert;
        }
        else
        {
            this->root = nodeToInsert;
        }

    }

    template<class T>
    void Avl<T>::DeleteAllNodesPostOrder(std::shared_ptr< Avl<T>::Node > r)
    {
        if (r == nullptr)
        {
            return;
        }

        DeleteAllNodesPostOrder(r->left);
        DeleteAllNodesPostOrder(r->right);

        //delete();
    }
}


#endif