#pragma once

#include "multi_function.hpp"

/**
 * The class represents Optimizer for functions with several arguments.
 * @tparam O The type of {@link Optimizer} to use.
 * @tparam The type of values
 */
template <typename T, int dimension>
class MultiOptimizer {
public:
    T sigma = static_cast<T>(1e-4);

    virtual std::pair<Vector<T, dimension>, T> find(QuadFunction<T, dimension>& function) = 0;
    
private:
    // TODO Store data
};
