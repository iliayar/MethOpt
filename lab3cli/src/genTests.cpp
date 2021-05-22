#include "matrix.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>
#include "generator.hpp"

int tests_main(std::string file = fire::arg({"-f", "--file", "file prefix"})
               , int kmax = fire::arg({"-c", "--count", "Tests count"})
               , int n = fire::arg({"-n", "Max dimension"})
               , int dist = fire::arg({"-d", "--dist", "The distance from main diag where elements are not zero"})
               , bool sim = fire::arg({"-s", "--symetrical", "Generate matrix symeetrical"})
               , bool hilberts = fire::arg({"--hilbert", "Generate Hilbert matrice. -d, -c parameters ignored"})) {
    if(hilberts) {
        generate_tests_hilbert(file, n);
    } else {
        generate_tests(file, kmax, n, dist, sim);
    }
    return 0;
}

FIRE(tests_main);
