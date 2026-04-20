#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace linalg {

template <typename T>
class Matrix {
public:
    Matrix();
    Matrix(std::size_t rows, std::size_t cols);
    Matrix(std::size_t rows, std::size_t cols, const T& value);

    T& operator()(std::size_t i, std::size_t j);
    const T& operator()(std::size_t i, std::size_t j) const;

    std::size_t rows() const;
    std::size_t cols() const;

    T* data();
    const T* data() const;

    void fill(const T& value);
    void fill_random();

private:
    std::size_t rows_{0};
    std::size_t cols_{0};
    std::vector<T> data_;
};

template <typename T>
Matrix<T>::Matrix() {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T>::Matrix(std::size_t /*rows*/, std::size_t /*cols*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
Matrix<T>::Matrix(std::size_t /*rows*/, std::size_t /*cols*/, const T& /*value*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
T& Matrix<T>::operator()(std::size_t /*i*/, std::size_t /*j*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
const T& Matrix<T>::operator()(std::size_t /*i*/, std::size_t /*j*/) const {
    throw std::runtime_error("not implemented");
}

template <typename T>
std::size_t Matrix<T>::rows() const {
    throw std::runtime_error("not implemented");
}

template <typename T>
std::size_t Matrix<T>::cols() const {
    throw std::runtime_error("not implemented");
}

template <typename T>
T* Matrix<T>::data() {
    throw std::runtime_error("not implemented");
}

template <typename T>
const T* Matrix<T>::data() const {
    throw std::runtime_error("not implemented");
}

template <typename T>
void Matrix<T>::fill(const T& /*value*/) {
    throw std::runtime_error("not implemented");
}

template <typename T>
void Matrix<T>::fill_random() {
    throw std::runtime_error("not implemented");
}

} // namespace linalg
