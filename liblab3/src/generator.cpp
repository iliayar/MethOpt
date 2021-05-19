#include "generator.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "matrix.hpp"

namespace {
int rand(int max) { return abs(std::rand()) % (max + 1); }
template <typename T>
T rand(std::vector<T>& v) {
    return v[rand(v.size() - 1)];
}

void generate_test(std::ostream& out, ProfileMatrix<double>& matrix, int n) {
    std::vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = i + 1;
    }
    std::vector<double> f = matrix.mul(x);
    matrix.dump(out);
    for (double e : f) {
        out << e << " ";
    }
    out << std::endl;
    for (double e : x) {
        out << e << " ";
    }
    out << std::endl;
}

}  // namespace

void generate_diag_dens(std::ostream& out, int k, int n, int dist) {
    std::vector<double> values = {0, -1, -2, -3, -4};
    std::vector<double> diag(n, 0);
    std::vector<size_t> ai(n + 1);
    ai[0] = 0;
    for (int i = 1; i < n + 1; ++i) {
        ai[i] = ai[i - 1] + rand(std::min(i - 1, dist));
    }
    std::vector<double> al(ai[n], 0);
    std::vector<double> au(ai[n], 0);
    for (int i = 0; i < n; ++i) {
        int start = ai[i];
        int size = ai[i + 1] - ai[i];
        double x;
        for (int j = 0; j < size; ++j) {
            x = rand(values);
            if (j == 0)
                while (x == 0) x = rand(values);
            au[start + j] = al[start + j] = x;
            diag[i] -= al[start + j];
            diag[i - (size - j)] -= au[start + j];
        }
    }
    diag[0] += pow(10, -k);
    out << n << std::endl;
    for (double e : diag) {
        out << e << " ";
    }
    out << std::endl;
    for (size_t i : ai) {
        out << i + 1 << " ";
    }
    out << std::endl;
    for (double e : al) {
        out << e << " ";
    }
    out << std::endl;
    for (double e : au) {
        out << e << " ";
    }
    out << std::endl;
}

void generate_tests(std::string file_prefix, int kmax, int n, int dist) {
    std::srand(std::time(nullptr));
    for (int k = 0; k < kmax; ++k) {
        std::ofstream out(file_prefix + "_" + std::to_string(k));
        std::stringstream ss;
        generate_diag_dens(ss, k, n, dist);
        ProfileMatrix<double> matrix(ss);
        generate_test(out, matrix, n);
        out.close();
    }
}

void generate_tests_hilbert(std::string file_prefix, int nmax) {
    std::srand(std::time(nullptr));
    for (int n = 0; n < nmax; ++n) {
        std::ofstream out(file_prefix + "_" + std::to_string(n));
        std::stringstream ss;
        generate_hilberts(ss, n);
        ProfileMatrix<double> matrix(ss);
        generate_test(out, matrix, n);
        out.close();
    }
}

void generate_hilberts(std::ostream& out, int n) {
    std::vector<std::vector<double>> a(n, std::vector<double>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = 1 / (double)(i + j + 1);
        }
    }
    PrimitiveMatrix<double> pmatrix(a);
    ProfileMatrix<double> matrix(pmatrix);
    matrix.dump(out);
}
