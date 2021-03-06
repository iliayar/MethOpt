#pragma once

#include <vector>
#include <memory>

#include "function.hpp"

#define EPS static_cast<T>(1e-5)

enum class DataType {
    Interval,
    Point,
    Function
};

template<typename T>
class IterationData {
public:
    virtual DataType get_type() = 0; 
};

template <typename T>
class IterationInterval : public IterationData<T> {
public:
    IterationInterval(T left_x, T right_x)
        : m_left_x(left_x), m_right_x(right_x) {}
    DataType get_type() { return DataType::Interval; }
    T get_left_x() { return m_left_x; }
    T get_right_x() { return m_right_x; }

private:
    T m_left_x;
    T m_right_x;
};

template <typename T>
class IterationPoint : public IterationData<T> {
public:
    IterationPoint(T x, T y) : m_x(x), m_y(y) {}
    DataType get_type() { return DataType::Point; }
    T get_x() { return m_x; }
    T get_y() { return m_y; }
private:
    T m_x;
    T m_y;
};

template<typename T>
class IterationFunction : public IterationData<T> {
public:
    IterationFunction(Function<T>* function) : m_function(function) {} 
    DataType get_type() {return DataType::Function;}
    Function<T>* get_function() { return m_function; }
private:
    Function<T>* m_function;
};

template<typename T>
class Optimizer {
public:
    Optimizer(Function<T>* function) : m_function(function) {}
    virtual bool forward() = 0;
    virtual std::vector<IterationData<T>*> get_data() = 0;
    virtual std::pair<T, T> get_min() = 0;

    Function<T>* get_function() { return m_function; }
protected:
    Function<T>* m_function;
};
