/*
 * Clean Generics
 *
 * Copyright (C) 2021-2022 bellrise
 *
 * Array object.
 */
#ifndef CG_ARRAY_H
#define CG_ARRAY_H

#include <generics/string.h>

#include <malloc.h>
#include <string.h>

#define CG_ARRAY_ALLOC_G    16

_CG_BEGIN

//
// This array is a dynamic collection of elements, which you can append & remove
// elements, sort, map, filter & reduce them. All the code needs to be in the
// header, because of the template.
//
template<typename T>
class Array : public Printable
{
public:
    typedef Function<T, T> map_function;
    typedef Function<bool, T&> filter_function;
    typedef Function<T, T&, T&> reduce_function;
    typedef Function<String, T&> format_function;
    typedef Function<T, size_t> producer_function;

    Array() : m_size(0), m_len(0), m_array(nullptr) {}

    // Copy the array and create a new one. Note that this will only copy the
    // type/object fields, without deep-copying if <T> is an array for example.
    Array(Array const& other)
    {
        _alloc(other.m_len);
        _copy_array<T>(other.m_array, m_array, other.m_len);

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

    // Support for moving a value into the array. The move-assignment operator
    // is called casting the element to T&&. This is basically what std::move
    // does.
    void append(T&& elem)
    {
        if (m_len >= m_size)
            _alloc(m_size + 1);

        m_array[m_len++] = (T&&) elem;
    }

    // Extend the array with another.
    void append(Array const& other)
    {
        for (size_t i = 0; i < other.m_len; i++)
            append(other.m_array[i]);
    }

    // Get an element at the index.
    T& get(size_t index)
    {
        if (index >= m_len)
            throw "Index is out of bounds";

        return m_array[index];
    }

    // Remove all elements from the array.
    void clear()
    {
        delete[] m_array;

        m_size  = 0;
        m_len   = 0;
        m_array = nullptr;
    }

    // Return a copy of the array.
    Array<T> copy() const
    {
        Array<T> new_array;
        new_array.produce(m_len, [this] (size_t i) {
            return m_array[i];
        });

        return new_array;
    }

    // Set the size of the array. If the current size is larger then the amount
    // of slots requested, this function will fail. This can help is optimizing
    // the array, because the amount of slots requested is allocated once, so
    // no copying and reallocation needs to happen.
    void set_size(size_t slots) const
    {
        if (m_size >= slots)
            return;

        _alloc(slots);
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

    // To make an Array printable, the default as_string() method is overriden
    // with a call to the Array as_string method with a custom formatter, which
    // will turn each element into a string.
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
    // function will pass the already collected value and the current value. If
    // you just want to add all the elements up, see Array<T>::sum().
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

    // Produce elements and add them to the array. This means the producer will
    // be called `amount` times, with the index of the element as the first and
    // only argument. The index will always start from 0.
    template<typename ProduceFunction>
    void produce(size_t amount, ProduceFunction&& producer)
    {
        for (size_t i = 0; i < amount; i++)
            append(producer(i));
    }

    // Sum all the objects in the array, using a simple add lamba in reduce().
    // Note that if T does not provide a operator+ which returns a copy of
    // itself, this method will not compile.
    T sum() const
    {
        return reduce([] (auto& previous, auto& val) {
            return previous + val;
        });
    }

    // Range-based for loop support. C++ requires the begin() and end() methods
    // for an iterator to work.
    //
    //  for (auto i : list)
    //
    // The line above is just syntactic sugar for this:
    //  for (auto it = integers.begin(), end = integers.end(); it != end; ++it)
    //
    // You may also iterate over the array using a simple for loop, with size_t
    // as the counter.
    T* begin() const { return &m_array[0]; }
    T* end() const { return &m_array[m_len]; }

    // Get an element at the given index.
    T& operator[](size_t index)
    {
        return get(index);
    }

    // Append an element.
    void operator+=(T const& elem)
    {
        append(elem);
    }

    // Extend this array with another.
    void operator+=(Array const& other)
    {
        append(other);
    }

    // Assign-copy constructor.
    void operator=(Array<T> const& other)
    {
        clear();

        Array<T> copied = other.copy();
        operator=((Array<T>&&) copied);
    }

    // See Array(Array&& other) move constructor
    void operator=(Array&& other)
    {
        m_array = other.m_array;
        m_size  = other.m_size;
        m_len   = other.m_len;

        other.m_array = nullptr;
        other.m_size  = 0;
        other.m_len   = 0;
    }

protected:

    // Allocate enough slots for the elements, rounding up to the closest value
    // defined by CG_ARRAY_ALLOC_G.
    void _alloc(size_t slots)
    {
        if (!slots)
            return;

        size_t alloc_size;
        T* old_array;

        alloc_size = slots - (slots % CG_ARRAY_ALLOC_G);
        alloc_size += CG_ARRAY_ALLOC_G;

        m_size    = alloc_size;
        old_array = m_array;
        m_array   = new T[alloc_size];

        if (old_array) {
            _copy_array<T>(old_array, m_array, m_len);
            delete[] old_array;
        }
    }

    // In order to support different types, the _copy_array method needs to
    // support a simple array copy for basic C types, and more sophisticated
    // .copy() routines for types like a String.
    template<typename E>
    void _copy_array(E* from, E* to, size_t elems)
    {
        memcpy((void*) to, (void*) from, sizeof(E) * elems);
    }

    template<>
    void _copy_array(String* from, String* to, size_t elems)
    {
        for (size_t i = 0; i < elems; i++)
            to[i] = from[i].copy();
    }


    size_t  m_size;
    size_t  m_len;
    T*      m_array;
};

// If you want to copy a two or three-dimensional array, the standard .copy()
// method will unfortunately fail. That is why deep_copy needs to be used for
// such arrays. Otherwise, it will result in a memcpy-param-overlap, meaning
// undefined behaviour.
template<typename T>
Array<Array<T>> deep_copy(Array<Array<T>> const& source)
{
    Array<Array<T>> copy;
    for (auto const& sublist : source)
        copy.append(sublist);

    return copy;
}

// See Array<Array<T>> deep_copy(Array<Array<T>> const&)
template<typename T>
Array<Array<Array<T>>> deep_copy(Array<Array<Array<T>>> const& source)
{
    Array<Array<Array<T>>> copy;
    for (auto const& subarray : source)
        copy.append((Array<Array<T>>&&) deep_copy(subarray));

    return copy;
}

_CG_END

#endif /* CG_ARRAY_H */
