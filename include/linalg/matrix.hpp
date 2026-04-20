#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <random>
#include <algorithm>

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
    Matrix<T>::Matrix(): rows_(0), cols_(0), data_() {}

    template <typename T>
    Matrix<T>::Matrix(std::size_t rows, std::size_t cols): rows_(rows), cols_(cols), data_(rows * cols, 0){}

    template <typename T>
    Matrix<T>::Matrix(std::size_t rows, std::size_t cols, const T& value): rows_(rows), cols_(cols), data_(rows * cols, value) {}

    template <typename T>
    T& Matrix<T>::operator()(std::size_t i, std::size_t j) {
        return data_[i * cols_ + j];
    }

    template <typename T>
    const T& Matrix<T>::operator()(std::size_t i, std::size_t j) const {
        return data_[i * cols_ + j];
    }

    template <typename T>
    std::size_t Matrix<T>::rows() const {
        return rows_;
    }

    template <typename T>
    std::size_t Matrix<T>::cols() const {
        return cols_;
    }

    template <typename T>
    T* Matrix<T>::data() {
        return &data_;
    }

    template <typename T>
    const T* Matrix<T>::data() const {
        return &data_;
    }

    template <typename T>
    void Matrix<T>::fill(const T& value) {
        data_(rows_ * cols_, value);
    }

    /*template <typename T>
    void Matrix<T>::fill_random() {
        throw std::runtime_error("not implemented");
    }*/

} // namespace linalg
