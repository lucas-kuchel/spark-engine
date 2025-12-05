#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include <spark/events/connection.hpp>

namespace spark::events {
    class signal {
    public:
        signal() = default;
        ~signal() = default;

        signal(const signal&) = delete;
        signal(signal&&) noexcept = default;

        signal& operator=(const signal&) = delete;
        signal& operator=(signal&&) noexcept = default;

        template <typename T, auto Fn>
        constexpr void connect() {
            connections_.emplace_back(nullptr, invoke_free<T, Fn>);
        }

        template <typename T, auto Fn, typename U>
        constexpr void connect(U& instance) {
            connections_.emplace_back(&instance, invoke_member<T, Fn, U>);
        }

        template <typename T, auto Fn>
        constexpr void disconnect() {
            for (std::size_t i = connections_.size(); i-- > 0;) {
                connection& connection = connections_[i];

                if (connection.equals(nullptr, invoke_free<T, Fn>)) {
                    std::swap(connections_[i], connections_.back());

                    connections_.pop_back();
                }
            }
        }

        template <typename T, auto Fn, typename U>
        constexpr void disconnect(U& instance) {
            for (std::size_t i = connections_.size(); i-- > 0;) {
                connection& connection = connections_[i];

                if (connection.equals(&instance, invoke_member<T, Fn, U>)) {
                    std::swap(connections_[i], connections_.back());

                    connections_.pop_back();
                }
            }
        }

        template <typename T>
        constexpr void invoke(const T& event) {
            for (connection& connection : connections_) {
                connection.invoke(event);
            }
        }

        constexpr void clear() {
            connections_.clear();
        }

    private:
        std::vector<connection> connections_;

        template <typename T, auto Fn>
        static void invoke_free(void*, const void* event) {
            Fn(*static_cast<const T*>(event));
        }

        template <typename T, auto Fn, typename U>
        static void invoke_member(void* instance, const void* event) {
            (static_cast<U*>(instance)->Fn)(*static_cast<const T*>(event));
        }
    };
}