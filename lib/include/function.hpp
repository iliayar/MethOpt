#pragma once

#include <memory>
#include <functional>

template<typename T>
class StdFunction;

/** 
 * Simple interface to operate with functions
 * @code
 * Function<double>* f = new StdFuntion<double>([](double x) { return x*x; });
 * double x = 0.5;
 * dobule y = (*f)(x); // y = 0.25
 * @endcode
 * @tparam T function's argument and return value type
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
Function<double>* Function<T>::to_double() {
    return new StdFunction<double>([this](double x) {
        return static_cast<double>((*this)(static_cast<T>(x)));
    });
}
