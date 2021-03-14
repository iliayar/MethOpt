/**
 * @file optimizer.hpp
 * Here we implement the interface if minizing methods.
 * Also there is a classes for representing information on each iteration.
 * @c T must respect requirements from {@link function.hpp}
 */
#pragma once

#include <vector>
#include <memory>

#include "function.hpp"

#define EPS static_cast<T>(1e-5)

/**
 * Enumeration of avaliable types, which can be fetched on each step from {@link Optimizer}.
 */
enum class DataType {
    Interval, /**< Interval on Ox */
    Point, /**< Just a point */
    Function /**< Approximation function */
};

/**
 * Interface of data, which can be fetched from {@link Optimizer} on each interation
 * @tparam T the type of data's values
 */
template<typename T>
class IterationData {
public:

    /**
     * Detect the type of data
     * @return {@see DataType}
     */
    virtual DataType get_type() = 0; 
};

/**
 * The interval on Ox axis. Represented by left and right values.
 * @tparam T {@see IterationData}
 */
template <typename T>
class IterationInterval : public IterationData<T> {
public:
    IterationInterval(T left_x, T right_x)
        : m_left_x(left_x), m_right_x(right_x) {}
    DataType get_type() { return DataType::Interval; }

    /**
     * @return left x coordinate of interval
     */
    T get_left_x() { return m_left_x; }

    /**
     * @return right x cooridnate of interval
     */ 
    T get_right_x() { return m_right_x; }

private:
    T m_left_x;
    T m_right_x;
};

/**
 * Just a point. Represented by x and y coordinate
 * @tparam T {@see IterationData}
 */
template <typename T>
class IterationPoint : public IterationData<T> {
public:
    IterationPoint(T x, T y) : m_x(x), m_y(y) {}
    DataType get_type() { return DataType::Point; }

    /**
     * @return x coordinate of point
     */
    T get_x() { return m_x; }

    /**
     * @return y coordinate of point
     */
    T get_y() { return m_y; }
private:
    T m_x;
    T m_y;
};

/**
 * Function used in {@link Optimizer} to find minimum. Represented by {@link Function} with {@c T = double}
 * @tparam {@see IterationData}
 */
template<typename T>
class IterationFunction : public IterationData<T> {
public:
    IterationFunction(Function<T>* function) : m_function(function) {} 
    DataType get_type() {return DataType::Function;}

    /**
     * @return approximation function pointer
     */
    Function<T>* get_function() { return m_function; }
private:
    Function<T>* m_function;
};

/**
 * Interface of optimization method.
 * @tparam T type of function, iteration data and resulting minimum value, used in all calculations
 */
template<typename T>
class Optimizer {
public:
    Optimizer(Function<T>* function) : m_function(new CountingFunction<T>(function)) {}

    /**
     * Makes a single iteration step.
     * @return @c false if optimization process ended, e.g. required accuracy reached. @c true otherwise
     */
    virtual bool forward() = 0;

    /**
     * Fetch data of currently passed iteration. So must be called after {@link Optimizer::forward}
     * Size of data always the same for each iteration.
     * @return @c std::vector of {@link IterationData} pointer.
     */
    virtual std::vector<IterationData<T>*> get_data() = 0;

    /**
     * Must be called after getting @c false value from {@link Optimizer::forward}
     * @return calculated minimum point
     */
    virtual std::pair<T, T> get_min() = 0;

    int get_call_count() { return m_function->get_count(); }
protected:
    CountingFunction<T>* m_function;
};
