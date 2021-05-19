#pragma once

#include <vector>

double vec_norm(std::vector<double> x);
std::vector<double> vec_sub(std::vector<double> a, std::vector<double> b);

std::vector<double> solve(ProfileMatrix<double>&& matrix, std::vector<double> f);

