/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Generic functions.
 */
#include <cg.h>

_CG_BEGIN

void print(Object const& val)
{
    printf("%s\n", val.as_string().get());
}

void print(String const& val)
{
    printf("%s\n", val.get());
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
