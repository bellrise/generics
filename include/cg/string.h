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

    // Move constructor, for an r-value reference.
    String(String&& str) noexcept;

    // Formatting constructors.
    String(char value);
    String(int value);
    String(float value);
    String(size_t value);

    // Free all used resources.
    ~String();

    // Return the pointer to the string for libc functions.
    char const* get() const;

    // Get a character at the given index. Returns 0 is out of bounds.
    char at(size_t index) const;

    // Return the length of the string.
    size_t len() const;

    // Returns true if both strings are equal.
    bool equals(String const& other) const;

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
    bool operator==(String const& other) const;

    // Addition operator, returns a copy of the created string.
    String operator+(String const& other) const;

    // Append operator, appends other string to the existing string.
    void operator+=(String const& other);

    // Assignment operator.
    void operator=(String const& other);

    // Move assignment operator, so you can move temporary strings instead of
    // copying them. This is seen in the Array<T>::append(T&&) method, which
    // will use the provided move operator instead of copying the value.
    void operator=(String&& str);

    // Get a character at the given index. Returns 0 is out of bounds.
    char operator[](size_t index) const;

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
