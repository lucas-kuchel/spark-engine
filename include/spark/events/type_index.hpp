#pragma once

#if SPARK_SUPPORTED

#include <cstdlib>

namespace spark::events {
    [[nodiscard]] std::size_t counter() noexcept {
        static std::size_t index = 0;

        return index++;
    }

    template <typename>
    [[nodiscard]] std::size_t type_index() noexcept {
        static std::size_t index = counter();

        return index;
    }
}

#endif