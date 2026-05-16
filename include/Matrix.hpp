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
};