#include "matrix.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>

int fired_main(std::string file = fire::arg({"-f", "--file", "The input file"})) {
    std::ifstream input(file);
    ProfileMatrix<double> matrix(input);
    std::cout << matrix << std::endl;
    return 0;
}

FIRE(fired_main)
