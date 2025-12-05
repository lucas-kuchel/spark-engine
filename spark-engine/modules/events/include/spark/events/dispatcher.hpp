#pragma once

#include <spark/events/family.hpp>
#include <spark/events/index.hpp>
#include <spark/events/sink.hpp>

namespace spark::events {
    class dispatcher {
    public:
        dispatcher() = default;
        ~dispatcher() = default;

        dispatcher(const dispatcher&) = delete;
        dispatcher(dispatcher&&) noexcept = default;

        dispatcher& operator=(const dispatcher&) = delete;
        dispatcher& operator=(dispatcher&&) noexcept = default;

        template <typename T>
        constexpr spark::events::sink<T> sink() {
            std::size_t index = acquireFamilyIndex<T>();

            return spark::events::sink<T>(families_, index);
        }

        template <typename T>
        constexpr void drain() {
            std::size_t index = detail::type_index<T>();

            family_delegate& delegate = families_[index];
            family<T>& family = delegate.get<T>();

            family.get_queue().clear();
            family.get_signal().clear();
        }

        template <typename T, typename... Args>
        constexpr void trigger(Args&&... args) {
            std::size_t index = detail::type_index<T>();

            family_delegate& delegate = families_[index];
            family<T>& family = delegate.get<T>();
            signal& signal = family.get_signal();

            T event(std::forward<Args>(args)...);

            signal.invoke(event);
        }

        template <typename T, typename... Args>
        constexpr void enqueue(Args&&... args) {
            std::size_t index = detail::type_index<T>();

            family_delegate& delegate = families_[index];
            family<T>& family = delegate.get<T>();
            std::vector<T>& queue = family.get_queue();

            queue.emplace_back(std::forward<Args>(args)...);
        }

        constexpr void update() {
            for (auto& delegate : families_) {
                delegate.invoke();
            }
        }

    private:
        template <typename T>
        std::size_t acquireFamilyIndex() {
            std::size_t index = detail::type_index<T>();

            if (index + 1 > families_.size()) {
                families_.resize(index + 1);
            }

            family_delegate& delegate = families_[index];

            if (delegate.needs_init()) {
                delegate.set_destructor<T>();
                delegate.set_invoke<T>();
            }

            return index;
        }

        std::vector<family_delegate> families_;
    };
}