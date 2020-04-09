#pragma once
#include"PriorNode.h"


template<class T>
class PriorityQueue
{
private:
	PriorNode<T>* front;
	PriorNode<T>* back;
	int count;
public:

	PriorityQueue();               // constructor to intilze count and friont , back pointers 
	bool enqueue(T item, int p);  
	bool dequeue(T& item);
	bool isEmpty();
	T peek();
	~PriorityQueue();            // clearing
	
};

template<class T>
inline PriorityQueue<T>::PriorityQueue()
{
	front = nullptr;
	back = nullptr;
}

template<class T>
inline bool PriorityQueue<T>::enqueue(T item, int p)
{
	PriorNode<T>* nptr = new PriorNode<T>;
	nptr->setItem(item);
	nptr->setPriority(p);

	if (front == nullptr ) //empty queue
	{
		front = nptr;
		count++;
		return true;
	}

	if (front->getPriority() < p) //trying to enqueue an element with priority greater than front 
	{
		nptr->setNext(front);
		front = nptr;
		count++;
		return true;
	}

	PriorNode<T>* ptr = front->getNext(); //keeping tracks of elements with relative priorties 
	PriorNode<T>* ptr1 = front; 

	while (ptr  &&  ptr->getPriority() >= p) //to advance thr pointers based on thr priority
	{
		ptr = ptr->getNext();
		ptr1 = ptr1->getNext();
	}

	nptr->setNext(ptr);
	ptr1->setNext(nptr);
	count++;
	return true;
}

template<class T>
inline bool PriorityQueue<T>::dequeue(T& item)
{
	if (front == nullptr)
		return false;

	PriorNode<T>* temp = front;
	item = temp->getItem();
	front = front->getNext();
 	
	count--;

	return true;
}

template<class T>
inline bool PriorityQueue<T>::isEmpty()
{
	return (front==nullptr);
}

template<class T>
inline T PriorityQueue<T>::peek()
{
	return front->getItem();
}

template<class T>
inline PriorityQueue<T>::~PriorityQueue()
{
	while (!isEmpty())
	{
		T x;
		dequeue(x);
	}
}