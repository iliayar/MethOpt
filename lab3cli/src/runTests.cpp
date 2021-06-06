#include "matrix.hpp"
#include "utils.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>

int tests_main(std::string file = fire::arg({"-f", "--file", "file prefix"})
               , int kmax = fire::arg({"-c", "--count", "Tests count"})
               , int n = fire::arg({"-n", "Max dimension"})
               , bool hilberts = fire::arg({"--hilbert", "Tests Hilbert matrice. -c parameters ignored"})) {
    if(hilberts) {
        std::ifstream in(file + "_n" + std::to_string(n));
        lab3::run_test(in, std::cout);
        std::cout << std::endl;
        in.close();
    } else {
        lab3::run_tests(file, kmax, n);
    }
    return 0;
}

FIRE(tests_main);
