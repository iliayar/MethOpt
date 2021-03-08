#pragma once

#include "optimizer.hpp"

#include <cmath>
#include <algorithm>

enum class Method {
    Dichotomy,
    GoldenSection,
    Fibonacci,
    Parabolas,
    Brent
};

template <typename T>
class DichotomyMethod : public Optimizer<T> {
private:
    T m_a, m_b, sigma, m_x1, m_x2, m_delta, m_fx1, m_fx2;
public:
    DichotomyMethod(Function<T>* function, T a, T b, T eps = EPS)
        : Optimizer<T>(function), m_a(a), m_b(b), sigma(eps) {
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

        return (m_b - m_a) / 2 > sigma;
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
};

template <typename T>
class GoldenSectionMethod : public Optimizer<T> {
private:
    const T const2 = static_cast<double>((sqrt(5) - 1) / 2);
    T a, b, sigma, x1, x2, f_x1, f_x2;
public:
    GoldenSectionMethod(Function<T>* function, T a, T b, T eps = EPS)
        : Optimizer<T>(function), a(a), b(b), sigma(eps) {
        x1 = b - const2 * (b - a);
        x2 = const2 * (b - a) + a;
        f_x1 = (*this->m_function)(x1);
        f_x2 = (*this->m_function)(x2);
    }
    bool forward() {
        if (f_x1 > f_x2) {
            a = x1;
            x1 = x2;
            f_x1 = f_x2;
            x2 = a + const2 * (b - a);
            f_x2 = (*this->m_function)(x2);
        } else {
            b = x2;
            x2 = x1;
            f_x2 = f_x1;
            x1 = b - const2 * (b - a);
            f_x1 = (*this->m_function)(x1);
        }

        return abs(b - a) > sigma;
    }
    std::pair<T, T> get_min() {
        T x = (a + b) / 2;
        return std::make_pair(x, (*this->m_function)(x));
    }
    std::vector<IterationData<T>*> get_data() {
        return {new IterationInterval<T>(a, b), new IterationPoint<T>(x1, f_x1),
                new IterationPoint<T>(x2, f_x2)};
    }
};

template <typename T>
class FibonacciMethod : public Optimizer<T> {
private:
    T a, b, sigma, x1, x2, f_x1, f_x2;
    int k = 0, n;
    std::vector<T> fib;
public:
    FibonacciMethod(Function<T>* function, T a, T b, T eps = EPS)
        : Optimizer<T>(function), a(a), b(b), sigma(eps) {
        fib.resize(100, 0);
        fib[0] = 1;
        fib[1] = 1;
        int sz = (int)((b - a) / sigma) + 1;
        n = 2;
        for (;; n++) {
            fib[n] = fib[n - 1] + fib[n - 2];
            if (fib[n] > sz) {
                break;
            }
        }
        x1 = a + (fib[n - 2] / fib[n]) * (b - a);
        x2 = a + (fib[n - 1] / fib[n]) * (b - a);
        f_x1 = (*this->m_function)(x1);
        f_x2 = (*this->m_function)(x2);
    }
    bool forward() {
        if (f_x1 > f_x2) {
            a = x1;
            x1 = x2;
            f_x1 = f_x2;
            x2 = a + (fib[n - k - 1] / fib[n - k]) * (b - a);
            f_x2 = (*this->m_function)(x2);
        } else {
            b = x2;
            x2 = x1;
            f_x2 = f_x1;
            x1 = a + (fib[n - k - 2] / fib[n - k]) * (b - a);
            f_x1 = (*this->m_function)(x1);
        }
        return k++ < n - 1;
    }
    std::pair<T, T> get_min() {
        if ((*this->m_function)(x1) == (*this->m_function)(x2)) {
            a = x1;
        } else {
            b = x2;
        }
        T x = (a + b) / 2;
        return std::make_pair(x, (*this->m_function)(x));
    }
    std::vector<IterationData<T>*> get_data() {
        return {new IterationInterval<T>(a, b), new IterationPoint<T>(x1, f_x1),
                new IterationPoint<T>(x2, f_x2)};
    }
};

template <typename T>
class ParabolasMethod : public Optimizer<T> {
private:
    T a, b, sigma, x2, x_res, f_a, f_b, f_x2, u, f_u;
public:
    ParabolasMethod(Function<T>* function, T a, T b, T eps = EPS)
        : Optimizer<T>(function), a(a), b(b), sigma(eps) {
        x2 = (a + b) / 2;
        x_res = a;
        f_a  = (*this->m_function)(a);
        f_b  = (*this->m_function)(b);
        f_x2 = (*this->m_function)(x2);
    }
    bool forward() {
        u =
            x2 - ((x2 - a) * (x2 - a) * (f_x2 - f_b) -
                  (x2 - b) * (x2 - b) * (f_x2 - f_a)) /
            ((x2 - a) * (f_x2 - f_b) - (x2 - b) * (f_x2 - f_a)) / 2;
        f_u = (*this->m_function)(u);
        if (abs(u - x_res) < sigma) {
            return false;
        }
        if ((u > a) && (u < x2)) {
            if (f_u <= f_x2) {
                b = x2;
                f_b = f_x2;
                x2 = u;
                f_x2 = f_u;
                x_res = u;
            } else {
                a = u;
                f_a = f_u;
                x_res = u;
            }
        } else {
            if (f_x2 >= f_u) {
                a = x2;
                f_a = f_x2;
                x2 = u;
                f_x2 = f_u;
                x_res = u;
            } else {
                b = u;
                f_b = f_u;
                x_res = u;
            }
        }
        return true;
    }
    std::pair<T, T> get_min() {
        return std::make_pair(x_res, (*this->m_function)(x_res));
    }
    std::vector<IterationData<T>*> get_data() {
        return {new IterationInterval<T>(a, b),
                new IterationPoint<T>(u, f_u)};  // :FIXME:
    }
};

template <typename T>
class BrentMethod : public Optimizer<T> {
private:
    const int ITER_MAX = 100;
    const T golden_sec_const = static_cast<double>(0.3819660);
    const T eps = std::numeric_limits<T>::epsilon() * 1.0e-3;
    int i = 0;
    T a, b, sigma;
    T d = 0.0, fu, fv, fw, fx;
    T tol1, tol2, u, v, w, x;
    T tol = 3e-8;
    T e = 0.0;
public:
    BrentMethod(Function<T>* function, T a, T b, T eps = EPS)
        : Optimizer<T>(function), a(a), b(b), sigma(eps) {
        x = w = v = b;
        fw = fv = fx = (*this->m_function)(x);
    }
    bool forward() {
        T xm = 0.5 * (a + b);
        tol2 = 2.0 * (tol1 = tol * abs(x) + eps);
        if (abs(x - xm) <= (tol2 - 0.5 * (b - a))) {
            return x;
        }
        if (abs(e) > tol1) {
            T r = (x - w) * (fx - fv);
            T q = (x - v) * (fx - fw);
            T p = (x - v) * q - (x - w) * r;
            q = 2.0 * (q - r);
            if (q > 0.0) p = -p;
            q = abs(q);
            T e_temp = e;
            e = d;
            if (abs(p) >= abs(0.5 * q * e_temp) || p <= q * (a - x) || p >= q * (b - x)) {
                d = golden_sec_const * (e = (x >= xm ? a - x : b - x));
            } else {
                d = p / q;
                u = x + d;
                if (u - a < tol2 || b - u < tol2)
                    d = xm - x;
            }
        } else {
            d = golden_sec_const * (e = (x >= xm ? a - x : b - x));
        }
        u = (abs(d) >= tol1 ? x + d : x + tol1);
        fu = (*this->m_function)(u);
        if (fu <= fx) {
            v = w;
            w = x;
            x = u;
            fv = fw;
            fw = fx;
            fx = fu;
        } else {
            if (u < x) a = u;
            else b = u;
            if (fu <= fw || w == x) {
                v = w;
                w = u;
                fv = fw;
                fw = fu;
            } else if (fu <= fv || v == x || v == w) {
                v = u;
                fv = fu;
            }
        }
        return i++ < ITER_MAX;
    }
    std::pair<T, T> get_min() {
        return std::make_pair(x, (*this->m_function)(x));
    }
    std::vector<IterationData<T>*> get_data() {
        return {new IterationInterval<T>(a, b), new IterationPoint<T>(u, fu)}; // :FIXME:
    }
};
