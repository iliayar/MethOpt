#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include "multi_function.hpp"
#include "multi_methods.hpp"


int fired_main() {
    Matrix<double> A({{2, -1, 2},
                      {-1, 1, -3},
                      {2, -3, 11}});
    Vector<double> b({-100, 1, -30});
    double c = 10;

    QuadFunction<double> func(A, b, c);


    GradientDescent<double> method{};
    Vector<double> p = method.find(func).first;
    std::cout << p << std::endl;
    return 0;
}

FIRE(fired_main)
