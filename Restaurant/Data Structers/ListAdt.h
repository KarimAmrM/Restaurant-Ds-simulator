#pragma once
template <class T>
class ListAdt
{
public:
	virtual bool isEmpty() = 0;
	virtual int getLength() = 0;
	virtual bool insert(T item, int pos) = 0;
	virtual bool remove(int pos) = 0 ;
	virtual void clear() = 0;
	virtual T getEntry(int pos) = 0;
	virtual T replace(int pos, T item)=0;


};

