#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "multi_helpers.hpp"

/*
 * Represents the square matrix
 * @tparam The numeric type
 */
template<typename T>
class AbstractMatrix {
public:

    /*
     * get element of {@code i} row and {@code j} column
     * @param i The row number
     * @param j The column number
     */
    virtual T &get(size_t i, size_t j) = 0;

//    maybe name it square matrix?
    virtual size_t size() const = 0;

    /**
     * y := Ax
     * Multiply matrix by vector vector size must be equal to {@link #size()}
     * @param x The vector mutiply by
     * @returns y
     */
    Vector<T> mul(Vector<T> x) {
        std::vector<T> res(size(), 0);
        for (int i = 0; i < size(); ++i) {
            for (int j = 0; j < size(); ++j) {
                res[i] += x[j] * get(i, j);
            }
        }
        return Vector<T>(res);
    }

    friend std::ostream &operator<<(std::ostream &o, AbstractMatrix<T> &matrix) {
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix.size(); ++j) {
                o << std::setw(4) << matrix.get(i, j) << " ";
            }
            o << std::endl;
        }
        return o;
    }
};

/**
 * Class handles the native matrix withut any optimization.
 * @tparam T numeric type
 */
template<typename T>
class PrimitiveMatrix : public AbstractMatrix<T> {
public:
    PrimitiveMatrix() = delete;

    PrimitiveMatrix(PrimitiveMatrix<T> &other) : data(other.data) {}

    PrimitiveMatrix(Matrix<T> other_matrix) {
        size_t n;
        n = other_matrix.size();
        data.resize(n);
        for (int i = 0; i < n; ++i) {
            data[i].resize(n);
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                data[i][j] = other_matrix[i][j];
            }
        }
    }

    PrimitiveMatrix(PrimitiveMatrix<T> &&other) noexcept {
        this->swap(std::move(other));
    }

    PrimitiveMatrix(AbstractMatrix<T> &other_matrix) {
        size_t n;
        n = other_matrix.size();
        data.resize(n);
        for (int i = 0; i < n; ++i) {
            data[i].resize(n);
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                data[i][j] = other_matrix.get(i, j);
            }
        }
    }

    PrimitiveMatrix(std::vector<std::vector<T>> matrix_data) : data(std::move(matrix_data)) {}

    PrimitiveMatrix<T>& operator=(PrimitiveMatrix<T>&& other) {
        this->swap(std::move(other));
        return *this;
    }

    void swap_rows(size_t i, size_t j) {
        if (i != j) {
            std::swap(data[i], data[j]);
        }
    }

    T &get(size_t i, size_t j) override {
        return data[i][j];
    }

    size_t size() const override {
        return data.size();
    }

private:
    void swap(PrimitiveMatrix<T> &&other) {
        std::swap(this->data, other.data);
    }

    std::vector<std::vector<T>> data;
};

/**
 * Class represent a matrix stored in optimized way using profiles.
 */
template<typename T>
class ProfileMatrix : public AbstractMatrix<T> {
public:
    ProfileMatrix() = delete;

    ProfileMatrix(ProfileMatrix<T> &) = delete;

    ProfileMatrix(ProfileMatrix<T> &&other) noexcept {
        this->swap(std::move(other));
    }

    ProfileMatrix(AbstractMatrix<T> &other_matrix) {
        size_t n;
        n = other_matrix.size();
        std::vector<T> &diag = m_diag;
        diag.resize(n);
        for (size_t i = 0; i < n; ++i) {
            diag[i] = other_matrix.get(i, i);
        }
        std::vector<size_t> &ia = m_ia;
        ia.resize(n + 1);
        ia[0] = 0;
        ia[1] = 0;
        for (size_t i = 1; i < n; ++i) {
            size_t not_zeros = i;
            for (size_t j = 0; j < i; ++j, --not_zeros) {
                if (other_matrix.get(i, j) == m_zero) {
                    if (other_matrix.get(j, i) != m_zero) {
                        throw std::invalid_argument("Given matrix is not profile-symmetrical");
                    }
                } else {
                    if (other_matrix.get(j, i) == m_zero) {
                        throw std::invalid_argument("Given matrix is not profile-symmetrical");
                    }
                    break;
                }
            }
            ia[i + 1] = ia[i] + not_zeros;
        }
        size_t size = ia[n];
        std::vector<T> &al = m_al;
        std::vector<T> &au = m_au;
        al.resize(size);
        au.resize(size);
        size_t al_ind = 0;
        size_t au_ind = 0;
        for (size_t i = 0; i < n; ++i) {
            bool zeros = true;
            for (size_t j = 0; j < i; ++j) {
                if (zeros && other_matrix.get(i, j) != m_zero) {
                    zeros = false;
                }
                if (!zeros) {
                    al[al_ind++] = other_matrix.get(i, j);
                    au[au_ind++] = other_matrix.get(j, i);
                }
            }
        }
    }


    ProfileMatrix(std::istream &in) {
        size_t n;
        in >> n;
        std::vector<T> &diag = m_diag;
        diag.resize(n);
        for (int i = 0; i < n; ++i) {
            in >> diag[i];
        }
        std::vector<size_t> &ia = m_ia;
        ia.resize(n + 1);
        for (int i = 0; i < n + 1; ++i) {
            in >> ia[i];
            ia[i]--;
        }
        size_t size = ia[n];
        std::vector<T> &al = m_al;
        std::vector<T> &au = m_au;
        al.resize(size);
        au.resize(size);
        for (int i = 0; i < size; ++i) {
            in >> al[i];
        }
        for (int i = 0; i < size; ++i) {
            in >> au[i];
        }
    }

    /**
     * Get element by row and column
     * @param i row
     * @param j column
     */
    T &get(size_t i, size_t j) override {
        m_zero = 0;
        if (i == j) {
            return m_diag[i];
        } else if (i > j) {
            int start = m_ia[i];
            int size = m_ia[i + 1] - m_ia[i];
            if (i - j > size) {
                return m_zero;
            } else {
                return m_al[start + (size - i + j)];
            }
        } else {
            int start = m_ia[j];
            int size = m_ia[j + 1] - m_ia[j];
            if (j - i > size) {
                return m_zero;
            } else {
                return m_au[start + (size - j + i)];
            }
        }
    }

    size_t size() const override {
        return m_diag.size();
    }

    void dump(std::ostream &out) {
        out << m_diag.size() << std::endl;
        for (T e : m_diag) {
            out << e << " ";
        }
        out << std::endl;
        for (size_t i : m_ia) {
            out << i + 1 << " ";
        }
        out << std::endl;
        for (T e : m_al) {
            out << e << " ";
        }
        out << std::endl;
        for (T e: m_au) {
            out << e << " ";
        }
        out << std::endl;
    }

private:

    void swap(ProfileMatrix<T> &&other) {
        std::swap(this->m_diag, other.m_diag);
        std::swap(this->m_ia, other.m_ia);
        std::swap(this->m_al, other.m_al);
        std::swap(this->m_au, other.m_au);
    }

    std::vector<T> m_diag;
    std::vector<size_t> m_ia;
    std::vector<T> m_al;
    std::vector<T> m_au;
    T m_zero = 0;
};

/**
 * Class performs the method of LU decomposition.
 */
template<typename T>
class LUDecomposition {
public:
    LUDecomposition() = delete;

    LUDecomposition(LUDecomposition<T> &) = delete;

    LUDecomposition(LUDecomposition<T> &&other) noexcept {
        this->swap(std::move(other));
    }

    LUDecomposition(ProfileMatrix<T> &&profile) : m_profile(std::move(profile)) {
//        computing complexity:
//        sum i = 2 to n (sum j = 1 to i - 1 (2 * sum k = 1 to j - 1 (1)) + sum k = 1 to i - 1 (1)) =
//        sum i = 2 to n (sum j = 1 to i - 1 (2j) + i) =
//        sum i = 2 to n ((i - 1) * i + i) =
//        sum i = 2 to n (i ^ 2) =
//        n ^ 3 / 3 + n ^ 2 /2 + n / 6 - 1
        int n = m_profile.size();
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                {
//                    L[i, j]
                    T sum = m_zero;
                    for (int k = 0; k < j; ++k) {
                        sum += getInL(i, k) * getInU(k, j);
                    }
                    m_profile.get(i, j) = m_profile.get(i, j) - sum;
                }
                {
//                    U[j, i]
                    T sum = m_zero;
                    for (int k = 0; k < j; ++k) {
                        sum += getInL(j, k) * getInU(k, i);
                    }
                    m_profile.get(j, i) = (m_profile.get(j, i) - sum) / getInL(j, j);
                }
            }
            {
//                    L[i, i]
                T sum = m_zero;
                for (int k = 0; k < i; ++k) {
                    sum += getInL(i, k) * getInU(k, i);
                }
                m_profile.get(i, i) = m_profile.get(i, i) - sum;
            }
        }
    }

    T &getInL(size_t i, size_t j) {
        if (i < j) {
            return m_zero;
        }
        return m_profile.get(i, j);
    }

    T &getInU(size_t i, size_t j) {
        if (i > j) {
            return m_zero;
        } else if (i == j) {
            return m_one;
        }
        return m_profile.get(i, j);
    }

    size_t size() const {
        return m_profile.size();
    }

private:

    void swap(LUDecomposition<T> &&other) {
        std::swap(m_profile, other.m_profile);
    }

    ProfileMatrix<T> m_profile;
    T m_zero = 0;
    T m_one = 1;
};

template<typename T>
class L_matrix_proxy : public AbstractMatrix<T> {
public:
    L_matrix_proxy(LUDecomposition<T> &lu) : lu_base(lu) {
    }

    T &get(size_t i, size_t j) {
        return lu_base.getInL(i, j);
    }

    size_t size() const {
        return lu_base.size();
    }

private:
    LUDecomposition<T> &lu_base;
};

template<typename T>
class U_matrix_proxy : public AbstractMatrix<T> {
public:
    U_matrix_proxy(LUDecomposition<T> &lu) : lu_base(lu) {
    }

    T &get(size_t i, size_t j) {
        return lu_base.getInU(i, j);
    }

    size_t size() const {
        return lu_base.size();
    }

private:
    LUDecomposition<T> &lu_base;
};

template<typename T>
class SparseMatrix : public AbstractMatrix<T> {
private:
    std::vector<T> m_diag;
    std::vector<T> m_al;
    std::vector<size_t> numbers_of_columns;
    std::vector<size_t> pos_first_not_zero;
    T m_zero = 0;

public:
    SparseMatrix(AbstractMatrix<T> &other_matrix) {
        size_t n = other_matrix.size();
        m_diag.resize(n);
        pos_first_not_zero.resize(n + 1);
        for (size_t i = 0; i < n; i++) {
            m_diag[i] = other_matrix.get(i, i);
        }
        size_t size = 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (other_matrix.get(i, j) != other_matrix.get(j, i)) {
                    throw std::invalid_argument("Matrix is not symmetrical");
                }
            }
            int pos = -1;
            for (size_t j = 0; j < n; j++) {
                if (i == j) {
                    continue;
                }
                if (other_matrix.get(i, j) != m_zero) {
                    size++;
                    if (pos == -1) {
                        pos = m_al.size();
                    }
                    m_al.push_back(other_matrix.get(i, j));
                    numbers_of_columns.push_back(j);
                }
            }
            pos_first_not_zero[i] = pos;
        }
        pos_first_not_zero[n] = size;
    }

    size_t size() const override {
        return m_diag.size();
    }


    T &get(size_t i, size_t j) override {
        if (i == j) {
            return m_diag[i];
        }
        if (i < j) {
            std::swap(i, j);
        }
        size_t start = pos_first_not_zero[i];
        for (size_t k = start; k < pos_first_not_zero[i + 1]; k++) {
            if (numbers_of_columns[k] == j) {
                return m_al[k];
            }
        }
        return m_zero;
    }

    Vector<T> operator*(const Vector<T> &point) {
        assert(size() == point.size());
        std::vector<T> res(size());
        for (size_t i = 0; i < size(); i++) {
            size_t start = pos_first_not_zero[i];
            for (size_t j = start; j < pos_first_not_zero[i + 1]; j++) {
                res[i] += m_al[j] * point.get(numbers_of_columns[j]);
            }
            res[i] += m_diag[i] * point.get(i);
        }
        return Vector<T>(res);
    }
};
