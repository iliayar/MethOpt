#include "newton.h"
#include "quazi.h"
#include "marquardt.h"

template <template <typename> class opt>
void test(std::string name, lab4::multivariate_function<double>& function, Vector<double>& init, double prec) {
    opt<double> method{};
    auto res = method.find(function, init, prec);
    std::cout << "Method:\t" << name << std::endl;
    std::cout << "f(x):\t" << res.second << std::endl;
    std::cout << "x:\t" << res.first << std::endl << std::endl; 
}

int main() {
    std::vector<Vector<double>> v = {
            Vector<double>({2,  -1, 2}),
            Vector<double>({-1, 1, -3}),
            Vector<double>({2,  -3, 11})
    };
    Vector<double> vect = {-100, 1, -30};
    Matrix<double> matrix = Matrix<double>(Vector<Vector<double>>(v));
    lab4::quad_multivariate_function<double> function(QuadFunction<double>(matrix, vect, 10));
    double prec = 1e-4;
    Vector<double> init = {3, 4, 2};
    test<lab4::marquardt_method>("Marquardt", function, init, prec);
    test<lab4::marquardt_method_cholesky>("Marquardt with Cholesky decomposition", function, init, prec);
    test<lab4::powell_method>("Powell", function, init, prec);
    test<lab4::bfs_method>("BFS", function, init, prec);
    test<lab4::newton_ordinary>("Ordinary Newton", function, init, prec);
    test<lab4::newton_with_search>("Newton with search", function, init, prec);
    test<lab4::newton_with_descent>("Newton with descent", function, init, prec);
}
