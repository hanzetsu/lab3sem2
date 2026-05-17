#pragma once
#include "Vector.hpp"
#include "MutableArraySequence.hpp"
#include "exceptions.hpp"
#include "Constants.hpp"
#include <cmath>

class ProjectileMotionCalculator
{
public:
    double calculateRange(const Vector<double, 2> &vel) const
    {
        double time = 2.0 * vel.get(1) / Constants::G;
        return vel.get(0) * time;
    }
    double calculateRange(double v0, double angleRad) const
    {
        double vx = v0 * std::cos(angleRad);
        double vy = v0 * std::sin(angleRad);
        return calculateRange(Vector<double, 2>({vx, vy}));
    }
    bool findAngle(double v0, double targetMin, double targetMax, double eps, double &resultAngle) const
    {
        double a = 0.0, b = Constants::PI_QUARTER;
        auto rangeAt = [this, v0](double angle)
        {
            double vx = v0 * std::cos(angle);
            double vy = v0 * std::sin(angle);
            return calculateRange(Vector<double, 2>({vx, vy}));
        };
        double fa = rangeAt(a);
        double fb = rangeAt(b);
        if (fa > targetMax && fb > targetMax)
            return false;
        if (fa < targetMin && fb < targetMin)
            return false;
        while ((b - a) > eps)
        {
            double c = (a + b) / 2.0;
            double fc = rangeAt(c);
            if (fc >= targetMin && fc <= targetMax)
            {
                resultAngle = c;
                return true;
            }
            if (fc < targetMin)
                a = c;
            else if (fc > targetMax)
                b = c;
            else
            {
                resultAngle = c;
                return true;
            }
        }
        resultAngle = (a + b) / 2.0;
        double finalRange = rangeAt(resultAngle);
        return (finalRange >= targetMin && finalRange <= targetMax);
    }

    bool findV0AndAngle(const MutableArraySequence<double> &v0List,
                        double targetMin, double targetMax, double eps,
                        double &bestV0, double &bestAngle, double &bestRange) const
    {
        for (std::size_t i = 0; i < v0List.GetLength(); ++i)
        {
            double v0 = v0List.Get(i);
            double angle;
            if (findAngle(v0, targetMin, targetMax, eps, angle))
            {
                bestV0 = v0;
                bestAngle = angle;
                bestRange = calculateRange(Vector<double, 2>({v0 * std::cos(angle), v0 * std::sin(angle)}));
                return true;
            }
        }
        return false;
    }

    Vector<double, 2> getVelocity(double v0, double angleRad) const
    {
        return Vector<double, 2>({v0 * std::cos(angleRad), v0 * std::sin(angleRad)});
    }
};