#pragma once

#include <vector>
#include "matrix.hpp"
#include <iostream>

double vec_norm(std::vector<double> x);
std::vector<double> vec_sub(std::vector<double> a, std::vector<double> b);

std::vector<double> solve(ProfileMatrix<double>&& matrix, std::vector<double> f);
std::vector<double> solve_main_element(ProfileMatrix<double>&& matrix, std::vector<double> f);
std::vector<double> solve_conjugate(ProfileMatrix<double>&& matrix, std::vector<double> f);

/**
 * Reads from {@code in} matrix, true solution, and right hand vector. Writes to out errors of solution.
 * @return dimension of matrix
 */
int run_test(std::istream& in, std::ostream& out);
void run_tests(std::string file_prefix, int k, int n);
