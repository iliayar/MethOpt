#pragma once

#include "helpers.h"

namespace lab4 {

template <typename T>

/**
 * The struct to log data from each iteration of method
 */
struct iter_data {
    // TODO
};

/**
 * The interfrace of optimization method
 */
template <typename T>
struct optimizer {
    const int MAX_ITERATIONS = 1e+6;

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

}  // namespace lab4
