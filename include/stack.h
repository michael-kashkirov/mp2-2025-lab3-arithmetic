#pragma once

#include <stdexcept> 

const size_t STACK_DEFAULT_CAPACITY = 100;

template <typename T>
class TStack
{
private:
    
    T* pData;         
    size_t capacity;   
    int topIndex;      

public:

TStack(size_t cap = STACK_DEFAULT_CAPACITY)
    {
        if (cap == 0)
            throw std::length_error("Stack capacity must be positive.");
        capacity = cap;
        pData = new T[capacity];
        topIndex = -1; 
    }

    ~TStack()
    {
        delete[] pData;
        pData = nullptr;
    }

    bool IsEmpty() const { return topIndex == -1; }
    bool IsFull() const { return (size_t)(topIndex + 1) == capacity; }
    
    size_t GetCapacity() const { return capacity; }
    size_t GetCount() const { return topIndex + 1; }

    void Push(const T& value)
    {
        if (IsFull())
        {
            throw std::out_of_range("Stack overflow: Cannot push element.");
        }
        pData[++topIndex] = value; 
    }

    T Pop()
    {
        if (IsEmpty())
        {
            throw std::out_of_range("Stack underflow: Cannot pop from empty stack.");
        }
        return pData[topIndex--]; 
    }

    const T& Top() const
    {
        if (IsEmpty())
        {
            throw std::out_of_range("Stack underflow: Cannot peek at empty stack.");
        }
        return pData[topIndex]; 
    }
};