#pragma once

#include <cstdio>
#include <format>

namespace spark::core {
    constexpr void println() {
        std::fputc('\n', stdout);
    }

    template <typename... Args>
    constexpr void println(std::format_string<Args...> message, Args&&... args) {
        std::string formatted = std::format(message, std::forward<Args>(args)...);

        std::fputs(formatted.c_str(), stdout);

        println();
    }

    template <typename... Args>
    constexpr void print(std::format_string<Args...> message, Args&&... args) {
        std::string formatted = std::format(message, std::forward<Args>(args)...);

        std::fputs(formatted.c_str(), stdout);
    }
}