#pragma once

#include "lab1ui.hpp"

#include <QColor>

/**
 * General implementation of {@link GraphicsSouce} for {@link Optimizer} interface
 * Provided method to iterate the optimizer. Draws each iterataion data.
 * @code
 * Lab1Window* w = ...
 * w.set_source(new OptimizationGraphicsSource<DichotomyMethod<double>, double>(new Parabola<double>(), -40, -5));
 * @encode
 * @tparam O Optimizer class 
 * @tparam T All numeric values type
 */
template<class O, typename T>
class OptimizationGraphicsSource : public GraphicsSource {
public:
    OptimizationGraphicsSource(Function<T>* function, T left, T right,
                               T eps = EPS)
        : m_optimizer(function, left, right, eps), m_function(function), m_done(false) {}

    void init_imp() {
        emit add_item(create_function(m_function));
        draw_iteration(m_optimizer.get_data());
    }

    void next_iteration() {
        if(m_done) return;
        if(!m_optimizer.forward()) {
            m_done = true;
            draw_iteration(std::vector<IterationData<T>*>());
            auto [x, y] = m_optimizer.get_min();
            emit add_item(create_point(x, y, Qt::green));
        } else {
            draw_iteration(m_optimizer.get_data());
        }
    }

private:
    O m_optimizer;
    Function<T>* m_function;
    bool m_done;
};
