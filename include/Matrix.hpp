#include "Sequence.hpp"

template <typename T, template <typename> class Container> // добавить концепты
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
    Matrix(size_t rows, size_t cols, T initValue = T{})
        : rows(rows), columns(cols), data()
    {
        for (size_t i = 0; i < rows; ++i)
        {
            Container<T> row(cols);
            for (size_t j = 0; j < cols; ++j)
                row.Set(j, initValue);
            data.Set(i, row);
        }
    }
    T Get(size_t row, size_t col) const
    {
        if (row >= rows || col >= columns)
            throw IndexOutOfRange(row * columns + col, rows * columns, "Matrix::get");
        return data.Get(row).Get(col);
    }

    void Set(size_t row, size_t col, T value)
    {
        if (row >= rows || col >= columns)
            throw IndexOutOfRange(row * columns + col, rows * columns, "Matrix::set");
        auto rowSeq = data.Get(row);
        rowSeq.Set(col, value);
        data.Set(row, rowSeq);
    }
    size_t getRows() const { return rows; }
    size_t getCols() const { return columns; }
};