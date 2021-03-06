#pragma once

#include "gauss.hpp"
#include "function.h"
#include "multi_methods.hpp"
#include "optimizer.h"

namespace lab4 {

/**
 * Implementation of Newton method without modifiactions
 */
template <typename T>
struct newton_ordinary : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        this->iter({x});

        while (true) {
            auto curr_grad = func.get_grad(x);
            Vector<T> t = (curr_grad * -1);
            p = solve(func.get_hessian(x), t);
            x = x + p;
            if (!this->iter({x})) break;
            if (p.norm() < eps) break;
        }

        return this->iter_last(func);
    }
};

/**
 * Implementation of Newton method with one dimensional search to find
 * the alpha paramter of direction
 */
template <typename T, template <typename> class Method = BrentMethod>
struct newton_with_search : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        this->iter({x});

        while (true) {
            auto curr_grad = func.get_grad(x);
            Vector<T> t = curr_grad * -1;
            p = solve(func.get_hessian(x), t);
            auto alpha = min_alpha<T, Method>(func, x, p, eps);
            x = x + p * alpha;
            if (!this->iter({x, alpha})) break;
            if ((p * alpha).norm() < eps) break;
        }

        return this->iter_last(func);
    }
};

/**
 * Implementation of Newton method with one dimensional search to find
 * and determining whether the p vector is valid direction
 */
template <typename T, template <typename> class Method = BrentMethod>
struct newton_with_descent : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        bool first = true;

        this->iter({x});

        while (true) {
            auto curr_grad = func.get_grad(x);
            Vector<T> t = curr_grad * -1;
            p = solve(func.get_hessian(x), t);

            if (p * curr_grad > 0 || first) {
                p = curr_grad * -1;
            }
            auto alpha = min_alpha<T, Method>(func, x, p, eps);
            x = x + p * alpha;
            first = false;
            if (!this->iter({x, alpha})) break;
            if (p.norm() < eps) break;
        }

        return this->iter_last(func);
    }
};
}  // namespace lab4
