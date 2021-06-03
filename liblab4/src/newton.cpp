#include "newton.h"

int main() {
    newton_ordinary<double> method;
    std::vector<Vector<double>> keks = {
            Vector<double>({2,  -1, 2}),
            Vector<double>({-1, 1, -3}),
            Vector<double>({2,  -3, 11})
    };
    Vector<Vector<double>> m = Vector<Vector<double>>(keks);
    Vector<double> vect = {-100, 1, -30};
    Matrix<double> matrix = Matrix<double>(m);
    QuadFunction<double> f = QuadFunction<double>(matrix, vect, 10);
    quad_multivariate_function<double> function(f);
    std::cout << method.find(function, {1, 1, 1}, 1e-4).second;
}