#ifndef AVL.H
#define AVL.H
#include <memory>
#include "library1.h"


namespace ds
{
    typedef enum
    {
        LEFT = 0,
        RIGHT
    } INSERT_DIRECTION ;

    typedef enum
    {
        LL = 0,
        LR,
        RL,
        RR
    } GILGOL_TYPE ;

    template<class T>
    class Avl
    {
    public:
        Avl(); //constructor
        ~Avl(); //destructor

        StatusType Avl<T>::insert(T data, int key);

        //Avl(const Avl&); //copy constructor

    private:
        class Node;
        std::shared_ptr<Node> root;

        //private helper functions:
        std::shared_ptr<Avl<T>::Node> binarySearch(int insert_key, std::shared_ptr<Avl<T>::Node> sub_root, INSERT_DIRECTION* dir);
        void AfterInsertCheckTree(std::shared_ptr<Avl<T>::Node> v);
    };



#pragma region Node


    /* *************************** Node Implementation Below *****************************/
    template<class T>
    class Avl<T>::Node
    {
    public:
        Node(T& data, int key); //Constructor
        Node(const Node& node); //copy constructor
        ~Node(); //destructor

        T data;
        //template<class K>
        int key;
        int balance_factor;
        int height;

        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::shared_ptr<Node> father;

        void UpdateBF();
        void UpdateHeight();

        
    };

    /* Node Constructor */
    template<class T>
    //template<class K>
    Avl<T>::Node::Node(T& data, int key) : data(data), key(key), left(nullptr), right(nullptr), father(nullptr), balance_factor(0), height(0)
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

    template<class T>
    void Avl<T>::Node::UpdateBF()
    {
        int hL, hR;

        if(this->left == nullptr)
            hL = -1;
        else
            hL = this->left->height;

        if(this->right == nullptr)
            hR = -1;
        else
            hR = this->right->height;
            
        this->balance_factor = hL - hR;
    }

    template<class T>
    void Avl<T>::Node::UpdateHeight()
    {
        this->height = max(this->left->height, this->right->height) + 1;
    }
#pragma endregion
    
#pragma region Avl Tree
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
    }

    template<class T>
    StatusType Avl<T>::insert(T data, int key)
    {
        if(this->root == nullptr)
        {
            try
            {
                std::shared_ptr< Avl<T>::Node > nodeToInsert ( new Node(data, key));
                this->root = nodeToInsert;
                return StatusType.SUCCESS;
            }
            catch(const std::exception& e)
            {
                return StatusType.ALLOCATION_ERROR;
            }
        }
        
        //Check if already exists:
        INSERT_DIRECTION dir;
        std::shared_ptr<Avl<T>::Node> father = binarySearch(key, this->root, &dir);
        if(father == nullptr) //Key already exists !
        {
            return StatusType.FAILURE;
        }

        try
        {
            std::shared_ptr< Avl<T>::Node > nodeToInsert ( new Node(data, key));
            if(dir == INSERT_DIRECTION.LEFT)
            {
                father->left = nodeToInsert;
            }
            else
            {
                father->right = nodeToInsert;
            }

            //Make gilgols:

        }
        catch(const std::exception& e)
        {
            return StatusType.ALLOCATION_ERROR;
        }
    }

    template<class T>
    void Avl<T>::AfterInsertCheckTree(std::shared_ptr<Avl<T>::Node> v)
    {
        while(v != this->root)
        {
            std::shared_ptr<Avl<T>::Node> p = v->father;
            if(p->height >= v->height + 1)
                return;
            p->height = v->height + 1;

            p->UpdateBF();


            switch (p->balance_factor)
            {
            case 2:
                if(p->left->balance_factor >= 0)
                {
                    Gilgol(p, GILGOL_TYPE.LL);
                }
                else if(p->left->balance_factor == -1) //maybe else here ?...
                {
                    Gilgol(p, GILGOL_TYPE.LR);
                }
                return;

            case -2:
                if(p->right->balance_factor >= 0)
                {
                    Gilgol(p, GILGOL_TYPE.RR);
                }
                else if(p->right->balance_factor == -1) //maybe else here ?...
                {
                    Gilgol(p, GILGOL_TYPE.RL);
                }
                return;

            default: //BF is ok
                v = p;
                break;
            }
        }
    }
    
    template<class T>
    void Gilgol(std::shared_ptr<Avl<T>::Node> r, GILGOL_TYPE type)
    {
        switch (type)
        {
        case GILGOL_TYPE.LL:
                std::shared_ptr<Avl<T>::Node> tempA = r->left;

                r->left = tempA->right;
                tempA->right->father = r;
                tempA->right = r;
                tempA->father = r->father;
                
                if(r->father->right == r)
                {
                    r->father->right = tempA;
                }
                else
                {
                    r->father->left = tempA;
                }
                r->father = tempA;

                            
                r->UpdateHeight();
                tempA->UpdateHeight();
            break;
        
        case GILGOL_TYPE.RR:
                std::shared_ptr<Avl<T>::Node> tempA = r->right;

                r->right = tempA->left;
                tempA->left->father = r;
                tempA->left = r;
                tempA->father = r->father;
                
                if(r->father->right == r)
                {
                    r->father->right = tempA;
                }
                else
                {
                    r->father->left = tempA;
                }
                r->father = tempA;

                r->UpdateHeight();
                tempA->UpdateHeight();
            break;

        case GILGOL_TYPE.LR:
            std::shared_ptr<Avl<T>::Node> tempA = r->left;
            std::shared_ptr<Avl<T>::Node> tempB = r->left->right;
            r->left = tempB->right;
            tempB->right->father = r;

            tempA->right = tempB->left;
            tempB->left->father = tempA;

            tempB->left = tempA;
            tempA->father = tempB;

            tempB->right = r;

            if(r->father->right == r)
            {
                r->father->right = tempB;
            }
            else
            {
                r->father->left = tempB;
            }
            r->father = tempB; //Keep this under the if/else

            tempA->UpdateHeight();
            r->UpdateHeight();
            tempB->UpdateHeight();

            break;

        case GILGOL_TYPE.RL:
            std::shared_ptr<Avl<T>::Node> tempA = r->right;
            std::shared_ptr<Avl<T>::Node> tempB = r->right->left;
            r->right = tempB->left;
            tempB->left->father = r;

            tempA->left = tempB->right;
            tempB->right->father = tempA;

            tempB->right = tempA;
            tempA->father = tempB;

            tempB->left = r;

            if(r->father->right == r)
            {
                r->father->right = tempB;
            }
            else
            {
                r->father->left = tempB;
            }
            r->father = tempB; //Keep this under the if/else

            tempA->UpdateHeight();
            r->UpdateHeight();
            tempB->UpdateHeight();
            break;

        default:
            break;
        }
    }

    template<class T>
    std::shared_ptr<Avl<T>::Node> Avl<T>::binarySearch(int insert_key, std::shared_ptr<Avl<T>::Node> sub_root, INSERT_DIRECTION* dir)
    {
        if(dir == nullptr)
        {
            //should'nt get in here...
            return nullptr;
        }

        if(sub_root.get() == nullptr)
        {
            return nullptr;
        }

        if(insert_key < sub_root->key)
        {
            if(sub_root->left == nullptr)
            {
                *dir = INSERT_DIRECTION.LEFT;
                return sub_root;
            }
            return binarySearch(insert_key, sub_root->left, dir);
        }
        else if( insert_key == sub_root->key)
        {
            *dir = INSERT_DIRECTION.FAIL;
            return nullptr;
        }
        else
        {
            if(sub_root->right == nullptr)
            {
                *dir = INSERT_DIRECTION.RIGHT;
                return sub_root;
            }
            return binarySearch(insert_key, sub_root->right, dir);
        }
    }
#pragma endregion
}
#endif