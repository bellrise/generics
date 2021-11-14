/*
 * Generics
 *
 * Copyright (C) 2021 bellrise
 */
#include <cg/string.h>
#include <string.h>
#include <malloc.h>

_CG_BEGIN

String::String() : m_size(0), m_len(0), m_val(NULL) {}

String::String(char const *str) : m_val(NULL)
{
    size_t size;

    size = strlen(str);
    if (!size)
        return;

    alloc(size);
    m_len = size;

    memcpy(m_val, str, m_len);
    m_val[m_len] = 0;
}

String::String(String const &str)
{
    /* The copy constructor also needs to copy the m_val allocation. */
    m_len = str.m_len;
    m_size = str.m_size;

    m_val = (char *) malloc(m_size);
    memcpy(m_val, str.m_val, m_len);
    m_val[m_len] = 0;
}

String::~String()
{
    free(m_val);
}

size_t String::len() const
{
    return m_len;
}

char const* String::get() const
{
    /* If the length is 0, we still need to ensure that we pass a null
       terminated string to libc functions, so this hack returns the address
       of the m_len variable which will start with a null byte, imitating
       an empty string. */
    if (!m_len)
        return (char *) &m_len;

    return m_val;
}

void String::alloc(size_t bytes)
{
    size_t alloc_size;

    alloc_size = bytes - (bytes % CG_STRING_ALLOC_G);
    alloc_size += CG_STRING_ALLOC_G;

    printf("alloc %zu (%zu)\n", bytes, alloc_size);

    m_size = alloc_size;
    m_val = (char *) realloc(m_val, m_size);
    m_val[m_size-1] = 0;
}

bool String::equals(String const &other)
{
    if (m_len != other.m_len)
        return false;

    return strncmp(m_val, other.m_val, m_size) == 0;
}

void String::append(String const &other)
{
    append(other.m_val);
}

void String::append(char const *other)
{
    append(other, strlen(other));
}

void String::append(char const *other, size_t len)
{
    if (len > m_size - m_len - 1)
        alloc(m_len + len);

    strncpy(m_val + m_len, other, len);
    m_len += len;
}

/* Convenience operator overloads. */

bool String::operator==(String const &other)
{
    return equals(other);
}

String String::operator+(String const &other)
{
    String new_str(m_val);
    new_str.append(other);
    return new_str;
}

void String::operator+=(const String &other)
{
    append(other.m_val);
}

_CG_END
