#include <fire-hpp/fire.hpp>
#include <iostream>
#include <string>
#include "multi_function.hpp"


int fired_main() {
    Vector<int, 3> vec({1, 2});
    std::cout << "Hello, world!" << std::endl;
    return 0;
}

FIRE(fired_main)
