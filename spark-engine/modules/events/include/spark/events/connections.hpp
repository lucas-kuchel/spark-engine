#pragma once

namespace spark::events {
    class free_connection {
    public:
        using invoke_fn_type = void (*)(const void*);
        constexpr free_connection(invoke_fn_type invokeFunction) noexcept
            : invoke_(invokeFunction) {
        }

        free_connection(const free_connection&) = default;
        free_connection(free_connection&&) noexcept = default;

        free_connection& operator=(const free_connection&) = default;
        free_connection& operator=(free_connection&&) noexcept = default;

        constexpr ~free_connection() noexcept {
            invoke_ = nullptr;
        }

        template <typename T>
        constexpr void invoke(const T& event) noexcept {
            invoke_(&event);
        }

        constexpr bool equals(invoke_fn_type invokeFunction) const {
            return invoke_ == invokeFunction;
        }

    private:
        invoke_fn_type invoke_;
    };

    class member_connection {
    public:
        using invoke_fn_type = void (*)(void*, const void*);
        using instance_type = void*;

        constexpr member_connection(instance_type instance, invoke_fn_type invokeFunction) noexcept
            : instance_(instance), invoke_(invokeFunction) {
        }

        member_connection(const member_connection&) = default;
        member_connection(member_connection&&) noexcept = default;

        member_connection& operator=(const member_connection&) = default;
        member_connection& operator=(member_connection&&) noexcept = default;

        constexpr ~member_connection() noexcept {
            instance_ = nullptr;
            invoke_ = nullptr;
        }

        template <typename T>
        constexpr void invoke(const T& event) noexcept {
            invoke_(instance_, &event);
        }

        constexpr bool equals(instance_type instance, invoke_fn_type invokeFunction) const {
            return instance_ == instance && invoke_ == invokeFunction;
        }

    private:
        instance_type instance_;
        invoke_fn_type invoke_;
    };
}