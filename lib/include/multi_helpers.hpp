#pragma once

#include <array>
#include <experimental/type_traits>
#include <functional>
#include <iostream>
#include <type_traits>
#include <cassert>

#include "function.hpp"

/**
 * The class represents \(T^{size}\) value. Implements algebraic operations
 * @tparam T The type of each component
 * @tparam size Number of components
 */
template <typename T>
class Vector {
public:
    Vector(std::vector<T> vec) : m_components(vec) {}
    Vector(std::initializer_list<T> list) {
        for(T e : list) {
            m_components.push_back(e);
        }
    }

    /**
     * Creates the vector of provided size, filled with zeros
     * @param n The size of vector
     */
    Vector(int n) : Vector(n, 0) {}

    /**
     * Creates a vector with provided size, filled with providev value
     * @param n The size of vector
     * @param value The value to vector with
     */
    Vector(int n, T value) : m_components(n, value) {}

    Vector<T> operator+(const Vector<T>& lhs) const {
        assert(m_components.size() == lhs.m_components.size());
        Vector<T> res(m_components.size());
        for (int i = 0; i < m_components.size(); ++i) {
            res[i] = this->get(i) + lhs[i];
        }
        return res;
    }

    Vector<T> operator-(const Vector<T>& lhs) const {
        return lhs * (-1) + *this;
    }

    /**
     * Multiply each component of vector by constant value
     * @param c constant value
     */
    Vector<T> operator*(T c) const {
        Vector<T> res = *this;
        for (int i = 0; i < m_components.size(); ++i) {
            res[i] = res[i] * c;
        }
        return res;
    }

    /**
     * Calculate the dot product of two vectors
     * @param other The second vector
     */
    T operator*(const Vector<T>& other) const {
        T res = static_cast<T>(0);
        for (int i = 0; i < m_components.size(); ++i) {
            res += m_components[i] * other[i];
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& o,
                                    const Vector<T>& vec) {
        for (int i = 0; i < vec.size(); ++i) {
            o << vec[i] << " ";
        }
        return o;
    }

    T norm() {
        T res = static_cast<T>(0);
        for(T comp : m_components) {
            res += comp * comp;
        }
        return sqrt(res);
    }

    T& get(int i) { return m_components[i]; }
    const T& get(int i) const { return m_components[i]; }

    T& operator[](int i) { return get(i); }
    const T& operator[](int i) const { return get(i); }

    int size() const { return m_components.size(); }

    std::vector<T> toStdVector() { return m_components; }

protected:
    std::vector<T> m_components;
};

/**
 * The class represents matrix. Implements multiplication
 * @tparam T The type of each component
 */
template <typename T>
class Matrix {
public:
    explicit Matrix(Vector<Vector<T>> vec)
        : m_matrix(vec) {}

    /**
     * Perform basic matrix multiplication
     * @param rhs The other matrix
     */
    Matrix<T> operator*(const Matrix<T>& rhs) const {
        int height = this->size();
        int width = this->get(0).size();
        int rwidth = rhs[0].size();
        assert(width == rhs.size());
        Matrix<T> res(Vector<Vector<T>>(height, Vector<T>(rwidth)));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < rwidth; ++j) {
                for (int k = 0; k < width; ++k) {
                    res[i][j] += this->get(i)[k] * rhs[k][j];
                }
            }
        }
        return res;
    }

    Matrix<T> transpose() const {
        int height = this->size();
        int width = this->get(0).size();
        Matrix<T> res(Vector<Vector<T>>(width, Vector<T>(height)));
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                res[i][j] = this->get(j)[i];
            }
        }
        return res;
    }

    Matrix<T> operator*(T value) const {
        Vector<Vector<T>> matrix(size(), Vector<T>(size(), 0));
        for(int i = 0; i < size(); ++i) {
            for(int j = 0; j < size(); ++j) {
                matrix[i][j] = get(i)[j] * value;
            }
        }
        return Matrix<T>(matrix);
    }

    /**
     * Perform multiplication of matrix by vector, which one is represents as
     * matrix with shape {@code (1, width)}
     * @param rhs The vector of size {@code width}
     */
    Vector<T> operator*(const Vector<T>& rhs) const {
        assert(this->get(0).size() == rhs.size());
        return (*this * (Matrix<T>{Vector<Vector<T>>(1, rhs)}.transpose()))
            .transpose()[0];
    }

    Matrix<T> operator+(const Matrix<T>& other) const {
        return Matrix<T>(m_matrix + other.m_matrix);
    }
    
    Matrix<T> operator-(const Matrix<T>& other) const {
        return *this + (other * (-1));
    }

    friend std::ostream& operator<<(std::ostream& o, Matrix<T> m) {
        int height = m.size();
        int width = m[0].size();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                o << m[i][j] << " ";
            }
            o << std::endl;
        }
        return o;
    }

    static Matrix<T> I(size_t n) {
        Vector<Vector<T>> matrix(n, Vector<T>(n, 0));
        for(int i = 0; i < n; ++i) {
            matrix[i][i] = 1;
        }
        return Matrix<T>(matrix);
    }

    Vector<T>& get(int i) { return m_matrix[i]; }
    const Vector<T>& get(int i) const { return m_matrix[i]; }
    Vector<T>& operator[](int i) { return get(i); }
    const Vector<T>& operator[](int i) const { return get(i); }

    int size() const { return m_matrix.size(); }

private:
    Vector<Vector<T>> m_matrix;
};

/**
 * The class to store m_diag matrix. Stores only main m_diag.
 */
template <typename T>
class DiagMatrix {
public:
    explicit DiagMatrix(Vector<T> vec)
        : m_diag(vec) {}

    DiagMatrix<T> operator*(const DiagMatrix<T>& rhs) const {
        Vector<T> res(size(), 0);
        for(int i = 0; i < size(); ++i) {
            res[i] = get(i) * rhs[i];
        }
        return DiagMatrix<T>(res);
    }

    Vector<T> operator*(const Vector<T>& rhs) const {
        return (*this * DiagMatrix<T>(rhs)).m_diag;
    }

    friend std::ostream& operator<<(std::ostream& o, DiagMatrix<T> m) {
        int height = m.size();
        int width = height;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if(i == j) {
                    o << m.get(i, j) << " ";
                } else {
                    o << "0 ";
                }
            }
            o << std::endl;
        }
        return o;
    }

    T& get(int i) { return m_diag[i]; }
    const T& get(int i) const { return m_diag[i]; }
    T& operator[](int i) { return get(i); }
    const T& operator[](int i) const { return get(i); }

    int size() const { return m_diag.size(); }
private:
    Vector<T> m_diag;
};

/**
 * The interface of quadratic functions, that takes several arguments and return
 * a number. \(T^{arity} \to T\) Represented by Matrix A, Vector b and constant
 * c: \(f(x) = \frac{1}{2}Ax^2 + bx + c\)
 * @tparam T The type of each element of argument {@link Vector}
 * @tparam Matrix The type of matrix to use. Maybe be common and m_diag
 */
template <typename T, typename Matrix = Matrix<T>>
class QuadFunction {
public:
    QuadFunction(Matrix A, Vector<T> b, T c)
        : m_A(A), m_b(b), m_c(c) {}

    /**
     * Call that function with giving arguments as {@link Vector}
     * @param The arguments
     */
    T call(Vector<T> args) const {
        return (m_A * args) * args * static_cast<T>(0.5) + m_b * args + m_c;
    }

    /**
     * Calculate gradient in giving point
     * @param args point
     */
    Vector<T> grad(Vector<T> args) const { return (m_A * args) + m_b; }

    /**
     * Creates a single argument function: \(\alpha - \nabla f(\alpha) * x\)
     * @param point \(\alpha\)
     */
    Function<T>* to_single(Vector<T> point) {
        return new StdFunction<T>([=](T x) {
            return this->call(point - this->grad(point) * x);
        });
    }

    T operator()(Vector<T> args) { return call(args); }

    int arity() { return m_b.size(); }

// private:
    Matrix m_A;
    Vector<T> m_b;
    T m_c;
};
