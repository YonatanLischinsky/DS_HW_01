#ifndef AVL_H
#define AVL_H

#include <memory>
#include <stdio.h>
#include <iostream>
#include "library1.h"


namespace ds
{
    template<typename T, typename K>
    class Avl;

    typedef enum
    {
        LEFT = 0,
        RIGHT
    } INSERT_DIRECTION;

    typedef enum
    {
        LL = 0,
        LR,
        RL,
        RR
    } GILGOL_TYPE;

#pragma region Node

    template<class T, class K>
    class Node
    {
    public:
        Node(T data, K key); //Constructor
        Node(const Node& node); //copy constructor
        ~Node(); //destructor

        T data;
        //template<class K>
        K key;

        std::shared_ptr<Node<T,K>> left;
        std::shared_ptr<Node<T,K>> right;
        std::shared_ptr<Node<T,K>> father;

        int balance_factor;
        int height;

        void UpdateBF();
        void UpdateHeight();
        int max(int a, int b); //only used for calculating heights - keep in int !

        friend class Avl<T,K>;
    };



    /* *************************** Node Implementation Below *****************************/


    /* Node Constructor */
    template<class T, class K>
    Node<T,K>::Node(T data, K key) : data(data), key(key), left(nullptr), right(nullptr), father(nullptr), balance_factor(0), height(0)
    {
    }

    /* Node Copy constructor */
    template<class T, class K>
    Node<T,K>::Node(const Node& node) : data(node.data), key(node.key), left(node.left), right(node.right), father(node.father)
    {
    }

    /* Destructor */
    template<class T, class K>
    Node<T,K>::~Node()
    {
    }

    template<class T, class K>
    void Node<T,K>::UpdateBF()
    {
        int hL, hR;

        if (this->left == nullptr)
            hL = -1;
        else
            hL = this->left->height;

        if (this->right == nullptr)
            hR = -1;
        else
            hR = this->right->height;

        this->balance_factor = hL - hR;
    }

    template<class T, class K>
    void Node<T,K>::UpdateHeight()
    {
        int Hl = -1, Hr = -1;
        if (this->left == nullptr)
            Hl = -1;
        else
            Hl = this->left->height;

        if (this->right == nullptr)
            Hr = -1;
        else
            Hr = this->right->height;

        this->height = max(Hl, Hr) + 1;
    }

    template<class T, class K>
    int Node<T,K>::max(int a, int b)
    {
        return (a >= b ? a : b);
    }

#pragma endregion


#pragma region Avl Tree
    template<class T, class K>
    class Avl
    {
    public:
        Avl(); //constructor
        ~Avl(); //destructor

        StatusType insert(T data, K key);
        void printInOrder(bool reverse);

    private:
        std::shared_ptr<Node<T,K>> root;

        //private helper functions:

        void AfterInsertCheckTree(std::shared_ptr< Node<T,K> > v);
        void Gilgol(std::shared_ptr< Node<T,K> > r, GILGOL_TYPE type);
        std::shared_ptr< Node<T,K> > binarySearch(K insert_key, std::shared_ptr< Node<T,K> > sub_root, INSERT_DIRECTION* dir);
        void recInOrder(std::shared_ptr<Node<T,K>> r, bool reverse);
        void deleteTree(std::shared_ptr<Node<T,K>> r);

    };


    /* *************************** Avl Implementation Below *****************************/

    /* AVL Constructor */
    template<class T, class K>
    Avl<T,K>::Avl() : root(nullptr)
    {
    }

    /* Destructor */
    template<class T, class K>
    Avl<T,K>::~Avl()
    {
        deleteTree(this->root);
    }

    template<class T,class K>
    void Avl<T,K>::deleteTree(std::shared_ptr<Node<T,K>> r)
    {
        if(r == nullptr)
            return;

        deleteTree(r->left);
        deleteTree(r->right);

        r->father = nullptr;
        r->left = nullptr;
        r->right = nullptr;
    }

    template<class T, class K>
    void Avl<T,K>::printInOrder(bool reverse)
    {
        recInOrder(this->root, reverse);
        std::cout << std::endl;
    }

    template<class T, class K>
    void Avl<T,K>::recInOrder(std::shared_ptr<Node<T,K>> r, bool reverse)
    {
        if (r == nullptr)
            return;
        if (!reverse)
            recInOrder(r->left, reverse);
        else
            recInOrder(r->right, reverse);
        std::cout << r->key << " ";
        if (!reverse)
            recInOrder(r->right, reverse);
        else
            recInOrder(r->left, reverse);
    }

    template<class T, class K>
    StatusType Avl<T,K>::insert(T data, K key)
    {
        std::shared_ptr< Node<T,K> > nodeToInsert(new Node<T,K>(data, key));
        if (nodeToInsert == nullptr)
            throw std::bad_alloc();

        if (this->root == nullptr)
        {
            this->root = nodeToInsert;
            return SUCCESS;
        }

        //Check if already exists:
        INSERT_DIRECTION dir;
        std::shared_ptr< Node<T,K> > father = binarySearch(key, this->root, &dir);
        if (father == nullptr) //Key already exists !
        {
            return FAILURE;
        }

        if (dir == LEFT)
        {
            father->left = nodeToInsert;
        }
        else
        {
            father->right = nodeToInsert;
        }
        nodeToInsert->father = father;

        //Make gilgols:
        AfterInsertCheckTree(nodeToInsert);

        return SUCCESS;
    }

    template<class T, class K>
    void Avl<T,K>::AfterInsertCheckTree(std::shared_ptr< Node<T,K> > v)
    {
        while (v != this->root)
        {
            std::shared_ptr< Node<T,K> > p = v->father;
            if (p->height >= v->height + 1)
                return;

            p->height = v->height + 1;
            p->UpdateBF();

            switch (p->balance_factor)
            {
            case 2:
                if (p->left->balance_factor >= 0)
                {
                    Gilgol(p, LL);
                }
                else if (p->left->balance_factor == -1) //maybe else here ?...
                {
                    Gilgol(p, LR);
                }
                return;

            case -2:
                if (p->right->balance_factor <= 0)
                {
                    Gilgol(p, RR);
                }
                else if (p->right->balance_factor == 1) //maybe else here ?...
                {
                    Gilgol(p, RL);
                }
                return;

            default: //BF is ok
                v = p;
                break;
            }
        }
    }

    template<class T, class K>
    void Avl<T,K>::Gilgol(std::shared_ptr< Node<T,K> > r, GILGOL_TYPE type)
    {
        switch (type)
        {
        case LL:
        {
            std::shared_ptr< Node<T,K> > tempA = r->left;

            r->left = tempA->right;
            if(tempA->right != nullptr)
                tempA->right->father = r;
            tempA->right = r;
            tempA->father = r->father;

            if (r->father != nullptr)
            {
                if (r->father->right == r)
                {
                    r->father->right = tempA;
                }
                else
                {
                    r->father->left = tempA;
                }
            }
            r->father = tempA;


            r->UpdateHeight();
            tempA->UpdateHeight();

            if (this->root == r)
                this->root = tempA;
        }
        break;

        case RR:
        {
            std::shared_ptr< Node<T,K> > tempA = r->right;

            r->right = tempA->left;
            if(tempA->left != nullptr)
                tempA->left->father = r;
            tempA->left = r;
            tempA->father = r->father;

            if (r->father != nullptr)
            {
                if (r->father->right == r)
                {
                    r->father->right = tempA;
                }
                else
                {
                    r->father->left = tempA;
                }
            }
            r->father = tempA;

            r->UpdateHeight();
            tempA->UpdateHeight();

            if (this->root == r)
                this->root = tempA;
        }
        break;

        case LR:
        {
            std::shared_ptr< Node<T,K> > tempA = r->left;
            std::shared_ptr< Node<T,K> > tempB = r->left->right;
            r->left = tempB->right;
            if (tempB->right != nullptr)
                tempB->right->father = r;

            tempA->right = tempB->left;
            if (tempB->left != nullptr)
                tempB->left->father = tempA;

            tempB->left = tempA;
            tempA->father = tempB;

            tempB->right = r;

            if (r->father != nullptr)
            {
                if (r->father->right == r)
                {
                    r->father->right = tempB;
                }
                else
                {
                    r->father->left = tempB;
                }
            }
            r->father = tempB; //Keep this under the if/else

            tempA->UpdateHeight();
            r->UpdateHeight();
            tempB->UpdateHeight();

            if (r == this->root)
                this->root = tempB;
        }
        break;

        case RL:
        {
            std::shared_ptr< Node<T,K> > tempA = r->right;
            std::shared_ptr< Node<T,K> > tempB = r->right->left;
            r->right = tempB->left;
            if(tempB->left != nullptr)
                tempB->left->father = r;

            tempA->left = tempB->right;
            if(tempB->right != nullptr)
                tempB->right->father = tempA;

            tempB->right = tempA;
            tempA->father = tempB;

            tempB->left = r;

            if (r->father != nullptr)
            {
                if (r->father->right == r)
                {
                    r->father->right = tempB;
                }
                else
                {
                    r->father->left = tempB;
                }
            }
            r->father = tempB; //Keep this under the if/else

            tempA->UpdateHeight();
            r->UpdateHeight();
            tempB->UpdateHeight();

            if (r == this->root)
                this->root = tempB;
        }
        break;
        }
    }

    template<class T, class K>
    std::shared_ptr< Node<T,K> > Avl<T,K>::binarySearch(K insert_key, std::shared_ptr< Node<T,K> > sub_root, INSERT_DIRECTION* dir)
    {
        if (dir == nullptr)
        {
            //should'nt get in here...
            return nullptr;
        }

        if (sub_root.get() == nullptr)
        {
            return nullptr;
        }

        if (insert_key < sub_root->key)
        {
            if (sub_root->left == nullptr)
            {
                *dir = LEFT;
                return sub_root;
            }
            return binarySearch(insert_key, sub_root->left, dir);
        }
        else if (insert_key == sub_root->key)
        {
            return nullptr;
        }
        else
        {
            if (sub_root->right == nullptr)
            {
                *dir = RIGHT;
                return sub_root;
            }
            return binarySearch(insert_key, sub_root->right, dir);
        }
    }

#pragma endregion
}

#endif