#pragma once

#include "Vector.hpp"
#include "MutableArraySequence.hpp"
#include "exceptions.hpp"
#include "Constants.hpp"
#include <cmath>

class ProjectileMotionCalculator {
private:
    double calculateRange(const DefaultVector<double>& vel) const {
        if (vel.size() != 2)
            throw InvalidArgument("Вектор скорости должен быть двумерным");
        double time = 2.0 * vel.get(1) / Constants::G;
        return vel.get(0) * time;
    }

    double rangeForAngle(double v0, double angleRad) const {
        return calculateRange(v0, angleRad);
    }

public:
    double calculateRange(double v0, double angleRad) const {
        double vx = v0 * std::cos(angleRad);
        double vy = v0 * std::sin(angleRad);
        DefaultVector<double> vel(2);
        vel.set(0, vx);
        vel.set(1, vy);
        return calculateRange(vel);
    }

    bool findAngle(double v0, double targetMin, double targetMax, double eps, double& resultAngle) const {
        double a = 0.0, b = Constants::PI_QUARTER;
        double fa = rangeForAngle(v0, a);
        double fb = rangeForAngle(v0, b);
        if (fa > targetMax && fb > targetMax) return false;
        if (fa < targetMin && fb < targetMin) return false;
        while ((b - a) > eps) {
            double c = (a + b) / 2.0;
            double fc = rangeForAngle(v0, c);
            if (fc >= targetMin && fc <= targetMax) {
                resultAngle = c;
                return true;
            }
            if (fc < targetMin) a = c;
            else if (fc > targetMax) b = c;
            else {
                resultAngle = c;
                return true;
            }
        }
        resultAngle = (a + b) / 2.0;
        double finalRange = rangeForAngle(v0, resultAngle);
        return (finalRange >= targetMin && finalRange <= targetMax);
    }

    bool findV0AndAngle(const MutableArraySequence<double>& v0List,
                        double targetMin, double targetMax, double eps,
                        double& bestV0, double& bestAngle, double& bestRange) const {
        for (std::size_t i = 0; i < v0List.GetLength(); ++i) {
            double v0 = v0List.Get(i);
            double angle;
            if (findAngle(v0, targetMin, targetMax, eps, angle)) {
                bestV0 = v0;
                bestAngle = angle;
                bestRange = calculateRange(v0, angle);
                return true;
            }
        }
        return false;
    }

    DefaultVector<double> getVelocity(double v0, double angleRad) const {
        DefaultVector<double> vel(2);
        vel.set(0, v0 * std::cos(angleRad));
        vel.set(1, v0 * std::sin(angleRad));
        return vel;
    }
};