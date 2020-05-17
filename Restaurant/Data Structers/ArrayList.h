#pragma once
#include<iostream>
using namespace std;
#include"ListAdt.h"
template <class T>
class ArrayList:public ListAdt<T>
{
private :
	int max;
	T* arr;
	int count;
public:
	ArrayList(int size);
	bool isEmpty();
	int getLength();
	bool insert( T item ,int pos );
	 bool remove(int pos) ;
	 void clear();
	 T getEntry(int pos) ;
	 T replace(int pos, T item) ;
	 void print();
};

template<class T>
inline ArrayList<T>::ArrayList(int size)
{
	max = size;
	arr = new T[max];
	count = 0;
}

template<class T>
inline bool ArrayList<T>::isEmpty()
{
	if (count == 0)
		return true;
	return false;
}

template<class T>
inline int ArrayList<T>::getLength()
{
	return count;
}

template<class T>
inline bool ArrayList<T>::insert(T item, int pos)
{
	if (count == max || pos>count || pos<0)
		return false;


	for (int i = count; i >= pos; i--)
	{
		arr[i+1] = arr[i];

	}
	arr[pos] = item;
	count++;
	return true;
}

template<class T>
inline bool ArrayList<T>::remove(int pos)
{
	//special cases to be done after : done
	if (count == 0 || pos < 0 || pos >= count)
		return  false;

	for (int i = 0; i < count; i++)
	{
		if (i >= pos)
		{
			arr[i] = arr[i + 1];
		}	
	}
	count--;
	return true;
}

template<class T>
inline void ArrayList<T>::clear()
{
	count = 0;
}

template<class T>
inline T ArrayList<T>::getEntry(int pos)
{
	if (count == 0)
	{
		T val = -1;
		cout << "Lits ie empty" << endl;
		return val;
	}

	if (pos >= count)
	{
		T val = -1;
		cout << "Invalid postion" << endl;
		return val;
	}

	return arr[pos];
}

template<class T>
inline T ArrayList<T>::replace(int pos, T item)
{
	if (count == 0)
	{
		T val = -1;
		cout << "Lits ie empty" << endl;
		return val;
	}

	if (pos >= count)
	{
		cout << "Invalid postion" << endl;
		T val = -1;
		return val;
	}
	T temp = arr[pos];
	arr[pos] = item;
	return temp;
}

template<class T>
inline void ArrayList<T>::print()
{
	if (count == 0)
	{
		cout << "List Is Empty , Try Inserting Some Stuff ;) " ;
	}
	for (int i = 0; i < count; i++)
	{
		cout << arr[i] << " " ;
	}
	cout << endl;
}
