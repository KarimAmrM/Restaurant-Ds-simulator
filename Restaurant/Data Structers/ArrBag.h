#ifndef ARRBAG
#define ARRBag
#include<iostream>
#include"Bag.h"
#define max 100


using namespace std;


template<class T>
class ArrBag:public Bag<T>
{
private:
	int count;
	T arr[max];
	int getIndexOf(T item);

public:
	ArrBag();
	 int getCurrentSize();
	 bool isEmpty();
	 bool add(const T& item);
	 bool remove(const T& item);
	 void clear();
	 int getFrequencyOf(const T& item);
	 bool contains(T item);
	 void print();
};
#endif

template<class T>
inline int ArrBag<T>::getIndexOf(T item)
{
	for (int i = 0; i < count; i++)
	{
		if (arr[i] == item)
			return i;
	}
	return -1;
}

template<class T>
inline ArrBag<T>::ArrBag()
{
	count = 0;
}

template<class T>
inline int ArrBag<T>::getCurrentSize()
{
	return count;
}

template<class T>
inline bool ArrBag<T>::isEmpty()
{
	if (count == 0)
		return true;
	return false;
}

template<class T>
inline bool ArrBag<T>::add(const T& item)
{
	if (count == max)
		return false;
	arr[count] = item;
	count++;
	return true;
}

template<class T>
inline bool ArrBag<T>::remove(const T& item)
{
	if (count == 0)
		return false;
	int index = getIndexOf(item);
	if (index == -1)
		return false;
	arr[index] = arr[count-1];
	count--;
	return true;
}

template<class T>
inline void ArrBag<T>::clear()
{
	count = 0;
	return;
}

template<class T>
inline int ArrBag<T>::getFrequencyOf(const T& item)
{
	int frequency = 0;
	for (int i = 0; i < count; i++)
	{
		if (arr[i] == item)
			frequency++;
	}
	return frequency;
}

template<class T>
inline bool ArrBag<T>::contains(T item)
{
	for (int i = 0; i < count; i++)
	{
		if (arr[i] == item)
			return true;
	}
	return false;
}

template<class T>
inline void ArrBag<T>::print()
{
	for (int i = 0; i < count; i++)
	{
		cout << arr[i] << " ";
	}
	return;
}
