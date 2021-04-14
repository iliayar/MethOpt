#pragma once

#include "multi_function.hpp"

/**
 * The class represents Optimizer for functions with several arguments.
 * @tparam The type of values
 */
template <typename T>
class MultiOptimizer {
public:
    T sigma = static_cast<T>(1e-4);

    virtual std::pair<Vector<T>, T> find(QuadFunction<T>& function) = 0;
    
private:
    // TODO Store data
};
