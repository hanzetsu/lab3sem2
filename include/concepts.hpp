#include <concepts>

template <typename C>
concept SequenceLike =
    requires(C c, std::size_t i, typename C::value_type val) {
        { c.GetLength() } -> std::convertible_to<std::size_t>;

        { c.Get(i) } -> std::convertible_to<const typename C::value_type &>;

        { c.Set(i, val) };

        { c.GetSubsequence(i, i) };

        { c.Append(val) };
    };