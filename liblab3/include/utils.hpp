#pragma once

#include <vector>
#include "matrix.hpp"
#include <iostream>

double vec_norm(std::vector<double> x);
std::vector<double> vec_sub(std::vector<double> a, std::vector<double> b);

std::vector<double> solve(ProfileMatrix<double>&& matrix, std::vector<double> f);

void run_test(std::istream& on);

