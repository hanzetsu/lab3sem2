#include "Sequence.hpp"

template <typename T, template <typename> class Container> //добавить концепты
class Matrix {
    private:
    Container<Container<T>> data;
    size_t rows;
    size_t columns;
    public:
    Matrix() = delete;
    Matrix(Container<Container<T>> Data) : data (Data), rows(Data.GetLength()), columns(rows > 0 ? data.Get(0).GetSize() : 0){}
};