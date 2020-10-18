// AVLSet.hpp
//
// ICS 46 Winter 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    ~AVLSet() noexcept override;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.a  
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.

    struct Node
    {
        ElementType data; 
        Node* left;
        Node* right;
        Node* parent;
        int treeHeight;

        Node()
        {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            treeHeight = -1;
        }

        ~Node()
        {
            delete left;
            delete right;
        }
    };

    Node* root;
    int treeSize = 0;
    bool balanced = true; 

    //insert an element into the tree. 
    void insert(Node*& previous, const ElementType& data);
    void copyNode(Node* target, Node* source) const;
    bool search(Node* root, const ElementType& data) const;
    int maxHeight(Node* root) const;
    void preorderHelp(Node* start, VisitFunction visit) const;
    void inorderHelp(Node* start, VisitFunction visit) const;
    void postorderHelp(Node* start, VisitFunction visit) const;
    int diff(Node* &start) const;
    void rr(Node* parent);
    void ll(Node* parent);
    void lr(Node* parent);
    void rl(Node* parent);
    void balance(Node* parent);
    void print(Node* current);
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    root = nullptr;
    treeSize = 0;
    balanced = shouldBalance;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    delete root;
}

//copy a node from a tree
template <typename ElementType>
void AVLSet<ElementType>::copyNode(Node* target, Node* source) const
{
    if(source == nullptr)
    {
        target = nullptr;
    }
    else
    {
        target = new Node;
        target->data = source->data; 
        copyNode(target->left, source->left);
        copyNode(target->right, source->right);
    }
    
}

//insert an element into the tree. 
template <typename ElementType>
void AVLSet<ElementType>::insert(Node*& previous, const ElementType& data)
{
    if(treeSize == 0)
    {
        Node* temp = new Node;
        temp -> parent = nullptr;
        temp -> left = nullptr;
        temp -> right = nullptr;
        temp -> data = data;
        temp -> treeHeight = 0; 
        root = temp;
    }
    else if(data < previous->data)
    {
        if(previous->left == nullptr)
        {
            Node* temp = new Node;
            temp -> parent = previous;
            temp -> left = nullptr;
            temp -> right = nullptr;
            temp -> data = data;
            temp -> treeHeight = previous->treeHeight +1;
            previous->left = temp;
        }
        else
        {
            insert(previous->left, data);
            if(balanced == true)
            {
             balance(previous);
            }
        }     
    }
    else
    {
        if(previous->right == nullptr)
        {
            Node* temp = new Node;
            temp -> parent = previous;
            temp -> left = nullptr;
            temp -> right = nullptr;
            temp -> data = data;
            temp -> treeHeight = previous->treeHeight +1;
            previous->right = temp;
        }
        else
        {
            insert(previous->right, data);
            if(balanced == true)
            {
             balance(previous);
            }
        }     
    }
}

template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
    treeSize = s.treeSize;
    balanced = s.balanced;

    if(s.root == nullptr)
    {
        root = nullptr;
    }
    else
    {
        copyNode(root, s.root);
    }    
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
    root = s.root;
    treeSize = s.treeSize;
    balanced = s.balanced;

    s.root = nullptr;
    s.treeSize = 0;
    s.balanced = true;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    this-> ~AVLSet();
    treeSize = s.treeSize;
    balanced = s.balanced;
    if(s.root == nullptr)
    {
        root = nullptr;
    }
    else
    {
        copyNode(root, s.root);
    }

    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    this-> ~AVLSet();
    treeSize = s.treeSize;
    balanced =s.balanced;
    root = s.root;

    s.root = nullptr;
    s.treeSize = 0;

    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    if(contains(element) == false)
    {
        insert(root, element);
        treeSize++;
        //std::cout << treeSize << std::endl;
    }
}

//search for an element
template <typename ElementType>
bool AVLSet<ElementType>::search(Node* root, const ElementType& data) const
{
    bool exist = false;
    if(root == nullptr)
    {
        return exist;
    }
    else if(root-> data == data)
    {
        exist = true;
        return exist;
    }

    if(data<root->data)
    {
        return search(root->left, data);
    }

    return search(root->right, data);

}

template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    bool exist = false;
    exist = search(root, element);

    return exist;
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return treeSize;
}

template <typename ElementType>
int AVLSet<ElementType>::maxHeight(Node* root) const
{
    if(root == nullptr)
    {
        return -1;
    }
    else
    {
        int leftHeight = maxHeight(root->left);
        int rightHeight = maxHeight(root->right);

        if(leftHeight>rightHeight)
        {
            return leftHeight+1;
        }
        else
        {
            return rightHeight+1;
        }  
    }
}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    if(treeSize == 0)
    {
        return -1; 
    }
    return maxHeight(root);
}

template <typename ElementType>
void AVLSet<ElementType>:: preorderHelp(Node* start, VisitFunction visit) const
{
    visit(start->data);
    if(start->left != nullptr)
    {
        preorderHelp(start->left, visit);
    }
    if(start->right != nullptr)
    {
        preorderHelp(start->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorderHelp(root, visit);
}

template <typename ElementType>
void AVLSet<ElementType>:: inorderHelp(Node* start, VisitFunction visit) const
{

    if(start->left != nullptr)
    {
        inorderHelp(start->left, visit);
    }
    visit(start->data);
    if(start->right != nullptr)
    {
        inorderHelp(start->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorderHelp(root, visit);
}

template <typename ElementType>
void AVLSet<ElementType>:: postorderHelp(Node* start, VisitFunction visit) const
{
    if(start->left != nullptr)
    {
        postorderHelp(start->left, visit);
    }
    if(start->right != nullptr)
    {
        postorderHelp(start->right, visit);
    }
    visit(start->data);
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorderHelp(root, visit);
}

template <typename ElementType>
int AVLSet<ElementType>::diff(Node* &start) const
{
    int lh = maxHeight(start->left);
    int rh = maxHeight(start->right);
    int d = lh-rh;
    return d;
}

template <typename ElementType>
void AVLSet<ElementType>::rr(Node* start)
{
    std::cout << " RR " << std::endl;
    Node* temp;
    temp = start->right;
    if(root == start)
    {
        root = temp;
        start->right = temp->left;
        temp->left = start;
        temp->parent = nullptr;
        start->parent = temp;
    }
    else
    {
        temp->parent = start->right;
        temp->parent = start->parent;
        start->parent->right = temp;
        start->right = temp->left;
        start->parent = temp;
        temp->left = start;
    }
    

    //start->parent = temp;   
}

template <typename ElementType>
void AVLSet<ElementType>::ll(Node* start)
{
    std::cout << "LL " << std::endl;
    Node* temp;
    temp = start->left;
    if(root == start)
    {
    root = temp;
    start->left = temp->right;
    temp->parent = nullptr;
    //std::cout << "one" << std::endl;
    temp -> right = start;
    //std::cout << "two" << std::endl;
    start->parent = temp;
    }
    else
    {
    start->left = temp->right;
    temp->parent = start -> parent;
    //std::cout << "one" << std::endl;
    temp -> right = start;
    //std::cout << "two" << std::endl;
    start->parent->left = temp;
    //std::cout << "three" << std::endl;
    start->parent = temp;
    // std::cout << "balancing complete. the root is:" << root->data << "root left is " << root->left->data << " center: "  <<  temp->data <<  " " << "left: " << 
    // temp->left->data << " right: "<< temp->right-> data << std::endl;
    }
}

template <typename ElementType>
void AVLSet<ElementType>::lr(Node* start)
{
    std::cout << "LR then " ;
    Node* temp;
    temp = start->left->right;
    start->left->right = nullptr;
    temp->left = start->left;
    start->left = temp;
    temp -> parent = start;
    temp ->left ->parent = temp;
    ll(start);
}

template <typename ElementType>
void AVLSet<ElementType>::rl(Node* start)
{
    std::cout<< "RL then ";
    Node* temp;
    // temp = start->right->left;
    // start->right = temp->right;
    // temp->right = start->right;
    // start->right = temp;
    temp = start -> right;
    start ->right = temp ->left; 
    temp ->parent = start -> right;
    start->right->right = temp;
    start->right->parent = start;
    temp -> left = nullptr; 
    rr(start);
}

template <typename ElementType>
void AVLSet<ElementType>::balance(Node* start)
{
    //std::cout << "balancing" << std::endl;
    int difference = diff(start);
    if(difference > 1)
    {
        if(diff(start->left)>0)
        {
            //std::cout << "using LL" << std::endl;
            ll(start);

        }
        else
        {

            //std::cout << "using LR" << std::endl;
            lr(start);
        }       
    }
    else if(difference < -1)
    {
        if(diff(start->right)>0)
        {
            rl(start);
        }
        else
        {
            rr(start);
        }
    }
    
}

#endif // AVLSET_HPP

