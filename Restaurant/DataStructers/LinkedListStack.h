#pragma once
#include"StackADT.h"
#include"../Generic_DS/Node.h"



template<class T>
class LinkedListStack:public StackADT<T>
{
private:
    Node<T>* top;
    int count;

public:
    LinkedListStack();
    bool isEmpty();
    bool push(T item);
    bool pop(T&it);
    T peek();
    

};

template<class T>
inline LinkedListStack<T>::LinkedListStack()
{
    top = nullptr;
    count = 0;
}

template<class T>
inline bool LinkedListStack<T>::isEmpty()
{
    return(count == 0);
}

template<class T>
inline bool LinkedListStack<T>::push(T item)
{
    if (top == nullptr)
    {
        Node<T>* nptr = new Node<T>;
        nptr->setItem(item);
        top = nptr;
        nptr->setNext(nullptr);
        count++;
        return true;
    }
    Node<T>* nptr = new Node<T>;
    nptr->setItem(item);
    nptr->setNext(top);
    top = nptr;
    count++;
    return true;

}

template<class T>
inline bool LinkedListStack<T>::pop(T&it)
{
    if (top == nullptr)
        return false;
    if (top->getNext() == nullptr)
    {
        Node<T>* temp = top;
        it = top->getItem();
        top = nullptr;
        delete temp;
        count--;
        return true;
    }
    Node<T>* temp = top;
    it = top->getItem();
    top = top->getNext();
    
    count--;
    return true;
}

template<class T>
inline T LinkedListStack<T>::peek()
{
    return top->getItem();
}







