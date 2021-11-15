/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Core header file.
 */
#ifndef CG_H
#define CG_H

#include <stddef.h>

#define CG_MINOR        0
#define CG_MAJOR        1
#define CG_VER          0x000001

#define _CG_BEGIN       namespace cg {
#define _CG_END         };

_CG_BEGIN

/*
 * Function template class, used for creating function types. For example,
 * to store a simple int add(int a, int b) function, you may use:
 *
 *  Function add_function = add;
 *  int result = add_function(2, 4);
 */
template<typename R, typename... Args>
class Function
{
public:
    typedef R (*type) (Args...);

    /* Used to construct only the type, without any function assigned. */
    Function() : m_func(NULL) {};

    /* Create the function template, but with a pointer so you can call it. */
    Function(type func) : m_func(func) {};

    /* The function call itself. In order to easily call the function object,
       some C++ template magic is used. Basically what happens is the void*
       gets cast into a function pointer using the generated function_t type. */
    R operator()(Args... args)
    {
        if (!m_func)
            return R();
        return m_func(args...);
    }

    /* Assignment operator, if you want to assign a perticular function
       to later call using the operator() method. */
    void operator=(type func) { m_func = func; }

private:

    /* Pointer to the function. */
    type m_func;
};

_CG_END

#endif /* CG_H */
