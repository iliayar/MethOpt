#include "matrix.hpp"

// destroys input vector
template<typename T>
std::vector<T> gauss_bottom_triangle(LUDecomposition<T> &lu, std::vector<T> &vec) {
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            T c = lu.getInL(j, i) / lu.getInL(i, i);
            vec[j] = vec[j] - c * vec[i];
//            lu.getInL(j, i) = 0;
        }
        result[i] = vec[i] / lu.getInL(i, i);
    }
    return result;
}

// destroys input vector
template<typename T>
std::vector<T> gauss_upper_triangle(LUDecomposition<T> &lu, std::vector<T> &vec) {
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = size - 1; i >= 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
//            in lu always 1
            T c = lu.getInU(j, i) / lu.getInU(i, i);
            vec[j] = vec[j] - c * vec[i];
//            lu.getInU(j, i) = 0;
        }
        result[i] = vec[i] / lu.getInU(i, i);
    }
    return result;
}