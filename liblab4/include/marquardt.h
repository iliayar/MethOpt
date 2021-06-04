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
        T tau0 = 1 / eps;
        T beta = 1.0 / 2;

        Vector<T> x = init_point;
        T fx = func.call(x);
        Matrix<T> I = Matrix<T>::I(init_point.size());
        
        while(true) {
            Vector<T> grad_x = func.get_grad(x) * (-1);
            Matrix<T> H = func.get_hessian(x);
            T tau = tau0;
            Vector<T> y = x;
            T fy;
            Vector<T> p(x.size(), 0);
            while(true) {
                PrimitiveMatrix<T> matrix = H + I * tau;
                std::vector<T> r = grad_x.toStdVector();
                p = gauss_main_element(matrix, r);
                y = x + p;
                fy = func.call(y);
                tau = tau / beta;
                if(fy <= fx) break;
            }
            x = y;
            fx = fy;
            tau0 = tau0 * beta;
            if(!this->iter({})) break;
            if(p.norm() < eps) break;
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
            PrimitiveMatrix<T> matrix = H + I * tau;
            std::vector<T> r = grad_x.toStdVector();
            Vector<T> p = gauss_main_element(matrix, r);
            x = x + p;
            fx = func.call(x);
            if(!this->iter({})) break;
            if(p.norm() < eps) break;
        }
        return {x, fx};
    }

    bool check_cholesky(Matrix<T> A) {
        int n = A.size();
        Vector<Vector<T>> L(n, Vector<T>(n, 0));
        L[0][0] = sqrt(A[0][0]);
        for(int i = 1; i < n; ++i) {
            L[i][0] = A[i][0] / L[0][0];
        }
        for(int i = 1; i < n; ++i) {
            T sum = 0;
            for(int j = 0; j < i; ++j) {
                sum += L[i][j]*L[i][j];
            }
            if (A[i][i] - sum < 0) return false;
            L[i][i] = sqrt(L[i][i] - sum);
        }
        for(int i = 1; i < n - 1; ++i) {
            for(int j = i + 1; j < n; ++j) {
                T sum = 0;
                for(int p = 0; p < i; ++p) {
                    sum += L[i][p] * L[j][p];
                }
                L[i][j] = (A[j][i] - sum) / L[i][i];
            }
        }
        return true;
    }
};
}  // namespace lab4