#ifndef BAG
#define BAG
template<class T>
class Bag
{
public:
	virtual int getCurrentSize() = 0;
	virtual bool isEmpty() = 0;
	virtual bool add(const T&  item) = 0;
	virtual bool remove( const T& item) = 0;
	virtual void clear() = 0;
	virtual int getFrequencyOf( const T& item) = 0;
	virtual bool contains(T item) = 0;

};
#endif
