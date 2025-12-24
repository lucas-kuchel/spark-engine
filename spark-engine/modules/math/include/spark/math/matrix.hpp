#pragma once

#include <spark/math/vector.hpp>

namespace spark::math {
    template <std::size_t R, std::size_t C, typename T>
    requires(R >= 2 && C >= 2 && std::is_arithmetic_v<T>)
    class matrix;

    namespace detail {
        template <typename, typename>
        inline constexpr bool matrix_supports_vector = false;

        template <std::size_t R, std::size_t C, typename T, std::size_t N, typename U>
        inline constexpr bool matrix_supports_vector<matrix<R, C, T>, vector<N, U>> = std::is_convertible_v<U, T> && N == R;
    }

    template <std::size_t R, std::size_t C, typename T>
    requires(R >= 2 && C >= 2 && std::is_arithmetic_v<T>)
    class matrix {
    public:
        static constexpr std::size_t columns = C;
        static constexpr std::size_t rows = R;

        using type = T;

        constexpr matrix() {
        }

        explicit constexpr matrix(const T& scalar) noexcept
            : columns_({scalar}) {
        }

        template <typename... Args>
        requires(sizeof...(Args) >= 2 && (detail::matrix_supports_vector<matrix<R, C, T>, Args> && ...))
        constexpr matrix(Args&&... args) noexcept
            : columns_(std::forward<Args>(args)...) {
        }

        [[nodiscard]] constexpr T& operator()(std::size_t r, std::size_t c) noexcept {
            return columns_[c][r];
        }

        [[nodiscard]] constexpr const T& operator()(std::size_t r, std::size_t c) const noexcept {
            return columns_[c][r];
        }

        [[nodiscard]] static constexpr matrix identity() noexcept {
            return identity(std::make_index_sequence<C>{});
        }

    private:
        template <std::size_t... Is>
        [[nodiscard]] static inline matrix identity(std::index_sequence<Is...>) {
            return {identity_column(Is, std::make_index_sequence<R>{})...};
        }

        template <std::size_t... Is>
        [[nodiscard]] static inline vector<R, T> identity_column(std::size_t index, std::index_sequence<Is...>) {
            return {(Is == index)...};
        }

        vector<R, T> columns_[C];
    };

    template <typename T>
    using matrix2x2 = matrix<2, 2, T>;

    template <typename T>
    using matrix3x3 = matrix<3, 3, T>;

    template <typename T>
    using matrix4x4 = matrix<4, 4, T>;

    template <typename T>
    using matrix3x2 = matrix<3, 2, T>;

    template <typename T>
    using matrix2x3 = matrix<2, 3, T>;

    template <typename T>
    using matrix4x2 = matrix<4, 2, T>;

    template <typename T>
    using matrix2x4 = matrix<2, 4, T>;

    template <typename T>
    using matrix4x3 = matrix<4, 3, T>;

    template <typename T>
    using matrix3x4 = matrix<3, 4, T>;
}