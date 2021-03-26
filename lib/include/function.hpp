/**
 * @file function.hpp
 * Here we implements a simple intefrace to work with different functions
 * @c T type passed in class must implement cast to/from double, and all algebraic operaions
 */
#pragma once

#include <memory>
#include <functional>
#include <cmath>
#include <vector>

template<typename T>
class StdFunction;

/** 
 * Simple interface to operate with functions
 * {@c
 * Function<double>* f = new StdFuntion<double>([](double x) { return x*x; });
 * double x = 0.5;
 * dobule y = (*f)(x); // y = 0.25
 * }
 * @tparam T function's argument and return value type.
 */
template <typename T>
class Function {
public:
    virtual T operator()(T) = 0;

    /**
     * Creates the equivalent of this function of type @c T -> @c T, but with types @c double -> @c double
     * @return new function pointer
     */
    Function<double>* to_double();
};

/**
 * Implementation of {@link Function} to be created from @c std::function class, e.g. lambdas
 * @tparam T the same as in {@link Function} 
 */
template <typename T>
class StdFunction : public Function<T> {
public:
    StdFunction(std::function<T (T)> function) : m_function(function) {}

    T operator()(T x) {
        return m_function(x);
    }
private:
    std::function<T (T)> m_function;
};

/**
 * Small example of how to implement the function interface, in this case function is y = x^2
 * @tparam T the same as in {@link Function} 
 */
template<typename T>
class Parabola : public Function<T> {
public:
    Parabola() { }
    T operator()(T x) { return x*x; };
};

template<typename T>
class Var2Function : public Function<T> {
public:
    Var2Function() { }
    T operator()(T x) { return x*x*x*x - static_cast<T>(1.5*atan(static_cast<double>(x))); }; // :FIXME:
};

/**
 * The wrapper for function, counts the number of calls to provided function
 * @tparam T the same as in {@link Function} 
 */
template<typename T>
class CountingFunction : public Function<T> {
public:
    CountingFunction(Function<T>* function)
        : m_function(function), m_counter(0) {}
    T operator()(T x) {
        m_counter++;
        return (*m_function)(x);
    }

    /**
     * @return the number of times this function was called
     */
    int get_count() { return m_counter; }
private:
    Function<T>* m_function;
    int m_counter;
};

/**
 * Polynomial function. Represents function y = p_0 + p_1*x + p_2*x^2 + ...
 * @tparam T the same as in {@link Function}
 */
template <typename T>
class PolynomFunction : public Function<T> {
public:
    PolynomFunction(std::vector<T> coefs) : m_coefs(coefs) {}

    T operator()(T x) {
        T res = 0;
        for (int i = m_coefs.size() - 1; i >= 0; --i) {
            res *= x;
            res += m_coefs[i];
        }
        return res;
    }

private:
    std::vector<T> m_coefs;
};

template<typename T>
Function<double>* Function<T>::to_double() {
    return new StdFunction<double>([this](double x) {
        return static_cast<double>((*this)(static_cast<T>(x)));
    });
}
