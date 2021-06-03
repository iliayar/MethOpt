#pragma once

#include "multi_methods.hpp"
#include "multi_helpers.hpp"
#include "gauss.hpp"

template <typename T>
class multivariate_function {
public:
//    copy - it should be changeable
    virtual Vector<T> get_grad(Vector<T> vector) const = 0;

//    copy - it should be changeable
    virtual Matrix<T> get_hessian(Vector<T> vector) const = 0;

    virtual T call(Vector<T> args) const = 0;

    Function<T>* to_single(Vector<T> point, Vector<T> p) const {
        return new StdFunction<T>([=](T x) {
            return this->call(point + p * x);
        });
    }

};

template <typename T>
class quad_multivariate_function : public multivariate_function<T> {
public:

    explicit quad_multivariate_function(QuadFunction<T> function) :function(std::move(function)) {}

//    copy - it should be changeable
    Vector<T> get_grad(Vector<T> vector) const override {
        return function.grad(vector);
    }

//    copy - it should be changeable
    Matrix<T> get_hessian(Vector<T> vector) const override {
        return function.m_A;
    }

    T call(Vector<T> args) const override {
        return function.call(args);
    }

private:
    QuadFunction<T> function;
};


//template <typename T, typename IterData>
template<typename T>
struct newton_ordinary {
    const int MAX_ITERATIONS = 1e+6;

    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func, const Vector<T>& init_point, const T eps) {

        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        while (true) {
            auto curr_grad = func.get_grad(x);
            PrimitiveMatrix<T> matrix = PrimitiveMatrix<T>(func.get_hessian(x));
            Vector<T> t = (curr_grad * -1);
            std::vector<T> tt  = t.toStdVector();
            p = gauss_main_element(matrix, tt);
            x = x + p;
            if (p.norm() < eps) break;
        }

        return {x, func.call(x)};
    }

//    bool iter(IterData i) {
//        m_data.push_back(std::move(i));
//        return m_data.size() <= MAX_ITERATIONS;
//    }
//
//    const std::vector<IterData>& get_data() { return m_data; }

private:
//    std::vector<IterData> m_data;
};

//template <typename T, typename IterData>
template<typename T, template <typename> class Method = BrentMethod>
struct newton_with_search {
    const int MAX_ITERATIONS = 1e+6;

    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func, const Vector<T>& init_point, const T eps) {

        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        while (true) {
            auto curr_grad = func.get_grad(x);
            PrimitiveMatrix<T> matrix = PrimitiveMatrix<T>(func.get_hessian(x));
            Vector<T> t = curr_grad * -1;
            auto tt = t.toStdVector();
            p = gauss_main_element(matrix, tt);
            Function<T>* func_double = func.to_single(x, p);
            BrentMethod<T> method = Method<T>(func_double, 0, 10, eps);
            auto alpha = get_min(method).first;
            x = x + p * alpha;
            if (p.norm() < eps) break;
        }

        return {x, func.call(x)};
    }

//    bool iter(IterData i) {
//        m_data.push_back(std::move(i));
//        return m_data.size() <= MAX_ITERATIONS;
//    }
//
//    const std::vector<IterData>& get_data() { return m_data; }

private:
//    std::vector<IterData> m_data;
};

//template <typename T, typename IterData>
template<typename T, template <typename> class Method = BrentMethod>
struct newton_with_descent {
    const int MAX_ITERATIONS = 1e+6;

    virtual std::pair<Vector<T>, T> find(const multivariate_function<T>& func, const Vector<T>& init_point, const T eps) {

        Vector<T> x = init_point;
        Vector<T> p(init_point.size());

        bool first = true;

        while (true) {
            auto curr_grad = func.get_grad(x);
            PrimitiveMatrix<T> matrix = PrimitiveMatrix<T>(func.get_hessian(x));
            Vector<T> t = curr_grad * -1;
            auto tt = t.toStdVector();
            p = gauss_main_element(matrix, tt);

            if (p * curr_grad > 0 || first) {
                p = curr_grad * -1;
            }
            Function<T>* func_double = func.to_single(x, p);
            BrentMethod<T> method = Method<T>(func_double, 0, 10, eps);
            auto alpha = get_min(method).first;
            x = x + p * alpha;
            first = false;
            if (p.norm() < eps) break;
        }

        return {x, func.call(x)};
    }

//    bool iter(IterData i) {
//        m_data.push_back(std::move(i));
//        return m_data.size() <= MAX_ITERATIONS;
//    }
//
//    const std::vector<IterData>& get_data() { return m_data; }

private:
//    std::vector<IterData> m_data;
};

