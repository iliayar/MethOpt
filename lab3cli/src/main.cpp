#include "matrix.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>
#include "generator.hpp"

int fired_main(std::string file = fire::arg({"-f", "--file", "The input file"})) {
    // std::stringstream ss;
    // generate(ss, 1, 9, 2);
    // ProfileMatrix<double> matrix(ss);
    // std::cout << matrix << std::endl;
    std::ifstream input(file);
    ProfileMatrix<double> matrix(input);
    std::cout << matrix << std::endl;
    LUDecomposition<double> lu(std::move(matrix));
    std::cout << "Matrix L" << '\n';
    for (int i = 0; i < lu.size(); ++i) {
        for (int j = 0; j < lu.size(); ++j) {
            std::cout << lu.getInL(i, j) << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "Matrix U" << '\n';
    for (int i = 0; i < lu.size(); ++i) {
        for (int j = 0; j < lu.size(); ++j) {
            std::cout << lu.getInU(i, j) << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}

FIRE(fired_main)
