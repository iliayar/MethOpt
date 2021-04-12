#pragma once

#include <type_traits>
#include <array>

/**
 * The class represents T^m value. Implements algebraic operations
 * @tparam The type of each component
 * @tparam Number of components
 */
template <typename T, int size>
class Vector {
public:
    /**
     * Creates and vector with all zeros
     */
    Vector() {
        for(int i = 0; i < size; ++i) {
            m_components[i] = 0;
        }
    }

    /**
     * Create a vector from giving list. If list size if less than {@link
     * #size}, the rest of components will be zero
     */
    Vector(std::initializer_list<T> list) {
       int i = 0;
       for(T e : list) {
           m_components[i++] = e;
       }
       for(; i < size; ++i) {
           m_components[i] = 0;
       }
    }

    friend Vector<T, size> operator+(Vector<T, size> rhs,
                                     Vector<T, size>& lhs) {
        Vector<T, size> res;
        for (int i = 0; i < size; ++i) {
            res[i] = rhs[i] + lhs[i];
        }
        return res;
    }

    T& get(int i) { return m_components[i]; }

    T& operator[](int i) { return get(i); }

private:

    T m_components[size];
};

/**
 * The interface of functions, that takes several arguments and return a number
 * @tparam The type of each element of argument {@link Vector}
 * @tparam The size of argument {@link Vector}
 */
template <typename T, int arity>
class VecFunction {
public:
    /**
     * Call that function with giving arguments as {@link Vector}
     * @param The arguments
     */
    virtual T call(Vector<T, arity> args) = 0;

    T operator()(Vector<T, arity> args) { return call(args); }
};
