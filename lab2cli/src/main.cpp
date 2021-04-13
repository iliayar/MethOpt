#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include "multi_function.hpp"
#include "multi_methods.hpp"


int fired_main() {
    // Vector<Vector<int, 3>, 3> vec((Vector<int, 3>(1, 2, 3)), Vector<int, 3>(4, 5, 6), Vector<int, 3>(7, 8, 9));
    Matrix3<double> A(Vector<Vec3<double>, 3>(
        Vec3<double>(2.0, -1.0, 2.0), Vec3<double>(-1.0, 1.0, -3.0), Vec3<double>(2.0, -3.0, 11.0)));
    Vec3<double> b(-100.0, 1.0, -30.0);
    double c = 10;

    QuadFunction<double, 3> func(A, b, c);
    // std::cout << func(0.0, 0.0 ,1.0) << std::endl;
    // std::cout << func.grad(0.0, 0.0 ,0.0) << std::endl;
    // std::cout << A * b << std::endl;


    SteepestDescent<double> method{};
    Vector<double, 3> p = method.find(func).first;
    std::cout << p << std::endl;
    // Matrix<int, 1 ,3> vec4{Vector<Vector<int, 3>, 1>(v1)};
    // Matrix<int, 3, 1> vec3 = vec4.transpose();
    // Matrix<int, 1, 3> vec = (vec1 * vec3).transpose();
    // std::cout << vec << std::endl;
    // auto vec =  vec1.transpose();
    return 0;
}

FIRE(fired_main)
