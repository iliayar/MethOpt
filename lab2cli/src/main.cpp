#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "multi_methods.hpp"

template<class O, typename T, typename M>
void test(QuadFunction<T, M>& func, bool no_data, std::experimental::optional<std::vector<T>> init_vec) {
    O* method;
    if(init_vec) {
        method = new O(init_vec.value());
    } else {
        method = new O();
    }
    auto [x, f] = method->find(func);
    std::cout << x << std::endl;
    std::cout << f << std::endl;
    auto data = method->get_data();
    std::cout << data.size() << std::endl;
    if(!no_data) {
        for (auto d : data) {
            std::cout << d << std::endl;
        }
    }
}

template<typename T, typename M>
int test(int dim, std::string method, M A, std::istream& in, std::string steepest_method, bool no_data, std::experimental::optional<std::vector<T>> init_vec) {
    Vector<T> b(dim, 0);
    T c;
    for(int i = 0; i < dim; ++i) {
        in >> b[i];
    }
    in >> c;
    QuadFunction<T, M> func(A, b, c);

    if(method == "gradient") {
        test<GradientDescent<T, M>, T, M>(func, no_data, init_vec);
    } else if(method == "conjugate") {
        test<ConjugateGradient<T, M>, T, M>(func, no_data, init_vec);
    } else if(method == "steepest") {
        if(steepest_method == "brent") {
            test<SteepestDescent<T, BrentMethod<T>, M>, T, M>(func, no_data, init_vec);
        } else if(steepest_method == "dichotomy") {
            test<SteepestDescent<T, DichotomyMethod<T>, M>, T, M>(func, no_data, init_vec);
        } else if(steepest_method == "parabolas") {
            test<SteepestDescent<T, ParabolasMethod<T>, M>, T, M>(func, no_data, init_vec);
        } else if(steepest_method == "goldensections") {
            test<SteepestDescent<T, GoldenSectionMethod<T>, M>, T, M>(func, no_data, init_vec);
        } else if(steepest_method == "fibonacci") {
            test<SteepestDescent<T, FibonacciMethod<T>, M>, T, M>(func, no_data, init_vec);
        } else {
            std::cerr << "Unknown steepest method provided" << std::endl;
        }
    } else {
        std::cerr << "Unknown method provided" << std::endl;
    }

    return 0;
}

int test(int dim, std::string method, std::string steepest_method, std::istream& in, bool diag, bool no_data, std::experimental::optional<std::vector<double>> init_vec) {
    if(diag) {
        Vector<double> A(dim, 0);
        for (int i = 0; i < dim; ++i) {
            in >> A[i];
        }
        test<double, DiagMatrix<double>>(dim, method, DiagMatrix<double>(A),
                                         in, steepest_method, no_data, init_vec);
    } else {
        Vector<Vector<double>> A(dim, Vector<double>(dim, 0));
        for (int i = 0; i < dim; ++i) {
            for (int j = 0; j < dim; ++j) {
                in >> A[i][j];
            }
        }
        test<double, Matrix<double>>(dim, method, Matrix<double>(A), in, steepest_method, no_data, init_vec);
    }
    return 0;
}

int fired_main(
    int dim = fire::arg({"-d", "--dimension", "The dimenstion number"}),

    std::string file = fire::arg({"-f", "--file",
                                  "The input file with declaring function. - To read from stdin"}),

    std::string method = fire::arg(
        {"-m", "--method", "Choose one from: gradient, conjugate, steepest"}),

    bool diag = fire::arg({"--diag", "Use diagonal matrix instead of common"}),

    fire::optional<std::string> initial =
        fire::arg({"-i", "--initial",
                   "The initial point, e.g. for --dimension=3 \"1 1 1\""}),

    double eps = fire::arg({"-e", "--epsilon"}, 1e-4),

    std::string steepest_method =
        fire::arg({"--smethod",
                   "Optimization method to use in steepest descent. Avaliable: "
                   "dichotomy, parabolas, "
                   "brent, goldensections, fibonacci"},
            "brent"),
    bool no_data = fire::arg({"--no-data", "Do not output interation data"})) {
    std::experimental::optional<std::vector<double>> initial_vec = {};
    if(initial) {
        std::stringstream ss(initial.value());
        std::vector<double> new_init(dim, 0);
        for(double& c : new_init) ss >> c;
        initial_vec = new_init;
    }
    if(file == "-") {
        return test(dim, method, steepest_method, std::cin, diag, no_data, initial_vec);
    } else {
        std::ifstream in(file);
        return test(dim, method, steepest_method, in, diag, no_data, initial_vec);
    }
}

FIRE(fired_main)
