#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include "multi_function.hpp"


int fired_main() {
    // Vector<Vector<int, 3>, 3> vec((Vector<int, 3>(1, 2, 3)), Vector<int, 3>(4, 5, 6), Vector<int, 3>(7, 8, 9));
    Matrix<int, 3, 3> A(Vector<Vector<int, 3>, 3>(Vector<int, 3>(2, -1, 2),
                                                     Vector<int, 3>(-1, 1, -3),
                                                     Vector<int, 3>(2, -3, 11)));
    Vector<int, 3> b(-100, 1, -30);
    int c = 10;

    QuadFunction<int, 3> func(A, b, c);
    std::cout << func(0, 0 ,0) << std::endl;
    std::cout << func.grad(0, 0 ,0) << std::endl;
    std::cout << A * b << std::endl;
    // Matrix<int, 1 ,3> vec4{Vector<Vector<int, 3>, 1>(v1)};
    // Matrix<int, 3, 1> vec3 = vec4.transpose();
    // Matrix<int, 1, 3> vec = (vec1 * vec3).transpose();
    // std::cout << vec << std::endl;
    // auto vec =  vec1.transpose();
    return 0;
}

FIRE(fired_main)
