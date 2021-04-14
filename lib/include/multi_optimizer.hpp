#pragma once

#include "multi_helpers.hpp"

template<typename T>
struct MultiIterationData {
    Vector<T> x;
    Vector<T> grad;

    friend std::ostream& operator<<(std::ostream& o, MultiIterationData<T>& data) {
        o << data.x << std::endl << data.grad;
        return o;
    }
};


/**
 * The class represents Optimizer for functions with several arguments.
 * @tparam The type of values
 */
template <typename T>
class MultiOptimizer {
public:
    T sigma = static_cast<T>(1e-4);

    virtual std::pair<Vector<T>, T> find(QuadFunction<T>& function) = 0;

    void iter(Vector<T> x, Vector<T> grad) { m_data.push_back({x, grad}); }
    std::vector<MultiIterationData<T>> get_data() { return  m_data; }
    
private:
    std::vector<MultiIterationData<T>> m_data;
    // TODO Store data
};
