#include "Sequence.hpp"
#include "concepts.hpp"
#include <iostream>
template <typename T, template <typename> class Container>
 requires SequenceLike<Container<T>>
class Matrix
{
private:
    Container<Container<T>> data;
    size_t rows;
    size_t columns;
    void swapRows(std::size_t r1, std::size_t r2) {
        if (r1 == r2) return;
        auto row1 = data.Get(r1);
        auto row2 = data.Get(r2);
        data.Set(r1, row2);
        data.Set(r2, row1);
    }
public:
    Matrix() = delete;
    Matrix(const Container<Container<T>> &Data) : data(Data), rows(Data.GetLength()), columns(rows > 0 ? data.Get(0).GetLength() : 0)
    {
        for (size_t i = 0; i < rows; ++i)
            if (data.Get(i).GetLength() != columns)
                throw InvalidArgument("Matrix: строки разной длины");
    }
    Matrix(std::size_t rows, std::size_t cols, T initValue = T{})
        : data(rows), rows(rows), columns(cols)
    {

        for (std::size_t i = 0; i < rows; ++i)
        {
            Container<T> row(cols);

            for (std::size_t j = 0; j < cols; ++j)
                row.Set(j, initValue);

            data.Set(i, row);
        }
    }
    Matrix(const Matrix &) = default;

    T Get(size_t row, size_t col) const
    {
        if (col >= columns)
            throw IndexOutOfRange(col, rows * columns, "Matrix::Get: индекс столбца выходит за матрицу");
        if (row >= rows)
            throw IndexOutOfRange(row, rows * columns, "Matrix::Get: индекс ряда выходит за матрицу");
        return data.Get(row).Get(col);
    }

    void Set(size_t row, size_t col, T value)
    {
        if (col >= columns)
            throw IndexOutOfRange(col, rows * columns, "");

        if (row >= rows)
            throw IndexOutOfRange(row, rows * columns, "");

        data.Get(row).Set(col, value);
    }
    size_t getRows() const { return rows; }
    size_t getCols() const { return columns; }
    T operator()(std::size_t row, std::size_t col) const { return Get(row, col); }
    Matrix operator+(const Matrix &other) const
    {
        if (rows != other.rows || columns != other.columns)
            throw InvalidArgument("Matrix::+: размеры не совпадают");
        Matrix result(rows, columns);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                result.Set(i, j, Get(i, j) + other.Get(i, j));
        return result;
    }
        Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols)
            throw InvalidArgument("Matrix::-: размеры не совпадают");
        Matrix result(rows, cols);
        for (std::size_t i = 0; i < rows; ++i)
            for (std::size_t j = 0; j < cols; ++j)
                result.set(i, j, get(i, j) - other.get(i, j));
        return result;
    }
        Matrix operator*(T scalar) const {
        Matrix result(rows, cols);
        for (std::size_t i = 0; i < rows; ++i)
            for (std::size_t j = 0; j < cols; ++j)
                result.set(i, j, get(i, j) * scalar);
        return result;
    }
     Matrix operator*(const Matrix& other) const {
        if (cols != other.rows)
            throw InvalidArgument("Matrix::*: количество столбцов первой не равно количеству строк второй");
        Matrix result(rows, other.cols, T(0));
        for (std::size_t i = 0; i < rows; ++i)
            for (std::size_t j = 0; j < other.cols; ++j) {
                T sum = T(0);
                for (std::size_t k = 0; k < cols; ++k)
                    sum += get(i, k) * other.get(k, j);
                result.set(i, j, sum);
            }
        return result;
    }
        Matrix transpose() const {
        Matrix result(cols, rows);
        for (std::size_t i = 0; i < rows; ++i)
            for (std::size_t j = 0; j < cols; ++j)
                result.set(j, i, get(i, j));
        return result;
        }
            T determinant() const {
        if (rows != cols)
            throw InvalidArgument("Matrix::determinant: матрица не квадратная");
        Matrix temp = *this;
        T det = T(1);
        const std::size_t n = rows;

        for (std::size_t i = 0; i < n; ++i) {
            std::size_t pivotRow = i;
            for (std::size_t k = i + 1; k < n; ++k) {
                if (std::abs(temp.get(k, i)) > std::abs(temp.get(pivotRow, i)))
                    pivotRow = k;
            }
            if (std::abs(temp.get(pivotRow, i)) < T(1e-12))
                return T(0);

            if (pivotRow != i) {
                temp.swapRows(i, pivotRow);
                det = -det;
            }

            det *= temp.get(i, i);

            for (std::size_t k = i + 1; k < n; ++k) {
                T factor = temp.get(k, i) / temp.get(i, i);
                for (std::size_t j = i; j < n; ++j) {
                    T newVal = temp.get(k, j) - factor * temp.get(i, j);
                    temp.set(k, j, newVal);
                }
            }
        }
        return det;
    }
        Matrix inverse() const {
        if (rows != cols)
            throw InvalidArgument("Matrix::inverse: матрица не квадратная");
        std::size_t n = rows;
        Matrix augmented(n, 2 * n);
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < n; ++j)
                augmented.set(i, j, get(i, j));
            augmented.set(i, n + i, T(1));
        }
        for (std::size_t i = 0; i < n; ++i) {
            std::size_t pivotRow = i;
            for (std::size_t k = i + 1; k < n; ++k)
                if (std::abs(augmented.get(k, i)) > std::abs(augmented.get(pivotRow, i)))
                    pivotRow = k;
            if (std::abs(augmented.get(pivotRow, i)) < T(1e-12))
                throw InvalidArgument("Matrix::inverse: матрица вырождена");
            if (pivotRow != i)
                augmented.swapRows(i, pivotRow);

            T diag = augmented.get(i, i);
            for (std::size_t j = i; j < 2 * n; ++j)
                augmented.set(i, j, augmented.get(i, j) / diag);
            for (std::size_t k = 0; k < n; ++k) {
                if (k != i) {
                    T factor = augmented.get(k, i);
                    for (std::size_t j = i; j < 2 * n; ++j)
                        augmented.set(k, j, augmented.get(k, j) - factor * augmented.get(i, j));
                }
            }
        }
        Matrix result(n, n);
        for (std::size_t i = 0; i < n; ++i)
            for (std::size_t j = 0; j < n; ++j)
                result.set(i, j, augmented.get(i, n + j));
        return result;
    }
};