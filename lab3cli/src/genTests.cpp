#include "matrix.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>
#include "generator.hpp"

int tests_main(std::string file = fire::arg({"-f", "--file", "file prefix"})
               , int kmax = fire::arg({"-c", "--count", "Tests count"})
               , int n = fire::arg({"-n", "dimension"})
               , int dist = fire::arg({"-d", "--dist", "The distance from main diag where elements are not zero"})
               , bool hilberts = fire::arg({"-h", "--hilbert", "Generate Hilbert matrice"})) {
    generate_tests(file, kmax, n, dist);
    return 0;
}

FIRE(tests_main);
