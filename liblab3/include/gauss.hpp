#include "matrix.hpp"

/**
 * Solves the equation y = Ux, where U is a matrix in the upper triangular form,
 * i.e performs the reverse Gaussian. Destroys input vector, does not change
 * given matrix.
 * @param lu The upper triangle matrix in LU decomposition
 * @param vec The x vector in the equation
 */
template<typename T>
std::vector<T> gauss_upper_triangle(AbstractMatrix<T> &lu,
                                    std::vector<T> &vec) {
    //    computing complexity (muls & divs):
    //    sum i = 1 to n (sum j = 1 to i - 1 (2) + 1) =
    //    sum i = 1 to n (2i + 1) =
    //    n + 2 * (1 + n) * n / 2 =
    //    n ^ 2 + 2n;
    //    [assuming lu specification] = (n ^ 2 + 3n) / 2
    int size = vec.size();
    std::vector<T> result(size);
    for (int i = size - 1; i >= 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
            //            optimization for lu method (can be performed by
            //            the compiler): c = U[j][i];
            T c = lu.get(j, i) / lu.get(i, i);
            vec[j] = vec[j] - c * vec[i];
        }
        result[i] = vec[i] / lu.get(i, i);
    }
    return result;
}

/**
 * Solves the equation Ly = b, where L is a matrix in the lower triangular
 * form, i.e performs direct Gaussian. Destroys input vector, does not change
 * given matrix.
 * @param lu The left triangle matrice in LU decomposition
 * @prama vec The b vector in equiation
 */
template<typename T>
std::vector<T> gauss_bottom_triangle(AbstractMatrix<T> &lu,
                                     std::vector<T> &vec) {
    //    computing complexity:
    //    sum i = 1 to n (1 + sum j = i + 1 to n (2)) = n ^ 2
    int size = vec.size();
    std::vector<T> result(size);
    //    method is simplified
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
 * Helper function for {@link #gauss_upper_triangle(AbstractMatrix<T>&,
 * std::vector<T>&)}
 */
template<typename T>
std::vector<T> gauss_upper_triangle(LUDecomposition<T> &lu,
                                    std::vector<T> &vec) {
    auto temp = U_matrix_proxy<T>(lu);
    return gauss_upper_triangle(temp, vec);
}

/**
 * Helper function for {@link #gauss_bottom_triangle(AbstractMatrix<T>&,
 * std::vector<T>&)}
 */
template<typename T>
std::vector<T> gauss_bottom_triangle(LUDecomposition<T> &lu,
                                     std::vector<T> &vec) {
    auto temp = L_matrix_proxy<T>(lu);
    return gauss_bottom_triangle(temp, vec);
}

/**
 * Solves the l. eq. system using Gauss method's modification with choice of
 * main element.
 * @param matrix The matrix in natural form
 * @param vec The b vector in the equation Ax = b
 */
template<typename T>
std::vector<T> gauss_main_element(PrimitiveMatrix<T> &matrix,
                                  std::vector<T> &vec) {
    //    computing complexity:
    //    sum i = 1 to n (sum j = i + 1 to n (2 + sum k = i + 1 to n (1))) + n ^ 2 + 2n =
    //    sum i = 1 to n (sum j = i + 1 to n (2 + n - i)) + n ^ 2 + 2n =
    //    sum i = 1 to n ((n - i) * (2 + n - i)) + n ^ 2 + 2n =
    //    n * (2 * n ^ 2 + 3n - 5) / 6 + n ^ 2 + 2n =
    //    n ^ 3 / 3 + 3 * n ^ 2 / 2 + 7 * n / 6
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
