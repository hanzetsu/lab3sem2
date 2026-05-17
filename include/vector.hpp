#pragma once
#include "MutableArraySequence.hpp"
#include "exceptions.hpp"
#include <cmath>
#include <initializer_list>

template <typename T>
class Vector {
private:
    MutableArraySequence<T> comps;
public:
    Vector() = default;
    explicit Vector(std::size_t dim) : comps(dim) {
        for (std::size_t i = 0; i < dim; ++i)
            comps.Set(i, T(0));
    }
    Vector(std::initializer_list<T> list) : comps(list.size()) {
        std::size_t i = 0;
        for (const T& val : list)
            comps.Set(i++, val);
    }
    std::size_t size() const { return comps.GetLength(); }
    T operator[](std::size_t i) const { return comps.Get(i); }
    void set(std::size_t i, T val) { comps.Set(i, val); }
    T get(std::size_t i) const { return comps.Get(i); }
    double length() const {
        double sum = 0.0;
        for (std::size_t i = 0; i < size(); ++i)
            sum += static_cast<double>(comps.Get(i) * comps.Get(i));
        return std::sqrt(sum);
    }
    Vector normalized() const {
        double len = length();
        if (len < 1e-12) return *this;
        Vector result(size());
        for (std::size_t i = 0; i < size(); ++i)
            result.set(i, static_cast<T>(comps.Get(i) / len));
        return result;
    }
    Vector operator+(const Vector& other) const {
        if (size() != other.size())
            throw InvalidArgument("Размерности векторов не совпадают");
        Vector result(size());
        for (std::size_t i = 0; i < size(); ++i)
            result.set(i, get(i) + other.get(i));
        return result;
    }
    Vector operator-(const Vector& other) const {
        if (size() != other.size())
            throw InvalidArgument("Размерности векторов не совпадают");
        Vector result(size());
        for (std::size_t i = 0; i < size(); ++i)
            result.set(i, get(i) - other.get(i));
        return result;
    }
    Vector operator*(T scalar) const {
        Vector result(size());
        for (std::size_t i = 0; i < size(); ++i)
            result.set(i, get(i) * scalar);
        return result;
    }
    T dot(const Vector& other) const {
        if (size() != other.size())
            throw InvalidArgument("Размерности векторов не совпадают");
        T sum = T(0);
        for (std::size_t i = 0; i < size(); ++i)
            sum += get(i) * other.get(i);
        return sum;
    }
    bool operator==(const Vector& other) const {
        if (size() != other.size()) return false;
        for (std::size_t i = 0; i < size(); ++i)
            if (get(i) != other.get(i)) return false;
        return true;
    }
    bool operator!=(const Vector& other) const { return !(*this == other); }
};