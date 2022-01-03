/*
 * Clean Generics
 *
 * Copyright (C) 2021-2022 bellrise
 */
#include <generics/string.h>
#include <string.h>
#include <malloc.h>

_CG_BEGIN

String::String() : m_size(0), m_len(0), m_val(nullptr) {}

String::String(char_type const* str) : m_val(nullptr)
{
    size_t size;

    size = strlen(str);
    if (!size)
        return;

    _alloc(size);
    m_len = size;

    memcpy(m_val, str, m_len);
    m_val[m_len] = 0;
}

String::String(String const& str)
{
    // The copy constructor also needs to copy the m_val allocation.
    m_len = str.m_len;
    m_size = str.m_size;

    m_val = (char_type *) malloc(m_size);
    memcpy(m_val, str.m_val, m_len);
    m_val[m_len] = 0;
}

String::String(String&& str) noexcept
    : m_size(str.m_size), m_len(str.m_len), m_val(str.m_val)
{
    // Moving the temporary string to this string will stop it from copying,
    // making it faster and more memory efficient because only 1 instance of
    // the string actually will exist.
    str.m_val  = nullptr;
    str.m_len  = 0;
    str.m_size = 0;
}

String::String(int value) : m_val(nullptr)
{
    char_type buf[16];
    snprintf(buf, 16, "%d", value);
    assign(buf);
}

String::String(float value) : m_val(nullptr)
{
    char_type buf[16];
    snprintf(buf, 16, "%f", value);
    assign(buf);
}

String::String(char_type value) : m_val(nullptr)
{
    _alloc(1);
    m_len = 1;
    m_val[0] = value;
}

String::String(size_t value) : m_val(nullptr)
{
    char_type buf[16];
    snprintf(buf, 16, "%zu", value);
    assign(buf);
}

String::~String()
{
    _free();
}

size_t String::len() const
{
    return m_len;
}

String::char_type const* String::get() const
{
    // If the length is 0, we still need to ensure that we pass a null
    // terminated string to libc functions, so this hack returns the address
    // of the m_len variable which will start with a null byte, imitating
    // an empty string.
    if (!m_len)
        return (char_type *) &m_len;

    return m_val;
}

String::char_type String::at(size_t index) const
{
    if (index >= m_len)
        return 0;

    return m_val[index];
}

bool String::equals(String const& other) const
{
    if (this == &other)
        return true;

    if (m_len != other.m_len)
        return false;

    return strncmp(m_val, other.m_val, m_size) == 0;
}

String String::copy() const
{
    String str;

    str.m_len  = m_len;
    str.m_size = m_size;

    // Copy the string contents
    str.m_val = (char_type *) malloc(m_size);
    strncpy(str.m_val, m_val, m_len);

    return str;
}

void String::append(String const& other)
{
    append(other.m_val);
}

void String::append(char_type const* other)
{
    if (!other)
        return;

    append(other, strlen(other));
}

void String::append(char_type const* other, size_t len)
{
    if (!other)
        return;

    // Append a C-style string to this string. If the string cannot fit, it
    // will allocate another CG_STRING_ALLOC_G * n bytes to fit the string.
    if (!m_size)
        _alloc(strlen(other));
    else if (len > m_size - m_len - 1)
        _alloc(m_len + len);

    strncpy(m_val + m_len, other, len);
    m_len += len;
}

void String::assign(String const& other)
{
    _free();

    String str(other);

    // Steal the string
    m_size = str.m_size;
    m_len  = str.m_len;
    m_val  = str.m_val;
    str.m_val  = nullptr;
    str.m_len  = 0;
    str.m_size = 0;
}

void String::assign(char_type const* other)
{
    assign(String(other));
}

void String::assign(String&& str)
{
    // Move the string into the current string.
    _free();

    m_val  = str.m_val;
    m_len  = str.m_len;
    m_size = str.m_size;

    str.m_size = 0;
    str.m_len  = 0;
    str.m_val  = nullptr;
}

bool String::operator==(String const& other) const
{
    return equals(other);
}

String String::operator+(String const& other) const
{
    // Because this may be used outside of the assignment operator, it needs
    // to return a new copy of the string.
    String new_str(m_val);
    new_str.append(other);
    return new_str;
}

void String::operator+=(const String& other)
{
    append(other.m_val);
}

void String::operator=(String const& other)
{
    assign(other);
}

void String::operator=(String&& str)
{
    assign((String&&) str);
}

String::char_type String::operator[](size_t index) const
{
    return at(index);
}

void String::_alloc(size_t bytes)
{
    size_t alloc_size;

    if (!bytes) {
        _free();
        return;
    }

    alloc_size = bytes - (bytes % CG_STRING_ALLOC_G);
    alloc_size += CG_STRING_ALLOC_G;

    m_size = alloc_size;
    m_val = (char_type *) realloc(m_val, m_size);
    m_val[m_size-1] = 0;
}

void String::_free()
{
    m_len  = 0;
    m_size = 0;
    free(m_val);
    m_val  = nullptr;
}

_CG_END
