#pragma once

#include "function.h"
#include "gauss.hpp"
#include "utils.hpp"
#include <iostream>

namespace lab4 {

/**
 * The struct to log data from each iteration of method
 */
template <typename T>
struct iter_data {
    
    friend std::ostream& operator<<(std::ostream& o, iter_data<T>& data) {
        o << data.x << std::endl;
        return o;
    }

    bool valid() {
        for (int i = 0; i < x.size(); ++i) {
            if (isnanf(x[i]) || isinff(x[i])) return false;
        }
        return true;
    }

    Vector<T> x;
};

/**
 * The interfrace of optimization method
 */
template <typename T>
struct optimizer {
    const int MAX_ITERATIONS = 1e+3;

    /**
     * Finds the minimum of function, started from initial point
     * @param func The function
     * @param init_point The initial point
     * @param eps Needed precision
     * @return Pair of point of minimum and value of function in this point
     */
    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func,
                                         const Vector<T>& init_point,
                                         const T eps) = 0;

    /**
     * Log the data for one step
     * @return false if max iteration number exceed, true otherwise
     */
    bool iter(iter_data<T> data) {
        if(!data.valid()) return false;
        m_data.push_back(data);
        if (m_data.size() > MAX_ITERATIONS) {
            return false;
        }
        return true;
    }

    /**
     * Get the data from all iterations
     */
    const std::vector<iter_data<T>>& get_data() { return m_data; }

private:
    std::vector<iter_data<T>> m_data;
};

template <typename T, template <typename> class Method>
T min_alpha(const multivariate_function<T>& func, const Vector<T> x,
            const Vector<T> p, T eps) {
    Method<T> method(func.to_single(x, p), -2, 2, eps);
    return get_min(method).first;
}

template <typename T>
Vector<T> solve(PrimitiveMatrix<T>&& matrix, Vector<T>& vector) {
    PrimitiveMatrix<T> tm = std::move(matrix);
    std::vector<T> tv = vector.toStdVector();
    std::vector<T> sol = lab3::solve_main_element(tm, tv);
    return Vector<T>(sol);
}

template <typename T>
Vector<T> solve(Matrix<T>& matrix, Vector<T>& vector) {
    return solve(PrimitiveMatrix<T>(matrix), vector);
}

template <typename T>
Vector<T> solve(Matrix<T> matrix, Vector<T>& vector) {
    return solve(PrimitiveMatrix<T>(matrix), vector);
}

}  // namespace lab4
