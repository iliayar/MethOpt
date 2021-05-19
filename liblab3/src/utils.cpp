#include "utils.hpp"

#include <cmath>

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
