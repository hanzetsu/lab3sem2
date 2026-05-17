#pragma once

#include "Vector.hpp"
#include "exceptions.hpp"
#include <cmath>

class ProjectileMotionCalculator {
private:
    const double g = 9.80665;
    const double PI = 3.14159265358979323846;
    const double PI_HALF = PI / 2.0;
public:
    double calculateRange(double angleRad, double v0) const {
        double vx = v0 * std::cos(angleRad);
        double vy = v0 * std::sin(angleRad);
        double time = 2.0 * vy / g;
        return vx * time;
    }

    bool findAngle(double a, double b, double v0,
                   double targetMin, double targetMax,
                   double eps, double& resultAngle) const {
        if (a >= b) return false;
        double fa = calculateRange(a, v0);
        double fb = calculateRange(b, v0);
        if (fa > targetMax && fb > targetMax) return false;
        if (fa < targetMin && fb < targetMin) return false;
        while ((b - a) > eps) {
            double c = (a + b) / 2.0;
            double fc = calculateRange(c, v0);
            if (fc >= targetMin && fc <= targetMax) {
                resultAngle = c;
                return true;
            }
            if (fc < targetMin) {
                a = c;
            } else if (fc > targetMax) {
                b = c;
            } else {
                resultAngle = c;
                return true;
            }
            fa = calculateRange(a, v0);
            fb = calculateRange(b, v0);
        }
        resultAngle = (a + b) / 2.0;
        double finalRange = calculateRange(resultAngle, v0);
        return (finalRange >= targetMin && finalRange <= targetMax);
    }

    bool findV0AndAngle(const MutableArraySequence<double>& v0List,
                        double targetMin, double targetMax, double eps,
                        double& bestV0, double& bestAngle, double& bestRange) const {
        for (std::size_t i = 0; i < v0List.GetLength(); ++i) {
            double v0 = v0List.Get(i);
            double angle;
            if (findAngle(0.0, PI_HALF, v0, targetMin, targetMax, eps, angle)) {
                bestV0 = v0;
                bestAngle = angle;
                bestRange = calculateRange(angle, v0);
                return true;
            }
        }
        return false;
    }
};