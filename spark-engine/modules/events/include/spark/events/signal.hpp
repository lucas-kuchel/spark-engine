#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include <spark/events/connections.hpp>

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
            freeConnections_.emplace_back(invoke_free<T, Fn>);
        }

        template <typename T, auto Fn, typename U>
        constexpr void connect(U& instance) {
            memberConnections_.emplace_back(&instance, invoke_member<T, Fn, U>);
        }

        template <typename T, auto Fn>
        constexpr void disconnect() {
            for (auto it = freeConnections_.rbegin(); it != freeConnections_.rend(); ++it) {
                free_connection& connection = *it;

                if (connection.equals(invoke_free<T, Fn>)) {
                    std::swap(connection, freeConnections_.back());

                    freeConnections_.pop_back();

                    break;
                }
            }
        }

        template <typename T, auto Fn, typename U>
        constexpr void disconnect(U& instance) {
            for (auto it = memberConnections_.rbegin(); it != memberConnections_.rend(); ++it) {
                member_connection& connection = *it;

                if (connection.equals(&instance, invoke_member<T, Fn, U>)) {
                    std::swap(connection, memberConnections_.back());

                    memberConnections_.pop_back();

                    break;
                }
            }
        }

        template <typename T>
        constexpr void invoke(const T& event) {
            for (free_connection& connection : freeConnections_) {
                connection.invoke(event);
            }

            for (member_connection& connection : memberConnections_) {
                connection.invoke(event);
            }
        }

        constexpr void clear() {
            freeConnections_.clear();
            memberConnections_.clear();
        }

    private:
        std::vector<free_connection> freeConnections_;
        std::vector<member_connection> memberConnections_;

        template <typename T, auto Fn>
        static void invoke_free(const void* event) {
            Fn(*static_cast<const T*>(event));
        }

        template <typename T, auto Fn, typename U>
        static void invoke_member(void* instance, const void* event) {
            (static_cast<U*>(instance)->Fn)(*static_cast<const T*>(event));
        }
    };
}