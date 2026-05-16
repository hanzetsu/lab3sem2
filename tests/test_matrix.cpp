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

void testMatrixMultiplication() {
    std::cout << "Тест умножения матриц..." << std::endl;
    Mat a(2,3,0.0);
    a.Set(0,0,1); a.Set(0,1,2); a.Set(0,2,3);
    a.Set(1,0,4); a.Set(1,1,5); a.Set(1,2,6);
    Mat b(3,2,0.0);
    b.Set(0,0,7); b.Set(0,1,8);
    b.Set(1,0,9); b.Set(1,1,10);
    b.Set(2,0,11); b.Set(2,1,12);
    Mat expected(2,2,0.0);
    expected.Set(0,0,1*7+2*9+3*11);
    expected.Set(0,1,1*8+2*10+3*12);
    expected.Set(1,0,4*7+5*9+6*11);
    expected.Set(1,1,4*8+5*10+6*12);
    Mat result = a * b;
    assert(matrixEqual(result, expected));
    std::cout << "  OK" << std::endl;
}

void testDeterminant() {
    std::cout << "Тест определителя..." << std::endl;
    Mat a(2,2,0.0);
    a.Set(0,0,1); a.Set(0,1,2);
    a.Set(1,0,3); a.Set(1,1,4);
    double det = a.determinant();
    assert(std::fabs(det + 2.0) < 1e-9); // 1*4 - 2*3 = -2
    std::cout << "  det(2x2) = " << det << " OK" << std::endl;

    Mat b(3,3,0.0);
    b.Set(0,0,2); b.Set(0,1,-1); b.Set(0,2,0);
    b.Set(1,0,-1); b.Set(1,1,2); b.Set(1,2,-1);
    b.Set(2,0,0); b.Set(2,1,-1); b.Set(2,2,2);
    det = b.determinant();
    assert(std::fabs(det - 4.0) < 1e-9);
    std::cout << "  det(3x3) = " << det << " OK" << std::endl;

    Mat c(2,2,0.0);
    c.Set(0,0,1); c.Set(0,1,2);
    c.Set(1,0,2); c.Set(1,1,4);
    det = c.determinant();
    assert(std::fabs(det) < 1e-9);
    std::cout << "  det(вырожденная) = 0 OK" << std::endl;
}

void testInverse() {
    std::cout << "Тест обратной матрицы..." << std::endl;
    Mat a(2,2,0.0);
    a.Set(0,0,4); a.Set(0,1,7);
    a.Set(1,0,2); a.Set(1,1,6);
    Mat inv = a.inverse();
    Mat expected(2,2,0.0);
    expected.Set(0,0,0.6); expected.Set(0,1,-0.7);
    expected.Set(1,0,-0.2); expected.Set(1,1,0.4);
    assert(matrixEqual(inv, expected, 1e-9));
    Mat prod = a * inv;
    Mat ident(2,2,0.0);
    ident.Set(0,0,1); ident.Set(0,1,0);
    ident.Set(1,0,0); ident.Set(1,1,1);
    assert(matrixEqual(prod, ident, 1e-9));
    std::cout << "  Обратная матрица для 2x2 OK" << std::endl;

    Mat c(2,2,0.0);
    c.Set(0,0,1); c.Set(0,1,2);
    c.Set(1,0,2); c.Set(1,1,4);
    try {
        c.inverse();
        assert(false && "Не выброшено исключение для вырожденной матрицы");
    } catch (const InvalidArgument& e) {
    }
    std::cout << "  Вырожденная матрица: исключение OK" << std::endl;
}

void testTranspose() {
    std::cout << "Тест транспонирования..." << std::endl;
    Mat a(2,3,0.0);
    a.Set(0,0,1); a.Set(0,1,2); a.Set(0,2,3);
    a.Set(1,0,4); a.Set(1,1,5); a.Set(1,2,6);
    Mat t = a.transpose();
    assert(t.getRows() == 3 && t.getCols() == 2);
    assert(t.Get(0,0) == 1 && t.Get(0,1) == 4);
    assert(t.Get(1,0) == 2 && t.Get(1,1) == 5);
    assert(t.Get(2,0) == 3 && t.Get(2,1) == 6);
    std::cout << "  OK" << std::endl;
}

void testExceptions() {
    std::cout << "Тест исключений при несовместимых размерах..." << std::endl;
    Mat a(2,2);
    Mat b(3,3);
    try {
        Mat c = a + b;
        assert(false);
    } catch (const InvalidArgument&) {}
    try {
        Mat c = a - b;
        assert(false);
    } catch (const InvalidArgument&) {}
    try {
        Mat c = a * b;
        assert(false);
    } catch (const InvalidArgument&) {}
    try {
        a.determinant();
    } catch (...) { assert(false); }
    try {
        Mat(2,3).determinant();
        assert(false);
    } catch (const InvalidArgument&) {}
    std::cout << "  OK" << std::endl;
}

int main() {
    try {
        testMatrixAddition();
        testMatrixSubtraction();
        testScalarMultiplication();
        testMatrixMultiplication();
        testDeterminant();
        testInverse();
        testTranspose();
        testExceptions();
        std::cout << "\nВсе тесты матрицы пройдены успешно!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}