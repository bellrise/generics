/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * String object, based on the C-string.
 */
#ifndef CG_STRING_H
#define CG_STRING_H

#include <cg/version.h>

#define CG_STRING_ALLOC_G       16

_CG_BEGIN

/*
 * The string class is a convenience wrapper around the C-style null-terminated
 * string. It ensures that the string is always end with a 0x00 byte, so it can
 * be passed to libc functions using the .get() method.
 */
class String
{
public:
    String();

    // These constructors will copy the string and put it in this string.
    String(char const* str);
    String(String const& str);

    // Formatting constructors.
    String(char value);
    String(int value);
    String(float value);

    // Free all used resources.
    ~String();

    // Return the pointer to the string for libc functions.
    char const* get() const;

    // Return the length of the string.
    size_t len() const;

    // Returns true if both strings are equal.
    bool equals(String const& other);

    // Returns a new copy of the string.
    String copy() const;

    // Append the other string to this string.
    void append(String const& other);
    void append(char const* other);
    void append(char const* other, size_t len);

    // Assigning a different string to this string will remove the original one,
    // and copy the other one it its place.
    void assign(String const& other);
    void assign(char const* other);

    // Comparison operator, calls equals().
    bool operator==(String const& other);

    // Addition operator, returns a copy of the created string.
    String operator+(String const& other);

    // Append operator, appends other string to the existing string.
    void operator+=(String const& other);

    // Assignment operator.
    void operator=(String const& other);

private:
    size_t  m_size;
    size_t  m_len;
    char*   m_val;

    // Allocate `bytes` bytes round up to the nearest granularity defined by
    // CG_STRING_ALLOC_G. This may cut the string if the requested size is
    // smaller than the existing string.
    void _alloc(size_t bytes);

    // Clear this string. Used by both the deconstructor and assign().
    void _free();
};

_CG_END

#endif /* CG_STRING_H */
