#pragma once

#include "multi_optimizer.hpp"
#include "methods.hpp"

template<typename T, int dimension = 3>
class GradientDescent : public MultiOptimizer<T, dimension>{
public:
    std::pair<Vector<T, dimension>, T> find(QuadFunction<T, dimension>& function) override {
        constexpr T e = 1e-4;
        T alpha = 0.5;
        Vector<T, dimension> x = vector_of<T, dimension>(1);
        T f_x = function(x);
        Vector<T, dimension> grad_x = function.grad(x);
        T norm_grad_x = grad_x.norm();
        grad_x = grad_x * (1.0 / (norm_grad_x));

        while (norm_grad_x >= e) {
            Vector<T, dimension> curr = grad_x * alpha;
            curr = x - curr;
            while (function(curr) >= f_x) {
                alpha /= 2;
                curr = grad_x * alpha;
                curr = x - curr;
            }
            x = curr;
            f_x = function(curr);
            grad_x = function.grad(x);
            norm_grad_x = grad_x.norm();
            grad_x = grad_x * (1 / norm_grad_x);
        }
        return {x, f_x};
    }
};

template<typename T, int dimension = 3, class O = BrentMethod<T>>
class SteepestDescent : public MultiOptimizer<T, dimension> {
public:
    std::pair<Vector<T, dimension>, T> find(QuadFunction<T, dimension>& function) override {
        constexpr T e = 1e-4;
        T alpha;
        Vector<T, dimension> x = vector_of<T, dimension>(1);
        Vector<T, dimension> grad_x = function.grad(x);
        while (grad_x.norm() >= e) {
            Function<T>* func_double = function.to_single(x);
            O method(func_double, 0, 10, this->sigma);
            alpha = get_min(method).first;
            Vector<T, dimension> y = grad_x * alpha;
            x = x - y;
            grad_x = function.grad(x);
        }
        return {x, function(x)};
    }

};

template<typename T, int dimension = 3>
class ConjugateGradient : public MultiOptimizer<T, dimension> {
public:
    std::pair<Vector<T, dimension>, T> find(QuadFunction<T, dimension>& function) override {
        Vector<T, dimension> x = vector_of<T, dimension>(1);
        Vector<T, dimension> grad_x = function.grad(x);
        Vector<T, dimension> p = grad_x * (-1);
        double alpha;

        for (int i = 0; i < dimension; i++) {
            Vector<T, dimension> ap = function.m_A * p; // ??
            alpha = (pow(grad_x.norm(), 2)) / (ap * p);
            Vector<T, dimension> p_alpha = p * alpha;
            x = x + p_alpha;
            Vector<T, dimension> a_p_alpha = ap * alpha;
            Vector<T, dimension> curr_grad = grad_x + a_p_alpha;
            double beta = (pow(curr_grad.norm(), 2)) / (pow(grad_x.norm(), 2));
            grad_x = curr_grad;
            Vector<T, dimension> p_beta = p * beta;
            p = grad_x * (-1) + p_beta;
        }
        return {x, function(x)};
    }
};
