#pragma once

#include <type_traits>
#include <experimental/type_traits>
#include <array>
#include <functional>
#include <iostream>

template <typename T>
T vector_of(T n);
template <typename T, int size, int... sizes>
auto vector_of(T n);

/**
 * The class represents \(T^{size}\) value. Implements algebraic operations
 * @tparam T The type of each component
 * @tparam size Number of components
 */
template <typename T, int size>
class Vector {
public:

    Vector() {}

    /**
     * Create the vector from variadic arguments. Checks if thay are same type
     * and check if the number of arguments mathes the template parameter {@link
     * #size}
     */
    template <typename... Args>
    Vector(Args... args) {
        static_assert(sizeof...(args) == size, "The number of arguments is not equals size of vector");
        static_assert(std::experimental::conjunction<std::is_same<Args, T>...>::value, "The arguments must be same type");
        int i = 0;
        for(T arg : { std::forward<T>(args)... }) {
            m_components[i++] = arg;
        }
    }

    Vector(T value) {
        for(int i = 0; i < size; ++i) {
            m_components[i] = value;
        }
    }

    Vector<T, size> operator+(const Vector<T, size>& lhs) const {
        Vector<T, size> res;
        for (int i = 0; i < size; ++i) {
            res[i] = this->get(i) + lhs[i];
        }
        return res;
    }

    Vector<T, size> operator-(const Vector<T, size>& lhs) const {
        return lhs * (-1) + *this;
    }

    /**
     * Multiply each component of vector by constant value
     * @param c constant value
     */
    template<typename C>
    Vector<T, size> operator *(C c) const {
        Vector<T, size> res = *this;
        for (int i = 0; i < size; ++i) {
            res[i] = res[i] * c;
        }
        return res;
    }

    // template<typename C>
    // friend Vector<T, size> operator *(C c, const Vector<T, size>& rhs) {
    //     return rhs * c;
    // }

    /**
     * Calculate the dot product of two vectors
     * @param other The second vector
     */
    T operator*(const Vector<T, size>& other) const {
        T res;
        for (int i = 0; i < size; ++i) {
            res += m_components[i] * other[i];
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& o, const Vector<T, size>& vec) {
        for(int i = 0; i < size; ++i) {
            o << vec[i] << " ";
        }
        return o;
    }

    T& get(int i) { return m_components[i]; }
    const T& get(int i) const { return m_components[i]; }

    T& operator[](int i) { return get(i); }
    const T& operator[](int i) const { return get(i); }

private:

    T m_components[size];
};

template <typename T>
T vector_of(T n) {
    return n;
}

/**
 * Creates the vector of elements type T and dimension equals to number of
 * {@code int} template arguments, each dimennsion has an appropriate size and
 * fill all elements with provided value
 * @param n The value, to fill vectors with
 */
template <typename T, int size, int... sizes>
auto vector_of(T n) {
    auto e = vector_of<T, sizes...>(n);
    return Vector<decltype(e),size>(e);
}

/**
 * The class represents matrix. Implements multiplication
 * @tparam T The type of each component
 * @tparat height The height of matrx
 * @tparam width The width of matrix. By default equals {@code heigth}
 */
template <typename T, int height, int width = height>
class Matrix : public Vector<Vector<T, width>, height> {
public:
    Matrix(Vector<Vector<T, width>, height> vec)
        : Vector<Vector<T, width>, height>(vec) {}

    template <int rwidth>
    Matrix<T, height, rwidth> operator*(const Matrix<T, width, rwidth>& rhs) const {
        Matrix<T, height, rwidth> res = vector_of<T, height, rwidth>(static_cast<T>(0));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < rwidth; ++j) {
                for (int k = 0; k < width; ++k) {
                    res[i][j] += this->get(i)[k] * rhs[k][j];
                }
            }
        }
        return res;
    }

    Matrix<T, width, height> transpose() const {
        Matrix<T, width, height> res = vector_of<T, width, height>(0);
        for(int i = 0; i < width; ++i) {
            for(int j = 0; j < height; ++j) {
                res[i][j] = this->get(j)[i];
            }
        }
        return res;
    }

    Vector<T, height> operator*(const Vector<T, width>& rhs) const {
        return (*this * (Matrix<int, 1, width>{Vector<Vector<T, width>, 1>(rhs)}.transpose())).transpose()[0];
    }

    friend std::ostream& operator<<(std::ostream& o, Matrix<T, height, width> m) {
        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                o << m[i][j] << " ";
            }
            o << std::endl;
        }
        return o;
    }

};

/**
 * The interface of quadratic functions, that takes several arguments and return a number. \(T^{arity} \to T\)
 * Represented by Matrix A, Vector b and constant c: \(f(x) = \frac{1}{2}Ax^2 + bx + c\)
 * @tparam T The type of each element of argument {@link Vector}
 * @tparam arity The number of arguments
 */
template <typename T, int arity>
class QuadFunction {
public:
    QuadFunction(Matrix<T, arity> A, Vector<T, arity> b, T c)
        : m_A(A), m_b(b), m_c(c) {}

    /**
     * Call that function with giving arguments as {@link Vector}
     * @param The arguments
     */
    T call(Vector<T, arity> args) {
        return m_A * args * args  * static_cast<T>(0.5) + m_b * args + m_c;
    }

    template <typename... Args>
    T call(Args... args) {
        static_assert(sizeof...(args) == arity, "The number of arguments is not equals arity of function");
        static_assert(std::experimental::conjunction<std::is_same<Args, T>...>::value, "The arguments must be same type");
        return call(Vector<T, arity>(args...));
    }

    template <typename... Args>
    Vector<T, arity> grad(Args... args) {
        static_assert(sizeof...(args) == arity, "The number of arguments is not equals arity of function");
        static_assert(std::experimental::conjunction<std::is_same<Args, T>...>::value, "The arguments must be same type");
        return grad(Vector<T, arity>(args...));
    }

    /**
     * Calculate gradient in giving point
     * @param args point
     */
    Vector<T, arity> grad(Vector<T, arity> args) {
        return (m_A * args) + m_b;
    }

    template <typename... Args>
    T operator()(Args... args) { return call(args...); }
    T operator()(Vector<T, arity> args) { return call(args); }

private:
    Matrix<T, arity> m_A;
    Vector<T, arity> m_b;
    T m_c;
};
