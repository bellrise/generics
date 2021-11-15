/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Array object.
 */
#ifndef CG_ARRAY_H
#define CG_ARRAY_H

#include <cg/string.h>
#include <cg/types.h>

#include <malloc.h>
#include <string.h>

#define CG_ARRAY_ALLOC_G    16

_CG_BEGIN

/*
 * This array is a dynamic collection of elements, which you can append & remove
 * elements, sort, map, filter & reduce them.
 */
template<typename T>
class Array : public Object
{
public:
    typedef Function<T, T> map_function;
    typedef Function<bool, T&> filter_function;
    typedef Function<T, T&, T&> reduce_function;
    typedef Function<String, T&> format_function;

    Array() : m_size(0), m_len(0), m_array(nullptr) {}

    // Copy the array and create a new one. Note that this will only copy the
    // type/object fields, without deep-copying if <T> is an array for example.
    Array(Array const& other)
    {
        _alloc(other.m_len);
        for (size_t i = 0; i < other.m_len; i++)
            m_array[i] = other.m_array[i];

        m_len = other.m_len;
    }

    // Delete the array.
    ~Array()
    {
        clear();
    }

    // Return the size of the array.
    size_t size() const
    {
        return m_size;
    }

    // Append an element to the array. The element will get copied.
    void append(T const& elem)
    {
        if (m_len >= m_size)
            _alloc(m_size + 1);

        m_array[m_len++] = elem;
    }

    // Remove all elements from the array.
    void clear()
    {
        delete[] m_array;

        m_size  = 0;
        m_len   = 0;
        m_array = nullptr;
    }

    // Add each element to the string using the given format function.
    template<typename FormatFunction>
    String as_string(FormatFunction&& formatter) const
    {
        if (!m_len)
            return String("[]");

        String result = "[";

        for (size_t i = 0; i < m_len - 1; i++)
            result += formatter(m_array[i]) + ", ";
        result += formatter(m_array[m_len-1]);

        return result + "]";
    }

    // Formatter from Genric
    String as_string() const override
    {
        return as_string([] (auto& val) {
            return String(val);
        });
    }

    // Apply the mapper function to each element in the array. The return value
    // from the function will be assigned to the given slot.
    template<typename MapFunction>
    void map(MapFunction&& mapper)
    {
        for (size_t i = 0; i < m_len; i++)
            m_array[i] = mapper(m_array[i]);
    }

    // Filter the array and return a copy of it with elements that have returned
    // true from the filter function.
    template<typename FilterFunction>
    Array<T> filter(FilterFunction&& filter) const
    {
        Array<T> new_array;

        for (size_t i = 0; i < m_len; i++) {
            if (filter(m_array[i]))
                new_array.append(m_array[i]);
        }

        return new_array;
    }

    // Reduce the array into a single value of the type. Each call of the reduce
    // function will pass the already collected value and the current value.
    template<typename ReduceFunction>
    T reduce(ReduceFunction&& reducer) const
    {
        if (!m_len)
            return T();

        T result = m_array[0];
        for (size_t i = 1; i < m_len; i++)
            result = reducer(result, m_array[i]);

        return result;
    }

    // Sum all the objects in the array, using a simple add lamba in reduce().
    T sum() const
    {
        return reduce([] (auto& previous, auto& val) {
            return previous + val;
        });
    }

private:

    // Allocate enough slots for the elements, rounding up to the closest value
    // defined by CG_ARRAY_ALLOC_G.
    void _alloc(size_t slots)
    {
        if (!slots) {
            return;
        }

        size_t alloc_size;
        T* old_array;

        alloc_size = slots - (slots % CG_ARRAY_ALLOC_G);
        alloc_size += CG_ARRAY_ALLOC_G;

        m_size    = alloc_size;
        old_array = m_array;
        m_array   = new T[alloc_size];

        if (old_array) {
            memcpy(m_array, old_array, sizeof(T) * alloc_size);
            delete[] old_array;
        }
    }

    size_t  m_size;
    size_t  m_len;
    T*      m_array;
};

_CG_END

#endif /* CG_ARRAY_H */
