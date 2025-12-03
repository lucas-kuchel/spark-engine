#pragma once

#include <spark/core/primitives.hpp>

namespace spark::core {
    template <typename>
    inline constexpr bool is_integer = false;

    template <>
    inline constexpr bool is_integer<int8> = true;

    template <>
    inline constexpr bool is_integer<int16> = true;

    template <>
    inline constexpr bool is_integer<int32> = true;

    template <>
    inline constexpr bool is_integer<int64> = true;

    template <>
    inline constexpr bool is_integer<uint8> = true;

    template <>
    inline constexpr bool is_integer<uint16> = true;

    template <>
    inline constexpr bool is_integer<uint32> = true;

    template <>
    inline constexpr bool is_integer<uint64> = true;

    template <typename>
    inline constexpr bool is_floating_point = false;

    template <>
    inline constexpr bool is_floating_point<float32> = true;

    template <>
    inline constexpr bool is_floating_point<float64> = true;

    template <typename T>
    inline constexpr bool is_arithmetic = is_integer<T> || is_floating_point<T>;

    template <typename>
    inline constexpr bool is_signed = false;

    template <>
    inline constexpr bool is_signed<int8> = true;

    template <>
    inline constexpr bool is_signed<int16> = true;

    template <>
    inline constexpr bool is_signed<int32> = true;

    template <>
    inline constexpr bool is_signed<int64> = true;

    template <>
    inline constexpr bool is_signed<char> = false;

    template <typename T>
    inline constexpr bool is_unsigned = !is_signed<T> && !is_floating_point<T>;

    template <>
    inline constexpr bool is_unsigned<char> = false;

    namespace detail {
        template <class T>
        struct reference_remover {
            using type = T;
        };

        template <class T>
        struct reference_remover<T&> {
            using type = T;
        };

        template <class T>
        struct reference_remover<T&&> {
            using type = T;
        };
    }

    template <typename T>
    using remove_reference = detail::reference_remover<T>::type;
}