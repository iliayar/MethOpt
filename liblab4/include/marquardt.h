#pragma once

// #include "gauss.hpp"
#include "matrix.hpp"
#include "multi_methods.hpp"
#include "optimizer.h"
#include "gauss.hpp"

namespace lab4 {

template <typename T, template <typename> class Method = BrentMethod>
struct marquardt_method : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        T tau0 = 1.0;
        T beta = 1.0 / 2.0;

        Vector<T> x = init_point;
        T fx = func.call(x);
        Matrix<T> I = Matrix<T>::I(init_point.size());
        
        while(true) {
            Vector<T> grad_x = func.get_grad(x) * (-1);
            Matrix<T> H = func.get_hessian(x);
            T tau = tau0;
            T p_norm;
            while(true) {
                Vector<T> p = solve(H + I * tau, grad_x);
                Vector<T> y = x + p;
                T fy = func.call(y);
                if(fy <= fx) {
                    x = y;
                    fx = fy;
                    p_norm = p.norm();
                    break;
                }
                tau /= beta;
            }
            tau0 = tau0 * beta;
            if(!this->iter({x})) break;
            if(p_norm <= eps) break;
        }
        return {x, fx};
    }
};
    
template <typename T, template <typename> class Method = BrentMethod>
struct marquardt_method_cholesky : public optimizer<T> {
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) override {
        Vector<T> x = init_point;
        T fx = func.call(x);
        Matrix<T> I = Matrix<T>::I(init_point.size());
        
        while(true) {
            Vector<T> grad_x = func.get_grad(x) * (-1);
            Matrix<T> H = func.get_hessian(x);
            T tau = 0;
            while (!check_cholesky(H + I * tau)) {
                tau = std::max(1.0, 2.0 * tau);
            }
            Vector<T> p = solve(H + I * tau, grad_x);
            x = x + p;
            fx = func.call(x);
            if(!this->iter({x})) break;
            if(p.norm() < eps) break;
        }
        return {x, fx};
    }

    bool check_cholesky(Matrix<T> A) {
        int n = A.size();
        Vector<Vector<T>> L(n, Vector<T>(n, 0));
        L[0][0] = sqrt(A[0][0]);
        for(int i = 1; i < n; ++i) {
            if(L[0][0] == 0) return false;
            L[i][0] = A[i][0] / L[0][0];
        }
        for(int i = 1; i < n; ++i) {
            T sum = 0;
            for(int j = 0; j < i; ++j) {
                sum += L[i][j]*L[i][j];
            }
            if (A[i][i] - sum < 0) return false;
            L[i][i] = sqrt(A[i][i] - sum);
        }
        for(int i = 1; i < n - 1; ++i) {
            for(int j = i + 1; j < n; ++j) {
                T sum = 0;
                for(int p = 0; p < i; ++p) {
                    sum += L[i][p] * L[j][p];
                }
                if(L[i][i] == 0) return false;
                L[i][j] = (A[j][i] - sum) / L[i][i];
            }
        }
        return true;
    }
};
}  // namespace lab4
