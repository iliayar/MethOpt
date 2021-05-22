#include "matrix.hpp"

/**
 * Conjugate gradient method for finding soltion for linear quations system
 */
class ConjugateMethod {
    static constexpr double eps = 1e-7;
    static constexpr int MAX_ITER = 1e6;

public:
    static std::pair<Vector<double>, int> find(SparseMatrix<double> &a, Vector<double> &b) {
        auto x = b;
        auto r = b - (a * x);
        auto z = r;
        int i = 0;
        while ((r.norm() / b.norm()) >= eps) {
            if (i == MAX_ITER) {
                break;
            }
            i++;
            auto r_r = r * r;
            auto a_z = a * z;
            auto alpha = r_r / (a_z * z);
            x = x + z * alpha;
            auto curr_r = r - a_z * alpha;
            auto beta = curr_r * curr_r;
            beta /= r_r;
            r = curr_r;
            z = r + z * beta;
        }
        return {x, i};
    }
};
