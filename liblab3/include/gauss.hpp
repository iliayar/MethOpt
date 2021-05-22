#include "matrix.hpp"

/**
 * Solve the equation y = Ux, i.e perform the reverse Gaussian. destroys input vector
 * @param lu The upper triangle matrix in LU decomposition
 * @param vec The x vector in the equation
 */
template<typename T>
std::vector<T> gauss_upper_triangle(AbstractMatrix<T> &lu, std::vector<T> &vec) {
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = size - 1; i >= 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
//            in lu always 1 but here we working with an unspecified matrix
            T c = lu.get(j, i) / lu.get(i, i);
            vec[j] = vec[j] - c * vec[i];
        }
        result[i] = vec[i] / lu.get(i, i);
    }
    return result;
}

/**
 * Solve the equation Ly = b, i.e perform direct Gaussian. destroys input vector
 * @param lu The left triangle matrice in LU decomposition
 * @prama vec The b vector in equiation
 */
template<typename T>
std::vector<T> gauss_bottom_triangle(AbstractMatrix<T> &lu, std::vector<T> &vec) {
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            T c = lu.get(j, i) / lu.get(i, i);
            vec[j] = vec[j] - c * vec[i];
        }
        result[i] = vec[i] / lu.get(i, i);
    }
    return result;
}

/**
 * Helper funtion for {@link #gauss_upper_triangle(AbstractMatrix<T>&, std::vector<T>&)}
 */
template<typename T>
std::vector<T> gauss_upper_triangle(LUDecomposition<T> &lu, std::vector<T> &vec) {
    auto temp = U_matrix_proxy<T>(lu);
    return gauss_upper_triangle(temp, vec);
}

/**
 * Helper funtion for {@link #gauss_bottom_triangle(AbstractMatrix<T>&, std::vector<T>&)}
 */
template<typename T>
std::vector<T> gauss_bottom_triangle(LUDecomposition<T> &lu, std::vector<T> &vec) {
    auto temp = L_matrix_proxy<T>(lu);
    return gauss_bottom_triangle(temp, vec);
}

/**
 * Solve the system using Gausse method with choice of main element.
 * @param matrix The matrix in natural form
 * @param vec The b vector in equition Ax = b
 */
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
            for (int k = i + 1; k < size; ++k) {
                matrix.get(j, k) = matrix.get(j, k) - c * matrix.get(i, k);
            }
        }
    }
    return gauss_upper_triangle(matrix, vec);
}
