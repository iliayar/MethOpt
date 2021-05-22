#include "utils.hpp"

#include <cmath>
#include <fstream>
#include "gauss.hpp"
#include "conjugate.hpp"

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
        res[i] -= b[i];
    }
    return res;
}

std::vector<double> solve(ProfileMatrix<double>&& matrix, std::vector<double> f) {
    LUDecomposition<double> lu(std::move(matrix));
    std::vector<double> t = gauss_bottom_triangle(lu, f);
    std::vector<double> result = gauss_upper_triangle(lu, t);
    return result;
}

std::vector<double> solve_main_element(ProfileMatrix<double>&& matrix, std::vector<double> f) {
    auto prim_matrix = PrimitiveMatrix<double>(matrix);
    return gauss_main_element(prim_matrix, f);
}

std::vector<double> solve_conjugate(ProfileMatrix<double>&& matrix, std::vector<double> f) {
    auto sparse_matrix = SparseMatrix<double>(matrix);
    auto wrapped_input_vector = Vector<double>(f);
    return ConjugateMethod::find(sparse_matrix, wrapped_input_vector).toStdVector();
}

int run_test(std::istream& in, std::ostream& out) {
    ProfileMatrix<double> matrix(in);
    int size = matrix.size();
    std::vector<double> f(size);
    std::vector<double> x_true(size);
    for(double& e : f) {
        in >> e;
    }
    for(double& e : x_true) {
        in >> e;
    }
    // std::vector<double> x = solve(std::move(matrix), f);
    // std::vector<double> x = solve_main_element(std::move(matrix), f);
    std::vector<double> x = solve_conjugate(std::move(matrix), f);
    double error = vec_norm(vec_sub(x_true, x));
    out << error << " " << error / vec_norm(x_true);
    return size;
}

void run_tests(std::string file_prefix, int kmax, int n) {
    for (int k = 0; k < kmax; ++k) {
        std::ifstream in(file_prefix + "_k" + std::to_string(k) + "_n" +
                          std::to_string(n));
        std::cout << n << " " << k <<  " ";
        run_test(in, std::cout);
        std::cout << std::endl;
    }
}
