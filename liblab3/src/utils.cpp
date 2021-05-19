#include "utils.hpp"

#include <cmath>
#include "gauss.hpp"

double vec_norm(std::vector<double> x) {
    double s = 0;
    for(double e : x) {
        s += e*e;
    }
    return sqrt(s);
}

std::vector<double> vec_sub(std::vector<double> a, std::vector<double> b) {
    std::vector<double> res = a;
    for(int i = 0; i < a.size(); ++i) {
        a[i] -= b[i];
    }
    return res;
}

std::vector<double> solve(ProfileMatrix<double>&& matrix, std::vector<double> f) {
    LUDecomposition<double> lu(std::move(matrix));
    std::vector<double> t = gauss_bottom_triangle(lu, f);
    std::vector<double> result = gauss_upper_triangle(lu, t);
    return result;
}
