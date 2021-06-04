#include "newton.h"
#include "quazi.h"

int main() {
    // lab4::newton_with_descent<double> method;
    // lab4::newton_ordinary<double> method;
    // lab4::newton_with_search<double> method;
    // lab4::bfs_method<double> method;
    lab4::powell_method<double> method;
    std::vector<Vector<double>> v = {
            Vector<double>({2,  -1, 2}),
            Vector<double>({-1, 1, -3}),
            Vector<double>({2,  -3, 11})
    };
    Vector<double> vect = {-100, 1, -30};
    Matrix<double> matrix = Matrix<double>(Vector<Vector<double>>(v));
    lab4::quad_multivariate_function<double> function(QuadFunction<double>(matrix, vect, 10));
    auto res = method.find(function, {1, 1, 1}, 1e-4);
    std::cout << res.second << "\n";
    std::cout << res.first << "\n";
}
