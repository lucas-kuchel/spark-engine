#pragma once

#include <cstring>

#include <spark/events/signal.hpp>

namespace spark::events {
    template <typename T>
    class family {
    public:
        family() = default;
        ~family() = default;

        family(const family&) = delete;
        family(family&&) noexcept = default;

        family& operator=(const family&) = delete;
        family& operator=(family&&) noexcept = default;

        constexpr signal& get_signal() noexcept {
            return signal_;
        }

        constexpr std::vector<T>& get_queue() noexcept {
            return queue_;
        }

        constexpr const signal& get_signal() const noexcept {
            return signal_;
        }

        constexpr const std::vector<T>& get_queue() const noexcept {
            return queue_;
        }

    private:
        signal signal_;
        std::vector<T> queue_;
    };

    class family_delegate {
    public:
        family_delegate() = default;
        ~family_delegate() {
            if (destructor_) {
                destructor_(buffer_);
            }
        }

        family_delegate(const family_delegate&) = delete;

        family_delegate(family_delegate&& other) noexcept {
            std::memcpy(buffer_, other.buffer_, sizeof(buffer_));

            destructor_ = other.destructor_;
            invoke_ = other.invoke_;

            other.destructor_ = nullptr;
            other.invoke_ = nullptr;
        }

        family_delegate& operator=(const family_delegate&) = delete;

        family_delegate& operator=(family_delegate&& other) noexcept {
            std::memcpy(buffer_, other.buffer_, sizeof(buffer_));
            destructor_ = other.destructor_;
            invoke_ = other.invoke_;

            other.destructor_ = nullptr;
            other.invoke_ = nullptr;

            return *this;
        }

        template <typename T>
        constexpr void set_destructor() {
            destructor_ = destruct<T>;
        }

        template <typename T>
        constexpr void set_invoke() {
            invoke_ = invoke<T>;
        }

        constexpr bool needs_init() const {
            return invoke_ == nullptr || destructor_ == nullptr;
        }

        constexpr void invoke() {
            invoke_(buffer_);
        }

        template <typename T>
        family<T>& get() {
            return *reinterpret_cast<family<T>*>(buffer_);
        }

        template <typename T>
        const family<T>& get() const {
            return *reinterpret_cast<const family<T>*>(buffer_);
        }

    private:
        using family_mock_type = family<void*>;
        using destructor_fn_type = void (*)(void*);
        using invoke_fn_type = void (*)(void*);

        alignas(family_mock_type) std::byte buffer_[sizeof(family_mock_type)];

        destructor_fn_type destructor_ = nullptr;
        invoke_fn_type invoke_ = nullptr;

        template <typename T>
        constexpr static void destruct(void* memory) {
            reinterpret_cast<family<T>*>(memory)->~family<T>();
        }

        template <typename T>
        constexpr static void invoke(void* memory) {
            family<T>& instance = *reinterpret_cast<family<T>*>(memory);
            std::vector<T>& queue = instance.get_queue();
            signal& signal = instance.get_signal();

            for (const T& event : queue) {
                signal.invoke(event);
            }

            signal.clear();
        }
    };
}