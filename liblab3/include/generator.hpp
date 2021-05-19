#include "matrix.hpp"

#include <iostream>
#include <sstream>
#include <string>

/**
 * Generate one profile matrix of sequence of test equations Aₖxₖ=fₖ and writes it to provided stream.
 * @param k The sequece number 
 * @param n The matrix size
 * @param dist The max distance from main diagonal where elements are not zero
 */
void generate_matrix(std::ostream& out, int k, int n, int dist);

/**
 * Generate one test with appropriate matrix {@link #generate_matrix(std::ostream&, int, int, int)}.
 */
void generate_test(std::ostream& out, int k, int n, int dist);


/**
 * Generate tests and write it to files with names {@code file_prefix_N}
 */
void generate_tests(std::string file_prefix, int k, int n, int dist);
