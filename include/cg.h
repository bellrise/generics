/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Core header file.
 */
#ifndef CG_H
#define CG_H

#include <cg/string.h>
#include <cg/types.h>

_CG_BEGIN

/*
 * Print an object, adding a newline.
 */
void print(Object const& val);
void print(String const& str);
void print(float val);
void print(char val);
void print(int val);

_CG_END

#endif /* CG_H */
