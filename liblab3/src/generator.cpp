#include "generator.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace {
    int rand(int max) {
        return abs(std::rand()) % (max + 1);
    }
    template <typename T>
    T rand(std::vector<T>& v) {
        return v[rand(v.size() - 1)];
    }
}


void generate(std::ostream& out, int k, int n, int dist) {
    std::srand(std::time(nullptr));
    std::vector<double> values = {0, -1, -2, -3, -4};
    std::vector<double> diag(n, 0);
    std::vector<size_t> ai(n + 1);
    ai[0] = 0;
    for(int i = 1; i < n + 1; ++i) {
        ai[i] = ai[i - 1] + rand(std::min(i - 1, dist));
    }
    std::vector<double> al(ai[n], 0);
    std::vector<double> au(ai[n], 0);
    for(int i = 0; i < n; ++i) {
        int start = ai[i];
        int size = ai[i + 1] - ai[i];
        double xl, xu;
        for(int j = 0; j < size; ++j) {
            xl = rand(values);
            if(j == 0) while(xl == 0) xl = rand(values);
            if(xl == 0) {
                au[start + j] = al[start + j] = 0;
            } else {
                xu = rand(values);
                while(xu == 0) xu = rand(values);
                au[start + j] = xu;
                al[start + j] = xl;
            }
            diag[i] -= al[start + j];
            diag[i - (size - j)] -= au[start + j];
        }
    }
    diag[0] += pow(10, -k);
    out << n << std::endl;
    for(double e : diag) {
        out << e << " ";
    }
    out << std::endl;
    for(size_t i : ai ) {
        out << i + 1 << " ";
    }
    out << std::endl;
    for(double e : al) {
        out << e  << " ";
    }
    out << std::endl;
    for(double e : au) {
        out << e << " ";
    }
    out << std::endl;
}
