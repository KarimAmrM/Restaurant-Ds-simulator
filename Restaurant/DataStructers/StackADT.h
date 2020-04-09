#pragma once
template<class T>
class StackADT
{
public:
    virtual bool isEmpty() = 0;
    virtual bool push(T item) = 0;
    virtual bool pop(T& item) = 0;
    virtual T peek() = 0;
};

