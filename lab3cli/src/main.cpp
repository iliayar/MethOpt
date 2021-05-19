#include "matrix.hpp"
#include "gauss.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>

int fired_main(std::string file = fire::arg({"-f", "--file", "The input file"})) {
    std::ifstream input(file);
    ProfileMatrix<double> matrix(input);
    std::vector<double> input_vector(matrix.size());
    for (double &x : input_vector) {
        double t;
        input >> t;
        x = t;
    }
    LUDecomposition<double> lu(std::move(matrix));
    std::vector<double> t = gauss_bottom_triangle(lu, input_vector);
    std::vector<double> result = gauss_upper_triangle(lu, t);
    for (double &x : result) {
        std::cout << x << ' ';
    }
    return 0;
}

FIRE(fired_main)
