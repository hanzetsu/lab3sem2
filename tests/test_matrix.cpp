#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using Mat = Matrix<double, MutableArraySequence>;

bool matrixEqual(const Mat& a, const Mat& b, double eps = 1e-9) {
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        return false;
    for (size_t i = 0; i < a.getRows(); ++i)
        for (size_t j = 0; j < a.getCols(); ++j)
            if (std::fabs(a.Get(i, j) - b.Get(i, j)) > eps)
                return false;
    return true;
}

void testMatrixAddition() {
    std::cout << "Тест сложения матриц..." << std::endl;
    Mat a(2, 2, 0.0);
    a.Set(0, 0, 1); a.Set(0, 1, 2);
    a.Set(1, 0, 3); a.Set(1, 1, 4);
    Mat b(2, 2, 0.0);
    b.Set(0, 0, 5); b.Set(0, 1, 6);
    b.Set(1, 0, 7); b.Set(1, 1, 8);
    Mat expected(2, 2, 0.0);
    expected.Set(0, 0, 6); expected.Set(0, 1, 8);
    expected.Set(1, 0, 10); expected.Set(1, 1, 12);
    Mat result = a + b;
    assert(matrixEqual(result, expected));
    std::cout << "  OK" << std::endl;
}

void testMatrixSubtraction() {
    std::cout << "Тест вычитания матриц..." << std::endl;
    Mat a(2, 2, 0.0);
    a.Set(0,0,5); a.Set(0,1,6);
    a.Set(1,0,7); a.Set(1,1,8);
    Mat b(2,2,0.0);
    b.Set(0,0,1); b.Set(0,1,2);
    b.Set(1,0,3); b.Set(1,1,4);
    Mat expected(2,2,0.0);
    expected.Set(0,0,4); expected.Set(0,1,4);
    expected.Set(1,0,4); expected.Set(1,1,4);
    Mat result = a - b;
    assert(matrixEqual(result, expected));
    std::cout << "  OK" << std::endl;
}

void testScalarMultiplication() {
    std::cout << "Тест умножения на скаляр..." << std::endl;
    Mat a(2,2,0.0);
    a.Set(0,0,1); a.Set(0,1,2);
    a.Set(1,0,3); a.Set(1,1,4);
    double scalar = 2.0;
    Mat expected(2,2,0.0);
    expected.Set(0,0,2); expected.Set(0,1,4);
    expected.Set(1,0,6); expected.Set(1,1,8);
    Mat result = a * scalar;
    assert(matrixEqual(result, expected));
    std::cout << "  OK" << std::endl;
}

int main() {
    try {
        testMatrixAddition();
        testMatrixSubtraction();
        testScalarMultiplication();
        std::cout << "\nВсе тесты матрицы пройдены успешно!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}