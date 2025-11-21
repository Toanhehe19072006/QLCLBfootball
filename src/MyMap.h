#pragma once

#include <stdexcept>
#include <cstring>
#include <string>
#include "MyVector.h"

template <typename Key, typename Value>
class MyMap
{
public:
    struct Pair
    {
        Key key;
        Value value;
        bool used;

        Pair() : used(false) {}
        Pair(const Key &k, const Value &v) : key(k), value(v), used(true) {}
    };

    class iterator
    {
    private:
        MyVector<Pair> *buckets_ptr;
        size_t idx;

        void skipUnused()
        {
            while (idx < buckets_ptr->size() && !(*buckets_ptr)[idx].used)
            {
                ++idx;
            }
        }

    public:
        iterator(MyVector<Pair> *bp, size_t i) : buckets_ptr(bp), idx(i) { skipUnused(); }

        iterator &operator++()
        {
            ++idx;
            skipUnused();
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++idx;
            skipUnused();
            return tmp;
        }

        Pair &operator*() { return (*buckets_ptr)[idx]; }
        Pair *operator->() { return &(*buckets_ptr)[idx]; }

        bool operator==(const iterator &other) const { return idx == other.idx && buckets_ptr == other.buckets_ptr; }
        bool operator!=(const iterator &other) const { return !(*this == other); }
    };

private:
    struct InternalPair
    {
        Key key;
        Value value;
        bool used;

        InternalPair() : used(false) {}
        InternalPair(const Key &k, const Value &v) : key(k), value(v), used(true) {}
    };

    MyVector<InternalPair> buckets;
    size_t count_;

    size_t hash(const Key &key) const
    {
        // For string types, use a proper hash function
        size_t h = 0;
        const char *str = key.c_str();
        while (*str)
        {
            h = h * 31 + *str++;
        }
        return h % buckets.size();
    }

    size_t findBucket(const Key &key) const
    {
        if (buckets.empty())
            return 0;

        size_t idx = hash(key);
        size_t start = idx;

        while (buckets[idx].used && buckets[idx].key != key)
        {
            idx = (idx + 1) % buckets.size();
            if (idx == start)
                return buckets.size(); // Not found, wrapped around
        }
        return idx;
    }

    void rehash()
    {
        MyVector<InternalPair> oldBuckets = buckets;
        buckets.clear();
        buckets.resize(oldBuckets.size() * 2);
        for (size_t i = 0; i < buckets.size(); ++i)
        {
            buckets[i] = InternalPair();
        }

        for (const auto &pair : oldBuckets)
        {
            if (pair.used)
            {
                size_t idx = hash(pair.key);
                while (buckets[idx].used)
                {
                    idx = (idx + 1) % buckets.size();
                }
                buckets[idx] = pair;
            }
        }
    }

public:
    MyMap() : count_(0)
    {
        buckets.resize(16);
        for (size_t i = 0; i < 16; ++i)
        {
            buckets[i] = InternalPair();
        }
    }

    ~MyMap() {}

    void insert(const Key &key, const Value &value)
    {
        if (buckets.empty())
        {
            buckets.resize(16);
            for (size_t i = 0; i < 16; ++i)
            {
                buckets[i] = InternalPair();
            }
        }

        // Check load factor (if > 0.75, rehash)
        if (count_ >= buckets.size() * 3 / 4)
        {
            rehash();
        }

        size_t idx = findBucket(key);
        if (idx >= buckets.size())
        {
            throw std::runtime_error("Map is full after rehashing");
        }

        if (!buckets[idx].used)
        {
            buckets[idx] = InternalPair(key, value);
            count_++;
        }
        else if (buckets[idx].key == key)
        {
            buckets[idx].value = value; // Update existing
        }
    }

    bool find(const Key &key) const
    {
        if (buckets.empty())
            return false;

        size_t idx = findBucket(key);
        return idx < buckets.size() && buckets[idx].used;
    }

    Value &at(const Key &key)
    {
        if (buckets.empty())
            throw std::out_of_range("Key not found in empty map");

        size_t idx = findBucket(key);
        if (idx >= buckets.size() || !buckets[idx].used)
        {
            throw std::out_of_range("Key not found");
        }
        return buckets[idx].value;
    }

    const Value &at(const Key &key) const
    {
        if (buckets.empty())
            throw std::out_of_range("Key not found in empty map");

        size_t idx = findBucket(key);
        if (idx >= buckets.size() || !buckets[idx].used)
        {
            throw std::out_of_range("Key not found");
        }
        return buckets[idx].value;
    }

    Value &operator[](const Key &key)
    {
        if (buckets.empty())
        {
            buckets.resize(16);
            for (size_t i = 0; i < 16; ++i)
            {
                buckets[i] = InternalPair();
            }
        }

        size_t idx = findBucket(key);
        if (idx >= buckets.size() || !buckets[idx].used)
        {
            insert(key, Value());
            idx = findBucket(key);
        }
        return buckets[idx].value;
    }

    bool erase(const Key &key)
    {
        if (buckets.empty())
            return false;

        size_t idx = findBucket(key);
        if (idx >= buckets.size() || !buckets[idx].used)
        {
            return false;
        }

        buckets[idx].used = false;
        count_--;
        return true;
    }

    size_t size() const { return count_; }
    bool empty() const { return count_ == 0; }

    void clear()
    {
        for (size_t i = 0; i < buckets.size(); ++i)
        {
            buckets[i].used = false;
        }
        count_ = 0;
    }
};
