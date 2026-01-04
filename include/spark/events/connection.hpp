#pragma once

#if SPARK_SUPPORTED

namespace spark::events {
    template <typename T>
    class connection {
    public:
        connection() noexcept = default;

        template <typename U, auto Fn>
        connection(U& instance) noexcept : instance_(&instance), invoke_(member_invoke<U, Fn>) {
        }

        template <auto Fn>
        connection() noexcept : invoke_(free_invoke<Fn>) {
        }

        template <typename U, auto Fn>
        [[nodiscard]] static connection create(U& instance) noexcept {
            connection temp = {};

            temp.instance_ = &instance;
            temp.invoke_ = &member_invoke<U, Fn>;

            return temp;
        }

        template <auto Fn>
        [[nodiscard]] static connection create() noexcept {
            connection temp = {};

            temp.invoke_ = &free_invoke<Fn>;

            return temp;
        }

        [[nodiscard]] bool operator==(const connection& other) const noexcept {
            return instance_ == other.instance_ && invoke_ == other.invoke_;
        }

        [[nodiscard]] bool operator!=(const connection& other) const noexcept {
            return instance_ != other.instance_ || invoke_ != other.invoke_;
        }

        void operator()(const T& event) noexcept {
            invoke_(event, instance_);
        }

    private:
        void* instance_ = nullptr;
        void (*invoke_)(const T&, void*) = nullptr;

        template <typename U>
        static U& cast(void* memory) noexcept {
            return *reinterpret_cast<U*>(memory);
        }

        template <typename U, auto Fn>
        static void member_invoke(const T& event, void* ptr) noexcept {
            U& instance = cast<U>(ptr);

            (instance.*Fn)(event);
        }

        template <auto Fn>
        static void free_invoke(const T& event, void*) noexcept {
            Fn(event);
        }
    };
}

#endif