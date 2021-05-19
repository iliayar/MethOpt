#include "matrix.hpp"
#include "random"
#include <sstream>

bool test_lu() {
    double lower_bound = -1000;
    double upper_bound = 1000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    size_t competed = 0;
    for (size_t iter = 0; iter < 1000; ++iter) {
        size_t size = 1 + rand() % 100;

        std::vector<std::vector<double>> raw_matrix(size, std::vector<double>(size));
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                raw_matrix[i][j] = rand() % 2 == 0 ? unif(re) : 0;
            }
        }

        PrimitiveMatrix<double> input = PrimitiveMatrix<double>(std::move(raw_matrix));

        try {
            LUDecomposition<double> lu{ProfileMatrix<double>(input)};

            std::vector<std::vector<double>> lu_result(size, std::vector<double>(size));

            for (size_t i = 0; i < size; ++i) {
                for (size_t j = 0; j < size; ++j) {
                    for (size_t k = 0; k < size; ++k) {
                        double a = lu.getInL(i, k), b = lu.getInU(k, j);
                        lu_result[i][j] += a * b;
                    }
                    if (std::abs(lu_result[i][j] - input.get(i, j)) > 1e-10) {
                        return false;
                    }
                }
            }
            std::cout << "[correct]\n";
            ++competed;
        } catch (std::invalid_argument& e) {
            std::cout << "[asymmetrical matrix skipped]\n";
        }
    }
    std::cout << "[tested " << competed << " matrices]\n";
    return true;
}

int main() {
    bool res = test_lu();
    std::cout << "Result of LU testing: " << std::boolalpha << res;
}
