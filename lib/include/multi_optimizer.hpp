#pragma once

#include "multi_helpers.hpp"
#include <experimental/optional>

template<typename T>
struct MultiIterationData {
    Vector<T> x;
    Vector<T> grad;
    std::experimental::optional<int> steepest_iters;

    friend std::ostream& operator<<(std::ostream& o, MultiIterationData<T>& data) {
        o << data.x << std::endl << data.grad << std::endl;
        if(data.steepest_iters) {
            o << 1 << std::endl;
            o << data.steepest_iters.value() << std::endl;
        }
        o << 0;
        return o;
    }
};


/**
 * The class represents Optimizer for functions with several arguments.
 * @tparam The type of values
 */
template <typename T, typename Matrix = Matrix<T>>
class MultiOptimizer {
public:

    const int MAX_ITERATIONS = 1e+4;

    MultiOptimizer() : m_initial({}), m_eps(static_cast<T>(1e-4)) {}
    MultiOptimizer(Vector<T> initial, T eps) : m_initial(initial), m_eps(eps) {}

    virtual std::pair<Vector<T>, T> find(QuadFunction<T, Matrix>& function) = 0;

    bool iter(Vector<T> x, Vector<T> grad,
              std::experimental::optional<int> steepest_iter = {}) {
        m_data.push_back({x, grad, steepest_iter});
        if(m_data.size() > MAX_ITERATIONS) {
            return false;
        }
        return true;
    }
    std::vector<MultiIterationData<T>> get_data() { return m_data; }

protected:
    T m_eps;

    Vector<T> get_initial(int arity) {
        if(m_initial) {
            return m_initial.value();
        } else {
            return Vector<T>(arity, 1);
        }
    }
    
private:
    std::vector<MultiIterationData<T>> m_data;
    std::experimental::optional<Vector<T>> m_initial;
    // TODO Store data
};
