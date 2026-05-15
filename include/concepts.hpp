#include <concepts>

template <typename C>
concept SequenceLike = requires(C c, C c2, std::size_t i, typename C::value_type val) {
    { c.GetLength() } -> std::convertible_to<std::size_t>;
    { c.Get(i) } -> std::convertible_to<typename C::value_type>;
    { c.Set(i, val) };
    { c.GetSubsequence(i, i) } -> std::convertible_to<C*>;
    { c.Append(val) } -> std::convertible_to<C*>;
};