#pragma once

#include <cstdlib>

namespace spark::ecs::detail {
    constexpr inline std::size_t increment() {
        static std::size_t value = 0;

        return value++;
    }

    template <typename T>
    [[nodiscard]] constexpr std::size_t type_index() {
        static std::size_t value = ecs::detail::increment();

        return value;
    }
}