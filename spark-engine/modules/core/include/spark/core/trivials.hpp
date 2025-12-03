#pragma once

#include <type_traits>

namespace spark::core {
    template <typename T>
    inline constexpr bool is_trivially_constructible = std::is_trivially_constructible_v<T>;

    template <typename T>
    inline constexpr bool is_trivially_copyable = std::is_trivially_copyable_v<T>;
}