#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "multi_helpers.hpp"
#include "function.h"
#include "marquardt.h"
#include "newton.h"
#include "quazi.h"
#include "optimizer.h"
#include "function.h"


Matrix<double> std_vector_to_matrix(std::vector<std::vector<double>> v) {
        std::vector<Vector<double>> tv{};
        for (auto l : v) {
            tv.push_back(Vector<double>(l));
        }
        return Matrix<double>(Vector<Vector<double>>(tv));
}

Vector<double> read_init(std::string init_str) {
    std::vector<double> init{};
    std::stringstream ss(init_str);
    double x;
    while(ss >> x) {
        init.push_back(x);
    }
    return init;
}

template <template <typename> class opt>
void test(lab4::multivariate_function<double>& function, Vector<double>& init, double prec) {
    opt<double> method{};
    auto res = method.find(function, init, prec);
    std::cout << res.second << std::endl;
    std::cout << res.first << std::endl << std::endl; 
    const std::vector<lab4::iter_data<double>>& data = method.get_data();
    std::cout << data.size() << std::endl;
    for(auto d : data) {
        std::cout << d << std::endl;
    }
}

#define QUAD_FUNC(name, A, b, c)                           \
    auto name() {                                          \
        std::vector<std::vector<double>> tv A;             \
        Vector<double> vect b;                             \
        Matrix<double> matrix = std_vector_to_matrix(tv);  \
        lab4::quad_multivariate_function<double> function( \
            QuadFunction<double>(matrix, vect, c));        \
        return function;                                   \
    }

#define FUNC(name, F, G, H)                                              \
    class name : public lab4::multivariate_function<double> {            \
        virtual Vector<double> get_grad(Vector<double> point) const {    \
            double x = point[0];                                         \
            double y = point[1];                                         \
            std::vector<double> r G;                                     \
            return Vector<double>(r);                                    \
        }                                                                \
        virtual Matrix<double> get_hessian(Vector<double> point) const { \
            double x = point[0];                                         \
            double y = point[1];                                         \
            std::vector<std::vector<double>> r H;                        \
            return std_vector_to_matrix(r);                              \
        }                                                                \
        virtual double call(Vector<double> args) const {                 \
            double x = args[0];                                          \
            double y = args[1];                                          \
            return F;                                                    \
        }                                                                \
    };
QUAD_FUNC(function1,
          ({{2, -1}, {-1, 1}}) , ({2, -3}) , 10);
FUNC(function2, 2 * x * x + y * y + x * x * y, ({4 * x + 2 * x * y, 6 * y + x * x}), ({{2 * y + 4, 2 * x}, {2 * x, 6}}))
#undef QUAD_FUNC
#undef FUNC

int fired_main(
    std::string method = fire::arg({"-m", "--method", "The method to use"}),
    std::string init_str = fire::arg({"-i", "--init", "The initial vector"}),
    double eps = fire::arg({"-e", "--eps", "precision"}, 1e-4)) {
    auto function = function2();
    auto init = read_init(init_str);
    if(method == "marquardt") {
        test<lab4::marquardt_method>(function, init, eps);
    } else if(method == "marquardt_cholesky") {
        test<lab4::marquardt_method_cholesky>(function, init, eps);
    } else if(method == "powell") {
        test<lab4::powell_method>(function, init, eps);
    } else if(method == "bfs") {
        test<lab4::bfs_method>(function, init, eps);
    } else if(method == "newton_ordinary") {
        test<lab4::newton_ordinary>(function, init, eps);
    } else if(method == "newton_with_search") {
        test<lab4::newton_with_search>(function, init, eps);
    } else if(method == "newton_with_descent") {
        test<lab4::newton_with_descent>(function, init, eps);
    } else {
        std::cerr << "Unknown method" << std::endl;
    }
    return 0;
}

FIRE (fired_main)
