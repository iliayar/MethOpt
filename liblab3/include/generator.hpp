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
void generate_diag_dens(std::ostream& out, int n, int dist, bool sim = false);

/**
 * Generate tests and write it to files with names {@code file_prefix_N}
 */
void generate_tests(std::string file_prefix, int k, int n, int dist, bool sim = false);

/**
 * Generates hilbert matrix and writes it to the file {@code file_prefix_nN}
 * @param file_prefix
 * @param n The size of matrix and also the N in file name
 */
void generate_tests_hilbert(std::string file_prefix, int n);

/**
 * Generates hilber matrix.
 * @see #generate_tests_hilbert(std::string, int)
 */
void generate_hilberts(std::ostream& out, int n);
