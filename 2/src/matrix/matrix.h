#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

class Matrix {
public :
    struct Row {
    private :
        std::vector<int64_t> row_;

    public :
        Row() = default;
        Row(const Row&) = default;
        Row(Row&&) = default;

        Row(const size_t n, const size_t value);
        Row(std::initializer_list<int64_t> init);

        std::vector<int64_t>::iterator begin();
        std::vector<int64_t>::iterator end();

        std::vector<int64_t>::const_iterator begin() const;
        std::vector<int64_t>::const_iterator end() const;

        size_t size() const;

        int64_t& operator [](const size_t id);
        int64_t operator [](const size_t id) const;
    };

public :
    Matrix() = default;
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;

    explicit Matrix(const std::vector<Row>& matrix);
    explicit Matrix(std::vector<Row>&& matrix);

    Matrix(const size_t rows, const size_t columns, const int64_t value);
    Matrix(std::initializer_list<Row> init);

    std::vector<Row>::iterator begin();
    std::vector<Row>::iterator end();

    size_t getRowsSize() const;
    size_t getColumnsSize() const;

    Matrix reshape(const size_t rows, const size_t columns) const;

    Row& operator [](const size_t id);
    const Row& operator [](const size_t id) const;

    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) = default;

    Matrix& operator *= (const int64_t value);

    friend bool operator ==(const Matrix& a, const Matrix& b);

    friend Matrix operator +(const Matrix& a, const Matrix& b);
    friend Matrix operator -(const Matrix& a, const Matrix& b);
    friend Matrix operator *(const Matrix& a, const Matrix& b);

    friend Matrix slowMatMul(const Matrix& a, const Matrix& b);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

private :
    std::vector<Row> matrix_;
};


class MatrixView {
public :
    MatrixView(const Matrix& m);
    MatrixView(const MatrixView&) = default;
    MatrixView(MatrixView&&) = default;

    explicit MatrixView(const Matrix& m, const size_t r_pos, const size_t c_pos, const size_t r, const size_t c);

    size_t getRowsSize() const;
    size_t getColumnsSize() const;

   Matrix createMatrix() const;
   void reView(const size_t r_pos, const size_t c_pos, const size_t r, const size_t c);

    MatrixView& operator=(const MatrixView&) = default;
    MatrixView& operator=(MatrixView&&) = default;

    friend Matrix operator +(const MatrixView& a, const MatrixView& b);
    friend Matrix operator -(const MatrixView& a, const MatrixView& b);

private :
    const Matrix& m_;
    size_t r_pos_ = 0;
    size_t c_pos_ = 0;
    size_t r_ = 0;
    size_t c_ = 0;
};