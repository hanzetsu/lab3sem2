#include "TrajectoryCalculator.hpp"
#include "MutableArraySequence.hpp"
#include "test_common.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cmath>

bool testCalculateRange() {
    std::cout << "  Проверка calculateRange" << std::endl;
    ProjectileMotionCalculator calc;
    double v0 = 30.0;
    double angle = 45.0 * Constants::PI / 180.0;
    double range = calc.calculateRange(v0, angle);
    double expected = (v0 * v0 * std::sin(2.0 * angle)) / Constants::G;
    TEST_ASSERT(std::abs(range - expected) < 1e-9, "calculateRange неверно");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testFindAngle() {
    std::cout << "  Проверка findAngle (дихотомия)" << std::endl;
    ProjectileMotionCalculator calc;
    double targetMin = 50.0, targetMax = 52.0;
    double v0 = 30.0;
    double angle;
    bool found = calc.findAngle(v0, targetMin, targetMax, 1e-6, angle);
    TEST_ASSERT(found, "Угол не найден, хотя должен быть");
    double range = calc.calculateRange(v0, angle);
    TEST_ASSERT(range >= targetMin && range <= targetMax, "Дальность не в целевом интервале");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testFindAngleUnreachable() {
    std::cout << "  Проверка findAngle для недостижимой цели" << std::endl;
    ProjectileMotionCalculator calc;
    double targetMin = 1000.0, targetMax = 1100.0;
    double v0 = 30.0;
    double angle;
    bool found = calc.findAngle(v0, targetMin, targetMax, 1e-6, angle);
    TEST_ASSERT(!found, "Угол найден, хотя цель недостижима");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testFindV0AndAngle() {
    std::cout << "  Проверка findV0AndAngle (дискретный перебор)" << std::endl;
    ProjectileMotionCalculator calc;
    MutableArraySequence<double> v0List;
    v0List.Append(20.0);
    v0List.Append(25.0);
    v0List.Append(30.0);
    v0List.Append(35.0);
    double targetMin = 50.0, targetMax = 52.0;
    double bestV0, bestAngle, bestRange;
    bool found = calc.findV0AndAngle(v0List, targetMin, targetMax, 1e-6, bestV0, bestAngle, bestRange);
    TEST_ASSERT(found, "Не найдено решение");
    TEST_ASSERT(bestRange >= targetMin && bestRange <= targetMax, "Дальность не в целевом интервале");
    std::cout << "    OK" << std::endl;
    return true;
}
bool testFindV0AndAngleEmptyList() {
    std::cout << "  Проверка findV0AndAngle с пустым списком" << std::endl;
    ProjectileMotionCalculator calc;
    MutableArraySequence<double> v0List;
    double targetMin = 50.0, targetMax = 52.0;
    double bestV0, bestAngle, bestRange;
    bool found = calc.findV0AndAngle(v0List, targetMin, targetMax, 1e-6, bestV0, bestAngle, bestRange);
    TEST_ASSERT(!found, "Решение найдено при пустом списке");
    std::cout << "    OK" << std::endl;
    return true;
}

bool testTrajectory() {
    std::cout << "\n=== Тестирование ProjectileMotionCalculator ===" << std::endl;
    if (!testCalculateRange()) return false;
    if (!testFindAngle()) return false;
    if (!testFindAngleUnreachable()) return false;
    if (!testFindV0AndAngle()) return false;
    if (!testFindV0AndAngleEmptyList()) return false;
    return true;
}

int main() {
    try {
        if (testTrajectory()) {
            std::cout << "\n=== Все тесты траектории пройдены ===" << std::endl;
            return 0;
        } else {
            std::cerr << "\n=== Тесты траектории не пройдены ===" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << std::endl;
        return 1;
    }
}