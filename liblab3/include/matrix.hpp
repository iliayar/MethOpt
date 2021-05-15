#pragma once

#include <vector>
#include <iostream>

/*
 * @tparam The numeric type
 */
template<typename T>
class Matrix {
public:

    /*
     * get element of {@code i} row and {@code j} column
     * @param i The row number
     * @param j The column number
     */
    virtual T& get(size_t i, size_t j) = 0;
};

template<typename T>
class ProfileMatrix : public Matrix<T> {
public:
    ProfileMatrix() = delete;
    ProfileMatrix(ProfileMatrix<T>&) = delete;
    ProfileMatrix(ProfileMatrix<T>&& other) {
        this->swap(std::move(other));
    }

    ProfileMatrix(std::istream& i) {
        site_t n; o >> n;
        std::vector<T> diag(n);
        for(int i = 0; i < n; ++i) {
            i >> diag[i];
        }
        std::vector<size_t> ia(n + 1);
        std::vector<size_t> ja(n + 1);
        for(int i = 0, i < n + 1; ++i) {
            i >> ia[i];
        }
        for(int i = 0; i < n + 1; ++i) {
            i >> ja[i];
        }
    }

    T& get(size_t i, size_t j) override {
        m_zero = 0;
        // TODO
        return m_zero;
    }

private:

    ProfileMatrix(std::vector<T>&& diag, std::vector<size_t>&& ia, std::vector<T>&& al, std::vector<T>&& au) {
        m_diag = std::move(diag);
        m_ia   = std::move(ia);
        m_al   = std::move(al);
        m_au   = std::move(au);
    }

    void swap(ProfileMatrix<T>&& other) {
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

template <typename T>
class LUDecomposition : public Matrix<T> {
public:
    LUDecomposition() = delete;
    LUDecomposition(LUDecomposition<T>&) = delete;
    LUDecomposition(LUDecomposition<T>&& other) {
        this->swap(std::move(other));
    }
    
    LUDecomposition(ProfileMatrix<T>&& profile) : m_profile(std::move(profile)) {
        std::cout << "Hello from LUDecomposition" << std::endl;
        // TODO
    }

    T& get(size_t i, size_t j) override {
        m_zero = 0;
        // TODO
        return m_zero;
    }
private:

    void swap(LUDecomposition<T>&& other) {
        std::swap(m_profile, other.m_profile);
    }
    ProfileMatrix<T> m_profile;
    T m_zero = 0;
};
