#include "linalg/ops.hpp"

#include <stdexcept>

namespace linalg {

    template <typename T>
    Matrix<T> matmul(const Matrix<T>& a, const Matrix<T>& b) {
        if (a.cols() != b.rows()) {
            throw std::invalid_argument("Column size of Matrix A does not match Row size of Matrix B");
        }

        Matrix<T> c(a.rows(), b.cols());
        for (std::size_t i = 0; i < a.rows(); i++) {
            for (std::size_t j = 0; j < b.cols(); j++) {
                for (std::size_t k = 0; k < a.cols(); k++) {
                    c(i, j) += a(i, k) * b(k, j);
                }
            }
        }

        return c;
    }

    template <typename T>
    Matrix<T> gemv(const Matrix<T>& a, const Matrix<T>& x) {
        if (a.cols() != x.rows()) {
            throw std::invalid_argument("The column size of A don't match the row size of X");
        }
        else if (x.cols() != 1) {
            throw std::invalid_argument("X is not a Nx1 it's NxM");
        }
        Matrix<T> c(a.rows(), x.cols());
       
        for (std::size_t j = 0; j < a.rows(); j++) {
            for (std::size_t k = 0; k < x.rows(); k++) {
                c(j, 0) += a(j, k) * x(k, 0);
            }
        }


        return c;
    }

    template <typename T>
    Matrix<T> add(const Matrix<T>& a, const Matrix<T>& b) {
        if (a.rows() != b.rows() || a.cols() != b.cols()) {
            throw std::invalid_argument("Dimesions are not the same");
        }

        Matrix<T> c(a.rows(), a.cols());
        T* cp = c.data();
        const T* ap = a.data();
        const T* bp = b.data();
        const std::size_t n = a.rows() * a.cols();

        for (std::size_t i = 0; i < n; i++) {
            cp[i] = ap[i] + bp[i];
        }

        return c;
    }

    template <typename T>
    Matrix<T> scale(const Matrix<T>& a, const T& alpha) {
        Matrix<T> c(a.rows(), a.cols());
        T* cp = c.data();
        const T* ap = a.data();
        const std::size_t n = a.rows() * a.cols();

        for (std::size_t i = 0; i < n; i++) {
            cp[i] = ap[i] * alpha;
        }
        
        return c;
    }

    template <typename T>
    Matrix<T> hadamard(const Matrix<T>& a, const Matrix<T>& b) {
        if (a.rows() != b.rows() || a.cols() != b.cols()) {
            throw std::invalid_argument("Dimesions are not the same");
        }
    
        Matrix<T> c(a.rows(), a.cols(), 0);
        T* cp = c.data();
        const T* ap = a.data();
        const T* bp = b.data();
        const std::size_t n = a.rows() * a.cols();

        for (std::size_t i = 0; i < n; i++) {
            cp[i] = ap[i] * bp[i];
        }

        return c;
    }

    template <typename T>
    Matrix<T> transpose(const Matrix<T>& a) {
        Matrix<T> c(a.cols(), a.rows());
        for (std::size_t i = 0; i < c.rows(); i++) {
            for (std::size_t j = 0; j < c.cols(); j++) {
                c(i, j) = a(j, i);
            }
        }

        return c;
    }

    template Matrix<float> matmul<float>(const Matrix<float>&, const Matrix<float>&);
    template Matrix<double> matmul<double>(const Matrix<double>&, const Matrix<double>&);
    template Matrix<float> gemv<float>(const Matrix<float>&, const Matrix<float>&);
    template Matrix<double> gemv<double>(const Matrix<double>&, const Matrix<double>&);
    template Matrix<float> add<float>(const Matrix<float>&, const Matrix<float>&);
    template Matrix<double> add<double>(const Matrix<double>&, const Matrix<double>&);
    template Matrix<float> scale<float>(const Matrix<float>&, const float&);
    template Matrix<double> scale<double>(const Matrix<double>&, const double&);
    template Matrix<float> hadamard<float>(const Matrix<float>&, const Matrix<float>&);
    template Matrix<double> hadamard<double>(const Matrix<double>&, const Matrix<double>&);
    template Matrix<float> transpose<float>(const Matrix<float>&);
    template Matrix<double> transpose<double>(const Matrix<double>&);

} // namespace linalg
