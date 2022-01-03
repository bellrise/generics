/*
 * Clean Generics
 *
 * Copyright (C) 2021-2022 bellrise
 *
 * Core header file.
 */
#ifndef CG_H
#define CG_H

#include <stddef.h>

#define CG_MINOR        0
#define CG_MAJOR        1
#define CG_VER          0x000001

#define _CG_BEGIN       namespace generic {
#define _CG_END         };

_CG_BEGIN

template<typename R, typename... Args> class Function;
template<typename K, typename V> class Map;
template<typename T> class Array;
class Printable;
class String;

//
// All generic objects have the _Printable trait, which allows them to be passed
// to the print() function.
//
class Printable
{
public:
    virtual String as_string() const=0;
};

//
// Print a string representation to standard out, with a newline at the end.
//
void print(Printable const& val);
void print(String const& str);
void print(void *val);
void print(float val);
void print(char val);
void print(int val);

_CG_END

#endif /* CG_H */
