#pragma once

#include "linalg/matrix.hpp"

namespace linalg {

template <typename T>
Matrix<T> matmul(const Matrix<T>& a, const Matrix<T>& b);

template <typename T>
Matrix<T> gemv(const Matrix<T>& a, const Matrix<T>& x);

template <typename T>
Matrix<T> add(const Matrix<T>& a, const Matrix<T>& b);

template <typename T>
Matrix<T> scale(const Matrix<T>& a, const T& alpha);

template <typename T>
Matrix<T> hadamard(const Matrix<T>& a, const Matrix<T>& b);

template <typename T>
Matrix<T> transpose(const Matrix<T>& a);

} // namespace linalg
