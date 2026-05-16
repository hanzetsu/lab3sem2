#pragma once

#include <cstddef>
#include <stdexcept>
#include <cmath>
#include <initializer_list>

template <typename T, std::size_t N>
class Vector
{
    static_assert(N > 0, "Размерность должна быть положительной");

private:
    T data[N];

public:
    Vector()
    {
        for (std::size_t i = 0; i < N; ++i)
            data[i] = T(0);
    }

    Vector(std::initializer_list<T> list)
    {
        if (list.size() != N)
            throw std::invalid_argument("Неверное количество компонентов");
        std::size_t i = 0;
        for (const T &val : list)
            data[i++] = val;
    }

    Vector(const Vector &) = default;
    Vector &operator=(const Vector &) = default;

    T &operator[](std::size_t i)
    {
        if (i >= N)
            throw std::out_of_range("Индекс вектора выходит за границы");
        return data[i];
    }

    const T &operator[](std::size_t i) const
    {
        if (i >= N)
            throw std::out_of_range("Индекс вектора выходит за границы");
        return data[i];
    }

    constexpr std::size_t size() const { return N; }

    double length() const
    {
        double sum = 0.0;
        for (std::size_t i = 0; i < N; ++i)
            sum += static_cast<double>(data[i] * data[i]);
        return std::sqrt(sum);
    }

    Vector normalized() const
    {
        double len = length();
        if (len < 1e-12)
            return *this;
        Vector result;
        for (std::size_t i = 0; i < N; ++i)
            result.data[i] = static_cast<T>(data[i] / len);
        return result;
    }

    Vector operator+(const Vector &other) const
    {
        Vector result;
        for (std::size_t i = 0; i < N; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    Vector operator-(const Vector &other) const
    {
        Vector result;
        for (std::size_t i = 0; i < N; ++i)
            result.data[i] = data[i] - other.data[i];
        return result;
    }
};