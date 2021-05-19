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
    virtual size_t size() = 0;

    friend std::ostream& operator<<(std::ostream& o, Matrix<T>& matrix) {
        for(int i = 0; i < matrix.size(); ++i) {
            for(int j = 0; j < matrix.size(); ++j) {
                o << matrix.get(i, j) << " ";
            }
            o << std::endl;
        }
        return o;
    }
};

template<typename T>
class ProfileMatrix : public Matrix<T> {
public:
    ProfileMatrix() = delete;
    ProfileMatrix(ProfileMatrix<T>&) = delete;
    ProfileMatrix(ProfileMatrix<T>&& other) {
        this->swap(std::move(other));
    }

    ProfileMatrix(std::istream& in) {
        size_t n; in >> n;
        std::vector<T>& diag = m_diag;
        diag.resize(n);
        for(int i = 0; i < n; ++i) {
            in >> diag[i];
        }
        std::vector<size_t>& ia = m_ia;
        ia.resize(n + 1);
        for(int i = 0; i < n + 1; ++i) {
            in >> ia[i]; ia[i]--;
        }
        size_t size = ia[n];
        std::vector<T>& al = m_al;
        std::vector<T>& au = m_au;
        al.resize(size);
        au.resize(size);
        for(int i = 0; i < size; ++i) {
            in >> al[i];
        }
        for(int i = 0; i < size; ++i) {
            in >> au[i];
        }
    }

    /**
     * Get element by row and column
     * @param i row
     * @param j column
     */
    T& get(size_t i, size_t j) override {
        m_zero = 0;
        if(i == j) {
            return m_diag[i];
        } else if(i > j) {
            int start = m_ia[i];
            int size = m_ia[i + 1] - m_ia[i];
            if(i - j > size) {
                return m_zero;
            } else {
                return m_al[start + (size - i + j)];
            }
        } else {
            int start = m_ia[j];
            int size = m_ia[j + 1] - m_ia[j];
            if(j - i > size) {
                return m_zero;
            } else {
                return m_au[start + (size - j + i)];
            }
        }
        return m_zero;
    }

    size_t size() override {
        return m_diag.size();
    }

private:

    // ProfileMatrix(std::vector<T>&& diag, std::vector<size_t>&& ia, std::vector<T>&& al, std::vector<T>&& au) {
    //     m_diag = std::move(diag);
    //     m_ia   = std::move(ia);
    //     m_al   = std::move(al);
    //     m_au   = std::move(au);
    // }

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
                for (int k = 0; k < j; ++k) {
                    sum += getInL(i, k) * getInU(k, i);
                }
                m_profile.get(i, i) = m_profile.get(i, i) - sum;
            }
        }
        std::cout << "Hello from LUDecomposition" << std::endl;
        // TODO
    }

    T& getInL(size_t i, size_t j) override {
        if (i < j) {
            throw std::invalid_argument("L matrix:  i >= j");
        }
        return m_profile.get(i, j);
    }

    T& getInU(size_t i, size_t j) override {
        if (i > j) {
            throw std::invalid_argument("U matrix:  i <= j");
        } else if (i == j) {
            return m_one;
        }
        return m_profile.get(i, j);
    }

    size_t size() override {
        return m_profile.size();
    }
private:

    void swap(LUDecomposition<T>&& other) {
        std::swap(m_profile, other.m_profile);
    }
    ProfileMatrix<T> m_profile;
    T m_zero = 0;
    T m_one = 1;
};
