#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "multi_function.hpp"
#include "multi_methods.hpp"

template<class O, typename T>
void test(QuadFunction<T>& func) {
    O method{};
    auto [x, f] = method.find(func);
    std::cout << x << std::endl;
    std::cout << f << std::endl;
    std::cout << method.get_iterations() << std::endl;
}

int fired_main(
    int dim = fire::arg({"-d", "--dimension", "The dimenstion number"}),
    std::string file = fire::arg({"-f", "--file",
            "The input file with declaring function"}),
    std::string method = fire::arg({"-m", "--method", "Choose one from: gradient, conjucting, steepest"})) {
    std::ifstream in(file);
    Vector<Vector<double>> A(dim, Vector<double>(dim, 0));
    Vector<double> b(dim, 0);
    double c = 10;
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            in >> A[i][j];
        }
    }
    for(int i = 0; i < dim; ++i) {
        in >> b[i];
    }
    in >> c;

    QuadFunction<double> func(A, b, c);

    if(method == "gradient") {
        test<GradientDescent<double>, double>(func);
    } else if(method == "conjucting") {
        test<ConjugateGradient<double>, double>(func);
    } else if(method == "steepest") {
        test<SteepestDescent<double>, double>(func);
    } else {
        std::cerr << "Unknown method provided" << std::endl;
    }

    return 0;
}

FIRE(fired_main)
