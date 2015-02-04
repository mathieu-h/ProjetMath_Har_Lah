// Node.h
// Author: Ali Selcuk AKYUZ
// Mail: selcuk@retinarobotics.com || e174043@metu.edu.tr
// Electrical and Electronics Engineering Department
// Middle East Technical University - ANKARA
// If any questions please send me an email

#ifndef NODE_H
#define NODE_H
#include <iostream>

using namespace std;

template<class T>
class Node
{
public:
    Node();
    Node(const T& item, Node<T>* ptrnext = NULL);
    T data;
    // access to the next node
    Node<T>* NextNode();
    // list modification methods
    void InsertAfter(Node<T>* p);
    Node<T>* DeleteAfter();
    Node<T> * GetNode(const T& item, Node<T>* nextptr = NULL);
private:
    
    Node<T> * next;
};

template<class T>
Node<T>::Node()
{
    // default constructor
    // this is to allow us to create an object without any initialization
}


//  This constructor is just to set next pointer of a node and the data contained.
template<class T>
Node<T>::Node(const T& item,Node<T>* ptrnext)
{
    this->data = item;
    this->next = ptrnext;
}

template<class T>
Node<T>*Node<T>::NextNode()
{
    return this->next;
}

//  This methods inserts a node just after the node that the method belongs to
//  TO-DO: Consider a better implementation
template<class T>
void Node<T>::InsertAfter(Node<T> *p)
{
    // not to lose the rest of the list, we ought to link the rest of the
    // list to the Node<T>* p first
    p->next = this->next;
    
    // now we should link the previous Node to Node<T> *p , i.e the Node that we are
    //inserting after,
    this->next = p;
}

// Deletes the node from the list and returns the deleted node
template<class T>
Node<T>* Node<T>::DeleteAfter()
{
    // store the next Node in a temporary Node
    Node<T>* tempNode = next;
    // check if there is a next node
    if(next != NULL)
        next = next->next;
    
    return tempNode;
}
template<class T>
Node<T> * GetNode(const T& item, Node<T>* nextptr = NULL)
{
    Node<T>* newnode; // Local ptr for new node
    newnode = new Node<T>(item,nextptr);
    if ( newnode == NULL)
    {
        cerr << "Memory allocation failed." << endl;
        exit(1);
    }
    return newnode;
}

#endif // NODE_H