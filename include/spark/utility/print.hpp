#pragma once

#if SPARK_SUPPORTED

#include <cstdio>
#include <format>

namespace spark::utility {
    template <typename... Args>
    constexpr inline void print(std::format_string<Args...> message, Args&&... args) noexcept {
        std::fputs(std::format(message, std::forward<Args>(args)...).c_str(), stdout);
    }

    constexpr inline void println() noexcept {
        std::fputc('\n', stdout);
    }

    template <typename... Args>
    constexpr inline void println(std::format_string<Args...> message, Args&&... args) noexcept {
        print(message, std::forward<Args>(args)...);
        println();
    }
}

#endif