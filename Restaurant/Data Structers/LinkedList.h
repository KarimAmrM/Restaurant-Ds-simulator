#pragma once
#include<iostream>
using namespace std;
#include"..\Generic_DS\Node.h"
#include"ListAdt.h"
template <class T>
class LinkedList:ListAdt<T>
{
	Node<T>* head;
	int count;
	Node<T>* getNodeAt(int pos);
public:
	LinkedList();
	bool isEmpty();
	int getLength();
	bool insert(T item, int pos);
	bool remove(int pos);
	void clear();
	T getEntry(int pos);
	T replace(int pos, T item);
	void print();

};

template<class T>
inline Node<T>* LinkedList<T>::getNodeAt(int pos)  //utulity function to get a node at a given position , complexity of O(n)
{
	if (pos >= count || pos<0)
	{
		cout << "Inavalid position" << endl;
		return nullptr;
	}
	if (count == 0)
	{
		cout << "List is empty" << endl;
		return nullptr;
	}

	Node<T>* ptr = head;
	for (int i = 0; i < count; i++)
	{
		if (i == pos)
		{
			return ptr;
		}
		ptr = ptr->getNext();
	}
}

template<class T>
inline LinkedList<T>::LinkedList()
{
	count = 0;
	head = nullptr;
}

template<class T>
inline bool LinkedList<T>::isEmpty()
{
	return (count == 0);
}

template<class T>
inline int LinkedList<T>::getLength()
{
	return count;
}

template<class T>
inline bool LinkedList<T>::insert(T item, int pos) //complexity of O(n)
{
	//special cases to be done 
	if (count == 0)
	{
		Node<T>* ptr = new Node<T>;
		ptr->setItem(item);
		head = ptr;
		ptr->setNext(nullptr);
		count++;
		return true;
	}
	if (pos >count)
	{
		cout << "invalid position" << endl;
		return false;
	}

	if (pos == 0)
	{
		Node<T>* ptr = new Node<T>;
		ptr->setItem(item);
		ptr->setNext(head);
		head = ptr;
		count++;
		return true;

	}

	Node<T>* nptr = new Node<T>;
	nptr->setItem(item);
	Node<T>* ptr = getNodeAt(pos - 1);
	nptr->setNext(ptr->getNext());
	ptr->setNext(nptr);
	count++;
	return true;
}

template<class T>
inline bool LinkedList<T>::remove(int pos) //O(n)
{
	if (pos == 0)
	{
		Node<T>* temp = head;
		head = head->getNext();
		delete temp;
		count--;
		return true;
	}
	if (pos >= count && count != 0)
	{
		cout << "invalid position" << endl;
		return false;
	}
	if (count == 0)
	{
		cout << "List is empty" << endl;
		return false;
	}
	Node<T>* temp = getNodeAt(pos);
	Node<T>* ptr = getNodeAt(pos - 1);
	ptr->setNext(temp->getNext());
	delete temp;
	count--;
	return true;

}

template<class T>
inline void LinkedList<T>::clear()
{
	
	while (head)
	{
		Node<T>* ptr = head;
		head = head->getNext();
		delete ptr;
	}
	count = 0;
}

template<class T>
inline T LinkedList<T>::getEntry(int pos)
{
	if(getNodeAt(pos)!=nullptr)
	return getNodeAt(pos)->getItem();
}

template<class T>
inline T LinkedList<T>::replace(int pos, T item)
{
	return T();
}

template<class T>
inline void LinkedList<T>::print()
{
	Node<T>* ptr = head;
	if (count == 0)
	{
		cout << "List Is Empty , Try Inserting Some Stuff ;) ";
	}
	for (int i = 0; i < count; i++)
	{
		cout << ptr->getItem()<<" ";
		ptr = ptr->getNext();
	}
	cout << endl;
}
