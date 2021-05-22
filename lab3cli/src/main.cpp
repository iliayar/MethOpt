#include "matrix.hpp"
#include "gauss.hpp"
#include "conjugate.hpp"
#include <fire-hpp/fire.hpp>
#include <iostream>
#include <fstream>

int fired_main(std::string file = fire::arg({"-f", "--file", "The input file"})) {
    std::ifstream input(file);
    ProfileMatrix<double> matrix(input);
    auto primitive_matrix = PrimitiveMatrix<double>(matrix);
    auto sparse_matrix = SparseMatrix<double>(primitive_matrix);
    std::vector<double> input_vector(matrix.size());
    for (double &x : input_vector) {
        double t;
        input >> t;
        x = t;
        // std::cout << x << " ";
    }
    // std::cout << std::endl << matrix << std::endl;
    auto wrapped_input_vector = Vector<double>(input_vector);
    std::vector<double> input_vector_copy = input_vector;
    LUDecomposition<double> lu(std::move(matrix));
    auto t = gauss_bottom_triangle(lu, input_vector);
    std::cout << "System of linear equations solution with LU decomposition method" << std::endl;
    for (double &x : gauss_upper_triangle(lu, t)) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    std::cout << "System of linear equations solution with Gauss method's modification (main element selection)"
              << std::endl;
    for (double &x : gauss_main_element(primitive_matrix, input_vector_copy)) {
        std::cout << x << ' ';
    }

    std::cout << std::endl;

    std::cout << "System of linear equations solution with Conjugate Method"
              << std::endl;

    auto answer = ConjugateMethod::find(sparse_matrix, wrapped_input_vector);
    for (int i = 0; i < answer.size(); ++i) {
        std::cout << answer.get(i) << ' ';
    }
    return 0;
}

FIRE (fired_main)
