/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * String object, based on the C-string.
 */
#ifndef CG_STRING_H
#define CG_STRING_H

#include <cg.h>

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
    String(char const *str);
    String(String const &str);

    ~String();

    /* Return the pointer to the string for libc functions. */
    char const* get() const;

    /* Return the length of the string. */
    size_t len() const;

    /* Returns true if both strings are equal. */
    bool equals(String const &other);

    /* Append the other string to this string. */
    void append(String const &other);
    void append(char const *other);
    void append(char const *other, size_t len);

    /* Conveniece operator overloads. */
    bool operator==(String const &other);
    String operator+(String const &other);
    void operator+=(String const &other);

private:
    size_t  m_size;
    size_t  m_len;
    char    *m_val;

    /* Allocate `bytes` bytes round up to the nearest granularity defined by
       CG_STRING_ALLOC_G. This may cut the string if the requested size is
       smaller than the existing string. */
    void alloc(size_t bytes);
};

_CG_END

#endif /* CG_STRING_H */
