#pragma once

// #include "gauss.hpp"
#include "matrix.hpp"
#include "multi_methods.hpp"
#include "optimizer.h"

namespace lab4 {

/**
 * The abstract class for quazi method, which are differs only in way
 * calculating Gₖ
 */
template <typename T, template <typename> class Method = BrentMethod>
struct abstract_quazi_method : public optimizer<T> {
    /**
     * The all variants:
     * 1. αₖ = 1
     * 2. αₖ fragmentation
     * 3. αₖ calculated from single variable optimization method
     * This implementation for case 3.
     */
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        Vector<T> w = func.get_grad(x) * -1;
        Vector<T> p = w;
        T alpha = min_alpha<T, Method>(func, x, p, eps);
        Vector<T> x1 = x + p * alpha;
        Vector<T> delta_x = x1 - x;
        Matrix<T> G = Matrix<T>::I(init_point.size());

        this->iter({x});
        this->iter({x1});
            
        while (true) {
            x = x1;
            Vector<T> w1 = func.get_grad(x) * -1;
            Vector<T> delta_w = w1 - w;
            w = w1;
            G = std::move(update(G, delta_x, delta_w));
            p = G * w;
            alpha = min_alpha<T, Method>(func, x, p, eps);
            x1 = x + p * alpha;
            delta_x = x1 - x;
            if (!this->iter({x1, alpha})) break;
            if (delta_x.norm() < eps) break;
        }

        return this->iter_last(func);
    }

    /**
     * Build the Gₖ₊₁ from Gₖ and Δx, Δw
     */
    virtual Matrix<T> update(Matrix<T>& Gk, Vector<T>& delta_x,
                             Vector<T>& delta_w) = 0;

    Matrix<T> cross(Vector<T> a, Vector<T> b) {
        size_t n = a.size();
        Vector<Vector<T>> res(n, Vector<T>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                res[i][j] = a[j] * b[i];
            }
        }
        return Matrix<T>(res);
    }
};

template <typename T>
struct bfs_method : public abstract_quazi_method<T> {
    virtual Matrix<T> update(Matrix<T>& G, Vector<T>& delta_x,
                             Vector<T>& delta_w) override {
        T rho = G * delta_w * delta_w;
        Vector<T> r = G * delta_w * (1 / rho) -
                      delta_x * (1 / (delta_x * delta_w));
        Matrix<T> G1 =
            G - this->cross(delta_x, delta_x) * (1 / (delta_x * delta_w)) -
            G * this->cross(delta_w, delta_w) * G.transpose() * (1 / rho) +
            this->cross(r, r) * rho;
        return G1;
    }
};

template <typename T>
struct powell_method : public abstract_quazi_method<T> {
    virtual Matrix<T> update(Matrix<T>& G, Vector<T>& delta_x,
                             Vector<T>& delta_w) override {
        Vector<T> x1 = delta_x + G * delta_w;
        Matrix<T> G1 = G - this->cross(x1, x1) * (1 / (delta_w * x1));
        return G1;
        // return G;
    }
};

}  // namespace lab4
