#pragma once

#include <cstring>
#include <stdexcept>
#include <iterator>
#include <iostream>

template <typename T>
class MyVector
{
private:
    T *data;
    size_t size_;
    size_t capacity_;

    void reserve_internal(size_t newCapacity)
    {
        if (newCapacity <= capacity_)
            return;

        T *newData = new T[newCapacity];
        if (data)
        {
            for (size_t i = 0; i < size_; ++i)
            {
                newData[i] = data[i];
            }
            delete[] data;
        }
        data = newData;
        capacity_ = newCapacity;
    }

public:
    class iterator
    {
    private:
        T *ptr;

    public:
        // Iterator traits
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::random_access_iterator_tag;

        iterator(T *p = nullptr) : ptr(p) {}

        iterator &operator++()
        {
            ++ptr;
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        iterator &operator--()
        {
            --ptr;
            return *this;
        }

        iterator operator--(int)
        {
            iterator tmp = *this;
            --ptr;
            return tmp;
        }

        T &operator*() { return *ptr; }
        T *operator->() { return ptr; }

        bool operator==(const iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const iterator &other) const { return ptr != other.ptr; }
        bool operator<(const iterator &other) const { return ptr < other.ptr; }
        bool operator<=(const iterator &other) const { return ptr <= other.ptr; }
        bool operator>(const iterator &other) const { return ptr > other.ptr; }
        bool operator>=(const iterator &other) const { return ptr >= other.ptr; }

        iterator operator+(ptrdiff_t n) const { return iterator(ptr + n); }
        iterator operator-(ptrdiff_t n) const { return iterator(ptr - n); }

        ptrdiff_t operator-(const iterator &other) const { return ptr - other.ptr; }
    };

    class const_iterator
    {
    private:
        const T *ptr;

    public:
        // Iterator traits
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T *;
        using reference = const T &;
        using iterator_category = std::random_access_iterator_tag;

        const_iterator(const T *p = nullptr) : ptr(p) {}

        const_iterator &operator++()
        {
            ++ptr;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++ptr;
            return tmp;
        }

        const T &operator*() const { return *ptr; }
        const T *operator->() const { return ptr; }

        bool operator==(const const_iterator &other) const { return ptr == other.ptr; }
        bool operator!=(const const_iterator &other) const { return ptr != other.ptr; }
    };

    MyVector() : data(nullptr), size_(0), capacity_(0) {}

    MyVector(size_t initialSize) : size_(initialSize), capacity_(initialSize)
    {
        data = new T[capacity_];
    }

    // Copy constructor
    MyVector(const MyVector &other) : data(nullptr), size_(other.size_), capacity_(other.capacity_)
    {
        if (capacity_ > 0)
        {
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i)
            {
                data[i] = other.data[i];
            }
        }
    }

    // Move constructor
    MyVector(MyVector &&other) noexcept : data(other.data), size_(other.size_), capacity_(other.capacity_)
    {
        other.data = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Assignment operator
    MyVector &operator=(const MyVector &other)
    {
        if (this != &other)
        {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            if (capacity_ > 0)
            {
                data = new T[capacity_];
                for (size_t i = 0; i < size_; ++i)
                {
                    data[i] = other.data[i];
                }
            }
            else
            {
                data = nullptr;
            }
        }
        return *this;
    }

    // Move assignment operator
    MyVector &operator=(MyVector &&other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    ~MyVector()
    {
        delete[] data;
    }

    void push_back(const T &value)
    {
        if (size_ == capacity_)
        {
            size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve_internal(newCapacity);
        }
        data[size_++] = value;
    }

    void pop_back()
    {
        if (size_ > 0)
            --size_;
    }

    T &at(size_t index)
    {
        if (index >= size_)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T &at(size_t index) const
    {
        if (index >= size_)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    T &operator[](size_t index)
    {
        return data[index];
    }

    const T &operator[](size_t index) const
    {
        return data[index];
    }

    T &front() { return data[0]; }
    const T &front() const { return data[0]; }

    T &back() { return data[size_ - 1]; }
    const T &back() const { return data[size_ - 1]; }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    void reserve(size_t newCapacity)
    {
        reserve_internal(newCapacity);
    }

    void resize(size_t newSize)
    {
        if (newSize > capacity_)
            reserve_internal(newSize);
        size_ = newSize;
    }

    void clear()
    {
        size_ = 0;
    }

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size_); }

    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + size_); }

    iterator erase(iterator pos)
    {
        if (pos < begin() || pos >= end())
            return end();

        size_t idx = pos - begin();
        for (size_t i = idx; i < size_ - 1; ++i)
        {
            data[i] = data[i + 1];
        }
        --size_;
        return iterator(data + idx);
    }

    iterator erase(iterator first, iterator last)
    {
        if (first >= last)
            return first;

        size_t startIdx = first - begin();
        size_t count = last - first;

        for (size_t i = startIdx; i < size_ - count; ++i)
        {
            data[i] = data[i + count];
        }
        size_ -= count;
        return iterator(data + startIdx);
    }
};
