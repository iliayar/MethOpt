#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "matrix.hpp"

/**
 * Generate one profile matrix of sequence of test equations Aₖxₖ=fₖ and writes
 * it to provided stream.
 * @param k The sequece number
 * @param n The matrix size
 * @param dist The max distance from main diagonal where elements are not zero
 */
void generate_diag_dens(std::ostream& out, int k, int n, int dist);

/**
 * Generate tests and write it to files with names {@code file_prefix_N}
 */
void generate_tests(std::string file_prefix, int k, int n, int dist);

void generate_tests_hilbert(std::string file_prefix, int k, int n, int dist);

void generate_hilberts(std::ostream& out, int n);
