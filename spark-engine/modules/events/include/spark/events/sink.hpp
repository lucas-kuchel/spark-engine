#pragma once

#include <spark/events/family.hpp>

namespace spark::events {
    template <typename T>
    class sink {
    public:
        constexpr sink(std::vector<family_delegate>& families, std::size_t index)
            : families_(families), index_(index) {
        }

        ~sink() = default;

        sink(const sink&) = default;
        sink(sink&&) noexcept = default;

        sink& operator=(const sink&) = default;
        sink& operator=(sink&&) noexcept = delete;

        template <auto Fn>
        constexpr void connect() {
            family<T>& family = acquire();
            signal& signal = family.get_signal();

            signal.disconnect<T, Fn>();
            signal.connect<T, Fn>();
        }

        template <auto Fn, typename C>
        constexpr void connect(C& instance) {
            family<T>& family = acquire();
            signal& signal = family.get_signal();

            signal.disconnect<T, Fn, C>(instance);
            signal.connect<T, Fn, C>(instance);
        }

        template <auto Fn>
        constexpr void disconnect() {
            family<T>& family = acquire();
            signal& signal = family.get_signal();

            signal.disconnect<T, Fn>();
        }

        template <auto Fn, typename C>
        constexpr void disconnect(C& instance) {
            family<T>& family = acquire();
            signal& signal = family.get_signal();

            signal.disconnect<T, Fn, C>(instance);
        }

    private:
        family<T>& acquire() {
            return *reinterpret_cast<family<T>*>(&families_[index_]);
        }

        std::vector<family_delegate>& families_;
        std::size_t index_;
    };
}