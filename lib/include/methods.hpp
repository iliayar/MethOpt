#pragma once

#include "optimizer.hpp"

enum class Method {
    Dichotomy
};

template <typename T>
class DichotomyMethod : public Optimizer<T> {
public:
    DichotomyMethod(Function<T>* function, T a, T b, T eps = EPS)
        : Optimizer<T>(function), m_a(a), m_b(b), m_eps(eps) {
        m_a = a;
        m_b = b;
        m_delta = eps;
        update_bounds();
    }
    bool forward() {
        if (m_fx1 <= m_fx2) {
            m_b = m_x2;
        } else {
            m_a = m_x1;
        }

        update_bounds();

        return (m_b - m_a) / 2 > m_eps;
    }
    std::pair<T, T> get_min() {
        double x = (m_a + m_b) / 2;
        return std::make_pair(x, (*this->m_function)(x));
    }
    std::vector<IterationData<T>*> get_data() {
        return {new IterationPoint<T>(m_x1, m_fx1),
                new IterationPoint<T>(m_x2, m_fx2),
                new IterationInterval<T>(m_a, m_b)};
    }

private:
    void update_bounds() {
        m_x1 = (m_b + m_a - m_delta) / 2;
        m_x2 = (m_b + m_a + m_delta) / 2;
        m_fx1 = (*this->m_function)(m_x1);
        m_fx2 = (*this->m_function)(m_x2);
    }
    T m_a;
    T m_b;
    T m_eps;
    T m_x1;
    T m_x2;
    T m_delta;
    T m_fx1;
    T m_fx2;
};
