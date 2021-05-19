#include "matrix.hpp"

// destroys input vector
template<typename T>
std::vector<T> gauss_bottom_triangle(LUDecomposition<T> &lu, std::vector<T> &vec) {
    auto temp = L_matrix_proxy<T>(lu);
    return gauss_bottom_triangle(temp, vec);
}

template<typename T>
std::vector<T> gauss_bottom_triangle(AbstractMatrix<T> &lu, std::vector<T> &vec) {
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            T c = lu.get(j, i) / lu.get(i, i);
            vec[j] = vec[j] - c * vec[i];
//            lu.get(j, i) = 0;
        }
        result[i] = vec[i] / lu.get(i, i);
    }
    return result;
}

// destroys input vector
template<typename T>
std::vector<T> gauss_upper_triangle(LUDecomposition<T> &lu, std::vector<T> &vec) {
    auto temp = U_matrix_proxy<T>(lu);
    return gauss_upper_triangle(temp, vec);
}

// destroys input vector
template<typename T>
std::vector<T> gauss_upper_triangle(AbstractMatrix<T> &lu, std::vector<T> &vec) {
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = size - 1; i >= 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
//            in lu always 1 but here we working with an unspecified matrix
            T c = lu.get(j, i) / lu.get(i, i);
            vec[j] = vec[j] - c * vec[i];
//            lu.get(j, i) = 0;
        }
        result[i] = vec[i] / lu.get(i, i);
    }
    return result;
}

template<typename T>
std::vector<T> gauss_main_element(PrimitiveMatrix<T> &matrix, std::vector<T> &vec) {
    int size = vec.size();
    for (int i = 0; i < size; ++i) {
        int max_el_row = i;
        T max = matrix.get(i, i);
        for (int j = i + 1; j < size; ++j) {
            if (matrix.get(j, i) > max) {
                max = matrix.get(j, i);
                max_el_row = j;
            }
        }
        matrix.swap_rows(i, max_el_row);
        std::swap(vec[i], vec[max_el_row]);

        for (int j = i + 1; j < size; ++j) {
            T c = matrix.get(j, i) / matrix.get(i, i);
            vec[j] = vec[j] - c * vec[i];
//            matrix.get(j, i) = 0;
            for (int k = i + 1; k < size; ++k) {
                matrix.get(j, k) = matrix.get(j, k) - c * matrix.get(i, k);
            }
        }
    }
    return gauss_upper_triangle(matrix, vec);
}
