#include "optimizer.hpp"

GoldenRation::GoldenRation(double xl, double xr, double eps)
    : m_xr(xr), m_xl(xl), m_eps(eps) {
    
}

bool GoldenRation::forward() {
    return false;
}

double GoldenRation::get_min() {
    return 0;
}
