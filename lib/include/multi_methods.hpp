#pragma once

#include "multi_optimizer.hpp"
#include "methods.hpp"

template<typename T, typename Matrix = Matrix<T>>
class GradientDescent : public MultiOptimizer<T, Matrix>{
public:
    GradientDescent() : MultiOptimizer<T, Matrix>() {}
    GradientDescent(Vector<T> initial, T eps) : MultiOptimizer<T, Matrix>(initial, eps) {}

    std::pair<Vector<T>, T> find(QuadFunction<T, Matrix>& function) override {
        T alpha = 0.5;
        Vector<T> x = this->get_initial(function.arity());
        T f_x = function(x);
        Vector<T> grad_x = function.grad(x);
        T norm_grad_x = grad_x.norm();
        grad_x = grad_x * (1.0 / (norm_grad_x));

        while (norm_grad_x >= this->m_eps) {
            Vector<T> curr = grad_x * alpha;
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
            this->iter(x, function.grad(x));
        }
        return {x, f_x};
    }
};

template<typename T, class O = BrentMethod<T>, typename Matrix = Matrix<T>>
class SteepestDescent : public MultiOptimizer<T, Matrix> {
public:
    SteepestDescent() : MultiOptimizer<T, Matrix>() {}
    SteepestDescent(Vector<T> initial, T eps) : MultiOptimizer<T, Matrix>(initial, eps) {}

    std::pair<Vector<T>, T> find(QuadFunction<T, Matrix>& function) override {
        T alpha;
        Vector<T> x = this->get_initial(function.arity());
        Vector<T> grad_x = function.grad(x);
        while (grad_x.norm() >= this->m_eps) {
            Function<T>* func_double = function.to_single(x);
            O method(func_double, 0, 10, this->m_eps);
            alpha = get_min(method).first;
            Vector<T> y = grad_x * alpha;
            x = x - y;
            grad_x = function.grad(x);
            this->iter(x, grad_x, method.get_call_count());
        }
        return {x, function(x)};
    }

};

template<typename T, typename Matrix = Matrix<T>>
class ConjugateGradient : public MultiOptimizer<T, Matrix> {
public:
    ConjugateGradient() : MultiOptimizer<T, Matrix>() {}
    ConjugateGradient(Vector<T> initial, T eps) : MultiOptimizer<T, Matrix>(initial, eps) {}

    std::pair<Vector<T>, T> find(QuadFunction<T, Matrix>& function) override {
        Vector<T> x = this->get_initial(function.arity());
        Vector<T> grad_x = function.grad(x);
        Vector<T> p = grad_x * (-1);
        double alpha;

        for (int i = 0; i < function.arity(); i++) {
            Vector<T> ap = function.m_A * p; // ??
            alpha = (pow(grad_x.norm(), 2)) / (ap * p);
            Vector<T> p_alpha = p * alpha;
            x = x + p_alpha;
            Vector<T> a_p_alpha = ap * alpha;
            Vector<T> curr_grad = grad_x + a_p_alpha;
            double beta = (pow(curr_grad.norm(), 2)) / (pow(grad_x.norm(), 2));
            grad_x = curr_grad;
            Vector<T> p_beta = p * beta;
            p = grad_x * (-1) + p_beta;
            this->iter(x, grad_x);
        }
        return {x, function(x)};
    }
};
