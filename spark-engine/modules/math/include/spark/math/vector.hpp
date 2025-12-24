#pragma once

#include <type_traits>
#include <utility>

namespace spark::math {
    template <std::size_t N, typename T>
    requires(std::is_arithmetic_v<T> && N >= 2)
    class vector;

    namespace detail {
        template <typename, typename>
        inline constexpr bool is_compatible_vector = false;

        template <std::size_t N, typename T, std::size_t N1, typename U>
        requires(std::is_arithmetic_v<T> && N >= 2 && std::is_arithmetic_v<U> && N1 >= 2)
        inline constexpr bool is_compatible_vector<vector<N, T>, vector<N1, U>> = N == N1;
    }

    template <std::size_t N, typename T>
    requires(std::is_arithmetic_v<T> && N >= 2)
    class vector {
    public:
        static constexpr std::size_t size = N;
        using type = T;

        constexpr vector() noexcept {
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        explicit constexpr vector(const U& scalar) noexcept : elements_(static_cast<T>(scalar)) {
        }

        template <typename... Args>
        requires(sizeof...(Args) == N && (std::is_convertible_v<Args, T> && ...))
        constexpr vector(Args&&... values) noexcept : elements_(static_cast<T>(std::forward<Args>(values))...) {
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        constexpr vector(const vector<N, U>& other) {
            copy(other, std::make_index_sequence<N>{});
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        constexpr vector(vector<N, U>&& other) noexcept {
            copy(other, std::make_index_sequence<N>{});
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        constexpr vector& operator=(const vector<N, U>& other) {
            copy(other, std::make_index_sequence<N>{});

            return *this;
        }

        template <typename U>
        requires(std::is_convertible_v<U, T>)
        constexpr vector& operator=(vector<N, U>&& other) noexcept {
            copy(other, std::make_index_sequence<N>{});

            return *this;
        }

        [[nodiscard]] constexpr T& operator[](std::size_t n) noexcept {
            return elements_[n];
        }

        [[nodiscard]] constexpr const T& operator[](std::size_t n) const noexcept {
            return elements_[n];
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        [[nodiscard]] constexpr vector operator+(const U& other) const noexcept {
            return add(other, std::make_index_sequence<N>{});
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        [[nodiscard]] constexpr vector operator-(const U& other) const noexcept {
            return sub(other, std::make_index_sequence<N>{});
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        [[nodiscard]] constexpr vector operator*(const U& other) const noexcept {
            return mul(other, std::make_index_sequence<N>{});
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        [[nodiscard]] constexpr vector operator/(const U& other) const noexcept {
            return div(other, std::make_index_sequence<N>{});
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        constexpr vector& operator+=(const U& other) noexcept {
            *this = add(other, std::make_index_sequence<N>{});

            return *this;
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        constexpr vector& operator-=(const U& other) noexcept {
            *this = sub(other, std::make_index_sequence<N>{});

            return *this;
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        constexpr vector& operator*=(const U& other) noexcept {
            *this = mul(other, std::make_index_sequence<N>{});

            return *this;
        }

        template <typename U>
        requires(std::is_convertible_v<U, T> || detail::is_compatible_vector<U, vector<N, T>>)
        constexpr vector& operator/=(const U& other) noexcept {
            *this = div(other, std::make_index_sequence<N>{});

            return *this;
        }

        [[nodiscard]] constexpr T& x() noexcept requires(N >= 1)
        {
            return elements_[0];
        }

        [[nodiscard]] constexpr const T& x() const noexcept requires(N >= 1)
        {
            return elements_[0];
        }

        [[nodiscard]] constexpr T& y() noexcept requires(N >= 2)
        {
            return elements_[1];
        }

        [[nodiscard]] constexpr const T& y() const noexcept requires(N >= 2)
        {
            return elements_[1];
        }

        [[nodiscard]] constexpr T& z() noexcept requires(N >= 3)
        {
            return elements_[2];
        }

        [[nodiscard]] constexpr const T& z() const noexcept requires(N >= 3)
        {
            return elements_[2];
        }

        [[nodiscard]] constexpr T& w() noexcept requires(N >= 4)
        {
            return elements_[3];
        }

        [[nodiscard]] constexpr const T& w() const noexcept requires(N >= 4)
        {
            return elements_[3];
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector<sizeof...(Is), T> swizzle() const noexcept requires((Is < N) && ...)
        {
            return {elements_[Is]...};
        }

    private:
        template <typename U, std::size_t... Is>
        constexpr void copy(const vector<N, U>& other, std::index_sequence<Is...>) noexcept {
            ((elements_[Is] = static_cast<T>(other[Is])), ...);
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector add(const T& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] + other)...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector sub(const T& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] - other)...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector mul(const T& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] * other)...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector div(const T& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] / other)...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector add(const vector<N, T>& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] + other[Is])...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector sub(const vector<N, T>& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] - other[Is])...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector mul(const vector<N, T>& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] * other[Is])...};
        }

        template <std::size_t... Is>
        [[nodiscard]] constexpr vector div(const vector<N, T>& other, std::index_sequence<Is...>) const noexcept {
            return {(elements_[Is] / other[Is])...};
        }

        T elements_[N];
    };

    template <typename T>
    using vector2 = vector<2, T>;

    template <typename T>
    using vector3 = vector<3, T>;

    template <typename T>
    using vector4 = vector<4, T>;
}
