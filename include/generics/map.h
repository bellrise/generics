/*
 * Clean Generics
 *
 * Copyright (C) 2021-2022 bellrise
 *
 * Map object.
 */
#ifndef CG_MAP_H
#define CG_MAP_H

#include <generics/string.h>
#include <generics/array.h>

#define CG_MAP_ALLOC_G      128

_CG_BEGIN

//
// This is a simple hash map, which takes the hash of the key type as the index
// of an internal array with pointers to values. Because it stores an array of
// the indices, it is an ordered map.
//
template<typename K, typename V>
class Map : public Printable
{
public:
    Map() : m_keys(), m_values() {}

    // Return the string representation of the map. This will try to print the
    // elements if possible.
    String as_string() const
    {
        return "asd";
    }

private:

    // The values array is actually built out of nodes, not only the elements.
    struct Node
    {
        V  elem;
        V* next;
    };

    Array<size_t>   m_keys;
    Array<Node>     m_values;
};

_CG_END

#endif /* CG_MAP_H */
