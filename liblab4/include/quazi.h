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
        T alpha = get_alpha(func, x, p, eps);
        Vector<T> x1 = x + p * alpha;
        Vector<T> delta_x = x1 - x;
        PrimitiveMatrix<T> G = PrimitiveMatrix<T>::I(init_point.size());

        while (true) {
            x = x1;
            Vector<T> w1 = func.get_grad(x) * -1;
            Vector<T> delta_w = w1 - w;
            w = w1;
            G = std::move(update(G, delta_x, delta_w));
            p = G.mul(w.toStdVector());
            alpha = get_alpha(func, x, p, eps);
            x1 = x + p * alpha;
            delta_x = x1 - x;
            if (delta_x.norm() < eps) break;
            if (!this->iter({})) break;
        }

        return {x, func.call(x)};
    }

    T get_alpha(const multivariate_function<T>& func, const Vector<T> x,
                const Vector<T> p, T eps) {
        Method<T> method(func.to_single(x, p), 0, 10, eps);
        return get_min(method).first;
    }

    /**
     * Build the Gₖ₊₁ from Gₖ and Δx, Δw
     */
    virtual PrimitiveMatrix<T> update(PrimitiveMatrix<T>& Gk,
                                      Vector<T>& delta_x,
                                      Vector<T>& delta_w) = 0;
};

template <typename T>
struct dummy_quazi : public abstract_quazi_method<T> {
    virtual PrimitiveMatrix<T> update(PrimitiveMatrix<T>& G,
                                      Vector<T>& delta_x,
                                      Vector<T>& delta_w) override {
        // Vector<T> rho = G.mul(delta_w.toStdVector()) * delta_w;
        // Vector<T> r = G.mul(delta_w.toStdVector()) * (1 / rho) - delta_x * (1 / (delta_x * delta_w));
        // PrimitiveMatrix<T> G1 = G - 
            // TODO
            return std::move(G);
    }
};
}  // namespace lab4
