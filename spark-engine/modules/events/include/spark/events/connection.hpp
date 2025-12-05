#pragma once

namespace spark::events {
    class connection {
    public:
        using invoke_fn_type = void (*)(void*, const void*);
        using instance_type = void*;

        constexpr connection(instance_type instance, invoke_fn_type invokeFunction) noexcept
            : instance_(instance), invoke_(invokeFunction) {
        }

        connection(const connection&) = default;
        connection(connection&&) noexcept = default;

        connection& operator=(const connection&) = default;
        connection& operator=(connection&&) noexcept = default;

        constexpr ~connection() noexcept {
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