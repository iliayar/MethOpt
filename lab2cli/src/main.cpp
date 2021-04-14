#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "multi_methods.hpp"

template<class O, typename T, typename M>
void test(QuadFunction<T, M>& func) {
    O method{};
    auto [x, f] = method.find(func);
    std::cout << x << std::endl;
    std::cout << f << std::endl;
    auto data = method.get_data();
    std::cout << data.size() << std::endl;
    for(auto d : data) {
        std::cout << d << std::endl;
    }
}

template<typename T, typename M>
int test(int dim, std::string method, M A, std::ifstream& in) {
    Vector<T> b(dim, 0);
    T c;
    for(int i = 0; i < dim; ++i) {
        in >> b[i];
    }
    in >> c;
    QuadFunction<T, M> func(A, b, c);

    if(method == "gradient") {
        test<GradientDescent<T, M>, T, M>(func);
    } else if(method == "conjugate") {
        test<ConjugateGradient<T, M>, T, M>(func);
    } else if(method == "steepest") {
        test<SteepestDescent<T, BrentMethod<T>, M>, T, M>(func);
    } else {
        std::cerr << "Unknown method provided" << std::endl;
    }

    return 0;
}

int fired_main(
    int dim = fire::arg({"-d", "--dimension", "The dimenstion number"}),
    std::string file = fire::arg({"-f", "--file",
            "The input file with declaring function"}),
    std::string method = fire::arg({"-m", "--method", "Choose one from: gradient, conjugate, steepest"}),
    bool diag = fire::arg({"--diag", "Use diagonal matrix instead of common"})) {
    std::ifstream in(file);
    if(diag) {
        Vector<double> A(dim, 0);
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                if (i == j) {
                    in >> A[i];
                } else {
                    int t;
                    in >> t;
                }
            }
        }
        test<double, DiagMatrix<double>>(dim, method, DiagMatrix<double>(A), in);
    } else {
        Vector<Vector<double>> A(dim, Vector<double>(dim, 0));
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                in >> A[i][j];
            }
        }
        test<double, Matrix<double>>(dim, method, Matrix<double>(A), in);
    }
    return 0;
}

FIRE(fired_main)
