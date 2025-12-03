#pragma once

// needed for placement new functionality
// cannot define own placement new; would break compat w. STL
#include <new>

#include <spark/core/traits.hpp>
#include <spark/core/trivials.hpp>

namespace spark::core {
    // @brief perfectly forwards a value, preserving its lvalue/rvalue status
    // @tparam the type to forward as
    // @param the value to forward
    // @returns the forwarded value
    template <typename T>
    [[nodiscard]] inline constexpr T&& forward(remove_reference<T>& value) noexcept {
        return static_cast<T&&>(value);
    }

    template <typename T>
    // @brief perfectly forwards a value, preserving its lvalue/rvalue status
    // @tparam the type to forward as
    // @param the value to forward
    // @returns the forwarded value
    [[nodiscard]] inline constexpr T&& forward(remove_reference<T>&& value) noexcept {
        return static_cast<T&&>(value);
    }

    // @brief cast the provided value to an rvalue to allow for move semantics
    // @tparam the type of the value being cast
    // @param the value to move
    // @returns rvalue to the provided value
    template <typename T>
    [[nodiscard]] inline constexpr remove_reference<T>&& move(T&& value) noexcept {
        return static_cast<remove_reference<T>&&>(value);
    }

    // @brief provides the minimum value out of the two provided cases
    // @tparam T the type of the two values
    // @param the first value candidate
    // @param the second value candidate
    // @returns a reference to the minimum value
    template <typename T>
    requires(is_arithmetic<T>)
    [[nodiscard]] inline constexpr const T& min(const T& a, const T& b) noexcept {
        return (b < a) ? b : a;
    }

    // @brief provides the maximum value out of the two provided cases
    // @tparam T the type of the two values
    // @param the first value candidate
    // @param the second value candidate
    // @returns a reference to the maximum value
    template <typename T>
    [[nodiscard]] inline constexpr const T& max(const T& a, const T& b) noexcept {
        return (a < b) ? b : a;
    }

    // @brief forces the provided value into the positive range
    // @tparam T the type of the value
    // @param the value to manipulate
    // @returns the manipulated value
    template <typename T>
    requires(is_arithmetic<T> && (is_floating_point<T> || is_signed<T>))
    [[nodiscard]] inline constexpr T abs(T value) noexcept {
        return (value < static_cast<T>(0)) ? -value : value;
    }

    // @brief constructs an object in-place
    // @tparam T the type of the object to construct
    // @tparam Args construction args for the type
    // @param pointer to the memory to construct at
    // @param arguments for the object's construction
    // @returns pointer to the memory of the object
    template <typename T, typename... Args>
    inline T* construct_at(T* ptr, Args&&... args) noexcept {
        return ::new (ptr) T(forward<Args>(args)...);
    }

    template <typename T>
    inline T* construct_at(T* ptr, T&& value) noexcept {
        if constexpr (is_trivially_constructible<T>) {
            *ptr = value;
            return ptr;
        }
        else {
            return ::new (ptr) T(forward<T>(value));
        }
    }

    template <typename T>
    inline T* construct_at(T* ptr) noexcept {
        if constexpr (is_trivially_constructible<T>) {
            return ptr;
        }
        else {
            return ::new (ptr) T();
        }
    }
}
