/*
 * Clean Generics
 *
 * Copyright (C) 2021 bellrise
 *
 * Type utilities.
 */
#ifndef CG_TYPES_H
#define CG_TYPES_H

#include <cg/string.h>

#include <stdio.h>

_CG_BEGIN

/*
 * Base class. All generic objects have this as their base class, apart from the
 * String class which this class relies on. Because this is a public class, you
 * may inherit from this class in order to use the print() function.
 */
class Object
{
public:
    virtual String as_string() const
    {
        char buf[32];
        snprintf(buf, 32, "<generic %p>", (void *) this);
        return String(buf);
    }
};

/*
 * Function template class, used for creating function types. For example,
 * to store a simple int add(int a, int b) function, you may use:
 *
 *  Function add_function = add;
 *  int result = add_function(2, 4);
 */
template<typename R, typename... Args>
class Function : public Object
{
public:
    typedef R (*type) (Args...);

    // Used to construct only the type, without any function assigned.
    Function() : m_func(nullptr) {};

    // Create the function template, but with a pointer so you can call it.
    Function(type func) : m_func(func) {};

    // The function call itself. In order to easily call the function object,
    // some C++ template magic is used. Basically what happens is the void*
    // gets cast into a function pointer using the generated function_t type.
    R operator()(Args... args)
    {
        if (!m_func)
            return R();
        return m_func(args...);
    }

    // Assignment operator, if you want to assign a perticular function
    // to later call using the operator() method.
    void operator=(type func) { m_func = func; }

    String as_string() const override
    {
        char buf[32];
        snprintf(buf, 32, "<function %p>", (void *) m_func);
        return String(buf);
    }

private:

    // Pointer to the function.
    type m_func;
};

_CG_END

#endif /* CG_TYPES_H */
