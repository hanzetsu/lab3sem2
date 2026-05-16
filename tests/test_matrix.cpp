#include "Matrix.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "exceptions.hpp"
#include "test_common.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

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

int main() {
    try {
        if (testMatrixAddition())
            std::cout << "Успешно все тесты пройдены" << std::endl;
        else
            std::cerr << "Тесты не пройдены" << std::endl;
    } catch (const IException& e) {
        std::cerr << "Исключение IException: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Исключение std::exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Неизвестное исключение" << std::endl;
    }
    return 0;
}
