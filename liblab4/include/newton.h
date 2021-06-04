#pragma once

#include "gauss.hpp"
#include "helpers.h"
#include "multi_methods.hpp"
#include "optimizer.h"

namespace lab4 {

template <typename T>
struct newton_ordinary : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        while (true) {
            auto curr_grad = func.get_grad(x);
            PrimitiveMatrix<T> matrix = PrimitiveMatrix<T>(func.get_hessian(x));
            Vector<T> t = (curr_grad * -1);
            std::vector<T> tt = t.toStdVector();
            p = gauss_main_element(matrix, tt);
            x = x + p;
            if (p.norm() < eps) break;
            if (!this->iter({})) break;  // TODO
        }

        return {x, func.call(x)};
    }
};

template <typename T, template <typename> class Method = BrentMethod>
struct newton_with_search : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        while (true) {
            auto curr_grad = func.get_grad(x);
            PrimitiveMatrix<T> matrix = PrimitiveMatrix<T>(func.get_hessian(x));
            Vector<T> t = curr_grad * -1;
            auto tt = t.toStdVector();
            p = gauss_main_element(matrix, tt);
            Function<T>* func_double = func.to_single(x, p);
            BrentMethod<T> method = Method<T>(func_double, 0, 10, eps);
            auto alpha = get_min(method).first;
            x = x + p * alpha;
            if (p.norm() < eps) break;
            if (!this->iter({})) break;  // TODO
        }

        return {x, func.call(x)};
    }
};

template <typename T, template <typename> class Method = BrentMethod>
struct newton_with_descent : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        bool first = true;

        while (true) {
            auto curr_grad = func.get_grad(x);
            PrimitiveMatrix<T> matrix = PrimitiveMatrix<T>(func.get_hessian(x));
            Vector<T> t = curr_grad * -1;
            auto tt = t.toStdVector();
            p = gauss_main_element(matrix, tt);

            if (p * curr_grad > 0 || first) {
                p = curr_grad * -1;
            }
            Function<T>* func_double = func.to_single(x, p);
            BrentMethod<T> method = Method<T>(func_double, 0, 10, eps);
            auto alpha = get_min(method).first;
            x = x + p * alpha;
            first = false;
            if (p.norm() < eps) break;
            if (!this->iter({})) break;  // TODO
        }

        return {x, func.call(x)};
    }
};
}  // namespace lab4
