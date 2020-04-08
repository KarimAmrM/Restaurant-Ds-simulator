#pragma once
#include<iostream>
using namespace std;
#include"Node.h"
#include"Bag.h"

template<class T>
class ListBag:public Bag<T>
{
private:
	Node<T>* head;
	int count;
	Node<T>* getPtrTo(T item);
public:
	int getCurrentSize();
	bool isEmpty();
	bool add(const T& it);
	bool remove(const T& item);
	void clear();
	int getFrequencyOf(const T& item);
	bool contains(T item);
	void print();

};

template<class T>
inline Node<T>* ListBag<T>::getPtrTo(T it)
{
	Node<T>* ptr = head;
	while (ptr)
	{
		if (ptr->getItem() == it)
			return ptr;
		ptr = ptr->getnext();
	}
	return NULL;
}

template<class T>
inline int ListBag<T>::getCurrentSize()
{
	return count;
}

template<class T>
inline bool ListBag<T>::isEmpty()
{
	if (count == 0)
		return true;
	return false;
}

template<class T>
inline bool ListBag<T>::add(const T& it)
{
	Node<T>* nptr = new Node<T>;
	nptr->setItem(it);
	nptr->setNext(head);
	head = nptr;
	if (nptr == NULL)
		return false;
	count++;
	return true;
}

template<class T>
inline bool ListBag<T>::remove(const T& item)
{
	Node<T>* ptr = getPtrTo(item);
	if (ptr == NULL)
		return false;
	ptr->setItem(head->getItem());
	Node<T>* temp = head;
	head = head->getnext();
	delete temp;
}

template<class T>
inline void ListBag<T>::clear()
{
	
	while (head)
	{
		Node<T>* ptr = head;
		head = head->getnext();
		delete ptr;
		
	}
	count = 0;
	delete head;
}

template<class T>
inline int ListBag<T>::getFrequencyOf(const T& item)
{
	return 0;
}

template<class T>
inline bool ListBag<T>::contains(T item)
{
	return false;
}

template<class T>
inline void ListBag<T>::print()
{
	Node<T>* ptr = head;
	while (ptr)
	{
		cout << ptr->getItem() << " ";
		ptr = ptr->getnext();
	}
}
