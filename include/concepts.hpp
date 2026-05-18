#pragma once
#include <concepts>
#include <cstddef>

template <typename C>
concept SequenceLike = requires(C c, std::size_t i, typename C::value_type val) {
    { c.GetLength() } -> std::convertible_to<std::size_t>;
    { c.Get(i) } -> std::convertible_to<const typename C::value_type &>;
    { c.Set(i, val) };
    { c.GetSubsequence(i, i) };
    { c.Append(val) };
};

template <typename T>
concept Vectorizable = requires(T a, T b, double s) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * s } -> std::convertible_to<T>;
    { s * a } -> std::convertible_to<T>;
    { a / s } -> std::convertible_to<T>;
    { a == b } -> std::convertible_to<bool>;
    { static_cast<double>(a) } -> std::convertible_to<double>;
    { std::abs(a) } -> std::convertible_to<double>;
    { T(0) };
};
