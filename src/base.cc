/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Generic functions.
 */
#include <generics.h>
#include <generics/string.h>
#include <generics/function.h>
#include <generics/array.h>

_CG_BEGIN

void print(Printable const& val)
{
    printf("%s\n", val.as_string().get());
}

void print(String const& val)
{
    printf("%s\n", val.get());
}

void print(void *val)
{
    printf("%p\n", val);
}

void print(float val)
{
    printf("%f\n", val);
}

void print(char val)
{
    printf("%c\n", val);
}

void print(int val)
{
    printf("%d\n", val);
}

_CG_END
