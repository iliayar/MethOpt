#include "matrix.hpp"

#include <iostream>
#include <sstream>
#include <string>

/**
 * Generate one diagonal matrix of sequence of test queations Aₖxₖ=fₖ and writes it to provided stream.
 * @param k The sequece number 
 * @param n The matrix size
 * @param dist The max distance from main diagonal where elements are not zero
 */
void generate(std::ostream& out, int k, int n, int dist);
