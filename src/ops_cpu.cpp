#include "linalg/ops.hpp"

#include <stdexcept>

namespace linalg {

template <typename T>
Matrix<T> matmul(const Matrix<T>& /*a*/, const Matrix<T>& /*b*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T> gemv(const Matrix<T>& /*a*/, const Matrix<T>& /*x*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T> add(const Matrix<T>& /*a*/, const Matrix<T>& /*b*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T> scale(const Matrix<T>& /*a*/, const T& /*alpha*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T> hadamard(const Matrix<T>& /*a*/, const Matrix<T>& /*b*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T> transpose(const Matrix<T>& /*a*/) {
    throw std::runtime_error("not implemented");
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
