#pragma once

#include "multi_helpers.hpp"

namespace lab4 {


/**
 * Interface for functions, differeciatable twice
 */
template <typename T>
class multivariate_function {
public:

    /**
     * Calcualte gradient if function in {@code point}
     * @param point
     */
    virtual Vector<T> get_grad(Vector<T> point) const = 0;

    /**
     * Calculate hessian of function in point
     * @param point
     */
    virtual Matrix<T> get_hessian(Vector<T> point) const = 0;

    /**
     * Calculate function in point
     * @param point
     */
    virtual T call(Vector<T> args) const = 0;

    /**
     * Converts multivariable function into the signle variable: f(x) = x0 + p * x
     * @param point The x0 parameter
     * @param p The direction
     */
    Function<T>* to_single(Vector<T> point, Vector<T> p) const {
        return new StdFunction<T>([=](T x) {
            return this->call(point + p * x);
        });
    }

};

/**
 * Implementation of {@link multivariable_function} for quadratic functions
 */
template <typename T>
class quad_multivariate_function : public multivariate_function<T> {
public:

    explicit quad_multivariate_function(QuadFunction<T> function) :function(std::move(function)) {}

    Vector<T> get_grad(Vector<T> vector) const override {
        return function.grad(vector);
    }

    Matrix<T> get_hessian(Vector<T> vector) const override {
        return function.m_A;
    }

    T call(Vector<T> args) const override { return function.call(args); }

private:
    QuadFunction<T> function;
};
}  // namespace lab4
