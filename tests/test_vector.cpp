#include "Vector.hpp"
#include "test_common.hpp"
#include "DynamicArray.hpp"
#include <ranges>
#include <iostream>

bool testVectorConstruction() {
    std::cout << "  Проверка конструкторов" << std::endl;
    Vector<int> v1(3);
    TEST_ASSERT(v1.size() == 3, "Размер не 3");
    TEST_ASSERT(v1[0] == 0 && v1[1] == 0 && v1[2] == 0, "Нулевой вектор не обнулён");
    Vector<int> v2{1, 2, 3};
    TEST_ASSERT(v2.size() == 3, "Размер не 3");
    TEST_ASSERT(v2[0] == 1 && v2[1] == 2 && v2[2] == 3, "Конструктор из списка");
    Vector<int> v3(v2);
    TEST_ASSERT(v3[0] == 1 && v3[1] == 2 && v3[2] == 3, "Конструктор копирования");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testVectorSetGet() {
    std::cout << "  Проверка set и get" << std::endl;
    Vector<double> v(2);
    v.set(0, 3.5);
    v.set(1, 4.5);
    TEST_ASSERT(v.get(0) == 3.5 && v.get(1) == 4.5, "set/get не работают");
    try {
        v.set(2, 0);
        TEST_ASSERT(false, "Исключение не выброшено");
    } catch (const IndexOutOfRange& e) {
        std::cout << "    Исключение перехвачено: " << e.what() << std::endl;
    }
    std::cout << "    OK" << std::endl;
    return true;
}

bool testVectorArithmetic() {
    std::cout << "  Проверка арифметических операций" << std::endl;
    Vector<int> a{1, 2, 3};
    Vector<int> b{4, 5, 6};
    Vector<int> sum = a + b;
    TEST_ASSERT(sum[0] == 5 && sum[1] == 7 && sum[2] == 9, "Сложение");
    Vector<int> diff = a - b;
    TEST_ASSERT(diff[0] == -3 && diff[1] == -3 && diff[2] == -3, "Вычитание");
    Vector<int> mul = a * 2;
    TEST_ASSERT(mul[0] == 2 && mul[1] == 4 && mul[2] == 6, "Умножение на скаляр");
    int dot = a.dot(b);
    TEST_ASSERT(dot == 32, "Скалярное произведение (1*4+2*5+3*6=32)");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testVectorLengthNormalize() {
    std::cout << "  Проверка длины и нормализации" << std::endl;
    Vector<double> v{3.0, 4.0};
    double len = v.length();
    TEST_ASSERT(std::abs(len - 5.0) < 1e-9, "Длина не 5");
    Vector<double> norm = v.normalized();
    TEST_ASSERT(std::abs(norm[0] - 0.6) < 1e-9 && std::abs(norm[1] - 0.8) < 1e-9, "Нормализация");
    Vector<double> zero(2);
    Vector<double> normZero = zero.normalized();
    TEST_ASSERT(normZero[0] == 0.0 && normZero[1] == 0.0, "Нормализация нуля");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testVectorComparison() {
    std::cout << "  Проверка сравнения" << std::endl;
    Vector<int> a{1, 2};
    Vector<int> b{1, 2};
    Vector<int> c{1, 3};
    TEST_ASSERT(a == b, "Равенство");
    TEST_ASSERT(a != c, "Неравенство");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testVector() {
    std::cout << "\n=== Тестирование Vector ===" << std::endl;
    auto tests = DynamicArray{
        testVectorConstruction,
        testVectorSetGet,
        testVectorArithmetic,
        testVectorLengthNormalize,
        testVectorComparison
    };
    return std::ranges::all_of(tests, [](auto f) { return f(); });
}

int main() {
    try {
        if (testVector()) {
            std::cout << "\n=== Все тесты вектора пройдены ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты вектора не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}