#include "generator.hpp"
#include <cstdlib>
#include <ctime>

namespace {
    int rand(int max) {
        return std::rand() % (max + 1);
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
    std::vector<double> ai(n + 1);
    ai[0] = 0;
    for(int i = 1; i < n + 1; ++i) {
        ai[i] = ai[i - 1] + rand();
    }
    std::vector<double> al(ai[n], 0);
    std::vector<double> au(ai[n], 0);
    for(int i = 0; i < n; ++i) {
        int start = ai[i];
        int size = ai[i + 1] - ai[i];
        double xl, xu;
        while(xl == 0 || xu == 0) {
            xl = rand(values);
            xu = rand(values);
        }
        au[start] = xu;
        al[start] = xl;
        for(int j = start + 1; j < start + size; ++j) {
            xl = rand(values);
            if(xl == 0) {
                au[j] = al[j] = 0;
            } else {
                xu = rand(values);
                while(xu == 0) xu = rand(values);
                au[j] = xu;
                al[j] = xl;
            }
        }
    }
    out << n << std::endl;
    for(int i = 0; i < n; ++i) {

    }
}
