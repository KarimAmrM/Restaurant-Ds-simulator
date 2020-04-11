#pragma once


template<class T>
class PriorNode  //This class is the same ass node but added a prioirty for the implemnetation of the priority queue
{
private:
	T item;
	double priority;
	PriorNode<T>* next;
public:

	PriorNode()
	{
		next = nullptr;
	}

	PriorNode(T newItem)
	{
		item = newItem;
		next = nullptr;
	}

	void setItem(T newItem)
	{
		item = newItem;
	}

	void setNext(PriorNode<T>* nextNodePtr)
	{
		next = nextNodePtr;
	}

	void setPriority(int p)
	{
		priority = p;
	}

	T getItem() const
	{
		return item;
	}

	PriorNode<T>* getNext() const
	{
		return next;
	}

	double getPriority()
	{
		return priority;
	}
};

