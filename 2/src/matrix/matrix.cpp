#include "matrix.h"

#include <sstream>
#include <stdexcept>
#include <utility>


namespace {

size_t getLog2Dim(const Matrix& a) {
    size_t sz = std::max(a.getRowsSize(), a.getColumnsSize());
    size_t ans = 1;
    while (ans < sz) {
        ans *= 2;
    }
    return ans;
}

std::vector<std::vector<MatrixView>> split(const Matrix& a) {
    const size_t n = a.getRowsSize() / 2;
    std::vector<std::vector<MatrixView>> ans(2, std::vector<MatrixView>(2, MatrixView(a)));
    for (size_t m_i = 0; m_i < 2; ++m_i) {
        for (size_t m_j = 0; m_j < 2; ++m_j) {
            ans[m_i][m_j].reView(m_i * n, m_j * n, n, n);
        }
    }
    return ans;
}

Matrix matMul(const Matrix& a, const Matrix& b) {
    size_t n = a.getRowsSize();
    Matrix ans(n, n, 0);
    if (n == 1) {
        ans[0][0] = a[0][0] * b[0][0];
    } else {
        const auto a_splited = split(a);
        const auto b_splited = split(b);

        std::vector<Matrix> p(7);
        p[0] = matMul(a_splited[0][0] + a_splited[1][1], b_splited[0][0] + b_splited[1][1]);
        p[1] = matMul(a_splited[1][0] + a_splited[1][1], b_splited[0][0].createMatrix());
        p[2] = matMul(a_splited[0][0].createMatrix(), b_splited[0][1] - b_splited[1][1]);
        p[3] = matMul(a_splited[1][1].createMatrix(), b_splited[1][0] - b_splited[0][0]);
        p[4] = matMul(a_splited[0][0] + a_splited[0][1], b_splited[1][1].createMatrix());
        p[5] = matMul(a_splited[1][0] - a_splited[0][0], b_splited[0][0] + b_splited[0][1]);
        p[6] = matMul(a_splited[0][1] - a_splited[1][1], b_splited[1][0] + b_splited[1][1]);

        std::vector<std::vector<Matrix>> c(2, std::vector<Matrix>(2));
        c[0][0] = p[0] + p[3] - p[4] + p[6];
        c[0][1] = p[2] + p[4];
        c[1][0] = p[1] + p[3];
        c[1][1] = p[0] - p[1] + p[2] + p[5];

        n /= 2;
        for (size_t m_i = 0; m_i < 2; ++m_i) {
            for (size_t m_j = 0; m_j < 2; ++m_j) {
                for (size_t i = 0; i < n; ++i) {
                    for (size_t j = 0; j < n; ++j) {
                        ans[i + m_i * n][j + m_j * n] = c[m_i][m_j][i][j];
                    }
                }
            }
        }
    }
    return ans;

}

} // anonymous namespace

Matrix::Row::Row(const size_t n, const size_t value)
    : row_(std::vector<int64_t>(n, value))
{
}

Matrix::Row::Row(std::initializer_list<int64_t> init)
    : row_(std::move(init))
{
}

std::vector<int64_t>::iterator Matrix::Row::begin() {
    return row_.begin();
}

std::vector<int64_t>::iterator Matrix::Row::end() {
    return row_.end();
}

std::vector<int64_t>::const_iterator Matrix::Row::begin() const {
    return row_.cbegin();
}

std::vector<int64_t>::const_iterator Matrix::Row::end() const {
    return row_.cend();
}

size_t Matrix::Row::size() const {
    return row_.size();
}

int64_t& Matrix::Row::operator [](const size_t id) {
    return row_.at(id);
}

int64_t Matrix::Row::operator [](const size_t id) const {
    return row_.at(id);
}

Matrix::Matrix(const std::vector<Matrix::Row>& matrix)
    : matrix_(matrix)
{
}

Matrix::Matrix(std::vector<Matrix::Row>&& matrix)
    : matrix_(std::move(matrix))
{
}

Matrix::Matrix(const size_t rows, const size_t columns, const int64_t value)
    : matrix_(std::vector<Row>(rows, Row(columns, value)))
{
}

Matrix::Matrix(std::initializer_list<Row> init)
    : matrix_(std::move(init))
{
}

std::vector<Matrix::Row>::iterator Matrix::begin() {
    return matrix_.begin();
}

std::vector<Matrix::Row>::iterator Matrix::end() {
    return matrix_.end();
}

size_t Matrix::getRowsSize() const {
    return matrix_.size();
}

size_t Matrix::getColumnsSize() const {
    return matrix_.empty() ? 0 : matrix_[0].size();
}

Matrix Matrix::reshape(const size_t rows, const size_t columns) const {
    Matrix ans(rows, columns, 0);
    for (size_t i = 0; i < std::min(rows, getRowsSize()); ++i) {
        for (size_t j = 0; j < std::min(columns, getColumnsSize()); ++j) {
            ans[i][j] = (*this)[i][j];
        }
    }
    return ans;
}

Matrix::Row& Matrix::operator [](const size_t id) {
    return matrix_.at(id);
}

const Matrix::Row& Matrix::operator [](const size_t id) const {
    return matrix_.at(id);
}

Matrix& Matrix::operator *= (const int64_t value) {
    for (auto& row : matrix_) {
        for (auto& item : row) {
            item *= value;
        }
    }
    return *this;
}

bool operator ==(const Matrix& a, const Matrix& b) {
    if (a.getRowsSize() != b.getRowsSize() || a.getColumnsSize() != b.getColumnsSize()) {
        return false;
    }

    for (size_t i = 0; i < a.getRowsSize(); ++i) {
        for (size_t j = 0; j < a.getColumnsSize(); ++j) {
            if (a[i][j] != b[i][j]) {
                return false;
            }
        }
    }
    return true;
}

Matrix operator +(const Matrix& a, const Matrix& b) {
    if (a.getRowsSize() != b.getRowsSize() || a.getColumnsSize() != b.getColumnsSize()) {
        throw std::runtime_error("Invalid matrix shapes: should be equal for A + B.");
    }

    Matrix c(a.getRowsSize(), a.getColumnsSize(), 0);
    for (size_t i = 0; i < a.getRowsSize(); ++i) {
        for (size_t j = 0; j < a.getColumnsSize(); ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    return c;
}

Matrix operator -(const Matrix& a, const Matrix& b) {
    if (a.getRowsSize() != b.getRowsSize() || a.getColumnsSize() != b.getColumnsSize()) {
        throw std::runtime_error("Invalid matrix shapes: should be equal for A - B.");
    }

    Matrix c(a.getRowsSize(), a.getColumnsSize(), 0);
    for (size_t i = 0; i < a.getRowsSize(); ++i) {
        for (size_t j = 0; j < a.getColumnsSize(); ++j) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
    return c;
}

Matrix operator *(const Matrix& a, const Matrix& b) {
    if (a.getColumnsSize() != b.getRowsSize()) {
        throw std::runtime_error("Invalid matrix shapes: A.columns should be equal B.rows in A * B.");
    }

    size_t cur_sz = std::max(getLog2Dim(a), getLog2Dim(b));
    Matrix cur_a = a.reshape(cur_sz, cur_sz);
    Matrix cur_b = b.reshape(cur_sz, cur_sz);
    Matrix c = matMul(cur_a, cur_b);
    return c.reshape(a.getRowsSize(), b.getColumnsSize());
}

Matrix slowMatMul(const Matrix& a, const Matrix& b) {
    if (a.getColumnsSize() != b.getRowsSize()) {
        throw std::runtime_error("Invalid matrix shapes: A.columns should be equal B.rows in A * B.");
    }

    Matrix c(a.getRowsSize(), b.getColumnsSize(), 0);
    for (size_t i = 0; i < a.getRowsSize(); i++) {
        for (size_t j = 0; j < b.getColumnsSize(); j++) {
	        for (size_t k = 0; k < a.getColumnsSize(); k++) {
	            c[i][j] += a[i][k] * b[k][j];
	        }
        }
    }
    return c;
}

std::ostream& operator << (std::ostream& os, const Matrix& m) {
    for (size_t i = 0; i < m.getRowsSize(); ++i) {
        for (size_t j = 0; j < m.getColumnsSize(); ++j) {
            os << m[i][j] << " ";
        }
        os << "\n";
    }
    return os;
}



MatrixView::MatrixView(const Matrix& m)
    : m_(m)
{
}

MatrixView::MatrixView(const Matrix& m, const size_t r_pos, const size_t c_pos, const size_t r, const size_t c)
    : m_(m)
    , r_pos_(r_pos)
    , c_pos_(c_pos)
    , r_(r)
    , c_(c)
{
}

size_t MatrixView::getRowsSize() const {
    return r_;
}

size_t MatrixView::getColumnsSize() const {
    return c_;
}

Matrix MatrixView::createMatrix() const {
    Matrix ans(r_, c_, 0);
    for (size_t i = 0; i < r_; ++i) {
        for (size_t j = 0; j < c_; ++j) {
            ans[i][j] = m_[i + r_pos_][j + c_pos_];
        }
    }
    return ans;
}

void MatrixView::reView(const size_t r_pos, const size_t c_pos, const size_t r, const size_t c) {
    r_pos_ = r_pos;
    c_pos_ = c_pos;
    r_ = r;
    c_ = c;
}

Matrix operator +(const MatrixView& a, const MatrixView& b) {
    if (a.getRowsSize() != b.getRowsSize() || a.getColumnsSize() != b.getColumnsSize()) {
        throw std::runtime_error("Invalid matrix shapes: should be equal for A + B.");
    }

    Matrix c(a.getRowsSize(), a.getColumnsSize(), 0);
    for (size_t i = 0; i < a.getRowsSize(); ++i) {
        for (size_t j = 0; j < a.getColumnsSize(); ++j) {
            c[i][j] = a.m_[a.r_pos_ + i][a.c_pos_ + j] + b.m_[b.r_pos_ + i][b.c_pos_ + j];
        }
    }
    return c;
}

Matrix operator -(const MatrixView& a, const MatrixView& b) {
    if (a.getRowsSize() != b.getRowsSize() || a.getColumnsSize() != b.getColumnsSize()) {
        throw std::runtime_error("Invalid matrix shapes: should be equal for A - B.");
    }

    Matrix c(a.getRowsSize(), a.getColumnsSize(), 0);
    for (size_t i = 0; i < a.getRowsSize(); ++i) {
        for (size_t j = 0; j < a.getColumnsSize(); ++j) {
            c[i][j] = a.m_[a.r_pos_ + i][a.c_pos_ + j] - b.m_[b.r_pos_ + i][b.c_pos_ + j];
        }
    }
    return c;
}
