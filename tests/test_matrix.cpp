#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "exceptions.hpp"
#include "test_common.hpp"
#include "DynamicArray.hpp"
#include <ranges>
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

bool testMatrixAddition() {
    std::cout << "  Проверка сложения матриц..." << std::endl;
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
    TEST_ASSERT(matrixEqual(result, expected), "сложение матриц неверно");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMatrixSubtraction() {
    std::cout << "  Проверка вычитания матриц..." << std::endl;
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
    TEST_ASSERT(matrixEqual(result, expected), "вычитание матриц неверно");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testScalarMultiplication() {
    std::cout << "  Проверка умножения на скаляр..." << std::endl;
    Mat a(2,2,0.0);
    a.Set(0,0,1); a.Set(0,1,2);
    a.Set(1,0,3); a.Set(1,1,4);
    double scalar = 2.0;
    Mat expected(2,2,0.0);
    expected.Set(0,0,2); expected.Set(0,1,4);
    expected.Set(1,0,6); expected.Set(1,1,8);
    Mat result = a * scalar;
    TEST_ASSERT(matrixEqual(result, expected), "умножение на скаляр неверно");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMatrixMultiplication() {
    std::cout << "  Проверка умножения матриц..." << std::endl;
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
    TEST_ASSERT(matrixEqual(result, expected), "умножение матриц неверно");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testDeterminant() {
    std::cout << "  Проверка определителя..." << std::endl;
    Mat a(2,2,0.0);
    a.Set(0,0,1); a.Set(0,1,2);
    a.Set(1,0,3); a.Set(1,1,4);
    double det = a.determinant();
    TEST_ASSERT(std::fabs(det + 2.0) < 1e-9, "определитель 2x2 неверен");
    std::cout << "    det(2x2) = " << det << std::endl;
    Mat b(3,3,0.0);
    b.Set(0,0,2); b.Set(0,1,-1); b.Set(0,2,0);
    b.Set(1,0,-1); b.Set(1,1,2); b.Set(1,2,-1);
    b.Set(2,0,0); b.Set(2,1,-1); b.Set(2,2,2);
    det = b.determinant();
    TEST_ASSERT(std::fabs(det - 4.0) < 1e-9, "определитель 3x3 неверен");
    std::cout << "    det(3x3) = " << det << std::endl;
    Mat c(2,2,0.0);
    c.Set(0,0,1); c.Set(0,1,2);
    c.Set(1,0,2); c.Set(1,1,4);
    det = c.determinant();
    TEST_ASSERT(std::fabs(det) < 1e-9, "определитель вырожденной не 0");
    std::cout << "    det(вырожденная) = 0" << std::endl;
    return true;
}

bool testInverse() {
    std::cout << "  Проверка обратной матрицы..." << std::endl;
    Mat a(2,2,0.0);
    a.Set(0,0,4); a.Set(0,1,7);
    a.Set(1,0,2); a.Set(1,1,6);
    Mat inv = a.inverse();
    Mat expected(2,2,0.0);
    expected.Set(0,0,0.6); expected.Set(0,1,-0.7);
    expected.Set(1,0,-0.2); expected.Set(1,1,0.4);
    TEST_ASSERT(matrixEqual(inv, expected, 1e-9), "обратная матрица неверна");
    Mat prod = a * inv;
    Mat ident(2,2,0.0);
    ident.Set(0,0,1); ident.Set(0,1,0);
    ident.Set(1,0,0); ident.Set(1,1,1);
    TEST_ASSERT(matrixEqual(prod, ident, 1e-9), "A * A⁻¹ не равно единичной");
    Mat c(2,2,0.0);
    c.Set(0,0,1); c.Set(0,1,2);
    c.Set(1,0,2); c.Set(1,1,4);
    try {
        c.inverse();
        TEST_ASSERT(false, "не выброшено исключение для вырожденной матрицы");
    } catch (const InvalidArgument&) {}
    std::cout << "    OK" << std::endl;
    return true;
}

bool testTranspose() {
    std::cout << "  Проверка транспонирования..." << std::endl;
    Mat a(2,3,0.0);
    a.Set(0,0,1); a.Set(0,1,2); a.Set(0,2,3);
    a.Set(1,0,4); a.Set(1,1,5); a.Set(1,2,6);
    Mat t = a.transpose();
    TEST_ASSERT(t.getRows() == 3 && t.getCols() == 2, "размеры транспонированной матрицы неверны");
    TEST_ASSERT(t.Get(0,0) == 1 && t.Get(0,1) == 4, "элемент (0,0)");
    TEST_ASSERT(t.Get(1,0) == 2 && t.Get(1,1) == 5, "элемент (1,0)");
    TEST_ASSERT(t.Get(2,0) == 3 && t.Get(2,1) == 6, "элемент (2,0)");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testExceptions() {
    std::cout << "  Проверка исключений при несовместимых размерах..." << std::endl;
    Mat a(2,2);
    Mat b(3,3);
    try {
        Mat c = a + b;
        TEST_ASSERT(false, "не выброшено исключение при сложении");
    } catch (const InvalidArgument&) {}
    try {
        Mat c = a - b;
        TEST_ASSERT(false, "не выброшено исключение при вычитании");
    } catch (const InvalidArgument&) {}
    try {
        Mat c = a * b;
        TEST_ASSERT(false, "не выброшено исключение при умножении");
    } catch (const InvalidArgument&) {}
    try {
        a.determinant();
    } catch (...) { TEST_ASSERT(false, "исключение при вычислении определителя квадратной матрицы"); }
    try {
        Mat(2,3).determinant();
        TEST_ASSERT(false, "не выброшено исключение для неквадратной матрицы");
    } catch (const InvalidArgument&) {}
    std::cout << "    OK" << std::endl;
    return true;
}

bool testMatrix() {
    std::cout << "\n=== Тестирование Matrix ===" << std::endl;
    auto tests = DynamicArray{
        testMatrixAddition,
        testMatrixSubtraction,
        testScalarMultiplication,
        testMatrixMultiplication,
        testDeterminant,
        testInverse,
        testTranspose,
        testExceptions
    };
    return std::ranges::all_of(tests, [](auto f) { return f(); });
}

int main() {
    try {
        if (testMatrix()) {
            std::cout << "\n=== Все тесты матрицы пройдены ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты матрицы не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}