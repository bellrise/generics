/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Version file.
 */
#ifndef CG_VERSION_H
#define CG_VERSION_H

#include <stddef.h>

#define CG_MINOR        0
#define CG_MAJOR        1
#define CG_VER          0x000001

#define _CG_BEGIN       namespace cg {
#define _CG_END         };

_CG_BEGIN

/* Forward-declare all classes. */
template<typename R, typename... Args> class Function;
template<typename T> class Array;
class Exception;
class String;
class Object;

/* Forward-declare useful functions. */
void print(String const& str);

_CG_END

#endif /* CG_VERSION_H */
