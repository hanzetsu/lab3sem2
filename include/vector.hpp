// Vector.hpp
#pragma once

#include "MutableArraySequence.hpp"
#include "exceptions.hpp"
#include <cmath>
#include <initializer_list>

template <typename T>
class Vector {
private:
    MutableArraySequence<T> components;

public:
    Vector() = default;

    explicit Vector(std::size_t dim) : components(dim) {
        for (std::size_t i = 0; i < dim; ++i)
            components.Set(i, T(0));
    }

    Vector(std::initializer_list<T> list) : components(static_cast<std::size_t>(list.size())) {
        std::size_t i = 0;
        for (const T& val : list)
            components.Set(i++, val);
    }

    Vector(const Vector&) = default;
    Vector& operator=(const Vector&) = default;

    std::size_t size() const {
        return components.GetLength();
    }

    T operator[](std::size_t i) const {
        return components.Get(i);
    }

    void set(std::size_t i, T val) {
        if (i >= components.GetLength())
            throw IndexOutOfRange(i, components.GetLength(), "Vector::set");
        components.Set(i, val);
    }

    T get(std::size_t i) const {
        return components.Get(i);
    }

    Vector operator+(const Vector& other) const {
        std::size_t n = size();
        if (other.size() != n)
            throw InvalidArgument("Размерности векторов не совпадают");
        Vector result(n);
        for (std::size_t i = 0; i < n; ++i)
            result.set(i, get(i) + other.get(i));
        return result;
    }

    Vector operator-(const Vector& other) const {
        std::size_t n = size();
        if (other.size() != n)
            throw InvalidArgument("Размерности векторов не совпадают");
        Vector result(n);
        for (std::size_t i = 0; i < n; ++i)
            result.set(i, get(i) - other.get(i));
        return result;
    }

    Vector operator*(T scalar) const {
        std::size_t n = size();
        Vector result(n);
        for (std::size_t i = 0; i < n; ++i)
            result.set(i, get(i) * scalar);
        return result;
    }

    T dot(const Vector& other) const {
        std::size_t n = size();
        if (other.size() != n)
            throw InvalidArgument("Размерности векторов не совпадают");
        T sum = T(0);
        for (std::size_t i = 0; i < n; ++i)
            sum += get(i) * other.get(i);
        return sum;
    }

    double length() const {
        double sum = 0.0;
        for (std::size_t i = 0; i < size(); ++i)
            sum += static_cast<double>(get(i) * get(i));
        return std::sqrt(sum);
    }

    Vector normalized() const {
        double len = length();
        if (len < 1e-12)
            return *this;
        std::size_t n = size();
        Vector result(n);
        for (std::size_t i = 0; i < n; ++i)
            result.set(i, static_cast<T>(get(i) / len));
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "[";
        for (std::size_t i = 0; i < v.size(); ++i) {
            if (i > 0) os << ", ";
            os << v.get(i);
        }
        os << "]";
        return os;
    }

    bool operator==(const Vector& other) const {
        if (size() != other.size()) return false;
        for (std::size_t i = 0; i < size(); ++i)
            if (get(i) != other.get(i))
                return false;
        return true;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
};