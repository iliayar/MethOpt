#pragma once

#include <memory>
#include <functional>

template<typename T>
class StdFunction;

template <typename T>
class Function {
public:
    virtual T operator()(T) = 0;
    Function<double>* to_double();
};

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
