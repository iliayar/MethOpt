#pragma once
#include "multi_methods.hpp"

template <typename T>
struct multivariate_function {

//    copy - it should be changeable
    Vector<T> get_grad();

//    copy - it should be changeable
    Matrix<T> get_hessian();
};

template <typename T, typename IterData>
struct mf_optimize {
    const int MAX_ITERATIONS = 1e+6;

    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func, const Vector<T>& init_point, const T eps) = 0;

    bool iter(IterData i) {
        m_data.push_back(std::move(i));
        return m_data.size() <= MAX_ITERATIONS;
    }

    const std::vector<IterData>& get_data() { return m_data; }

private:
    std::vector<IterData> m_data;
};
