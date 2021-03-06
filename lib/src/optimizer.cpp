#include "optimizer.hpp"


template<typename T>
GoldenRation<T>::GoldenRation(T xl, T xr, T eps)
    : m_xr(xr), m_xl(xl), m_eps(eps) {
    
}

template<typename T>
bool GoldenRation<T>::forward() {
    return false;
}

template<typename T>
T GoldenRation<T>::get_min() {
    return 0;
}
