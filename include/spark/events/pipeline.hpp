#pragma once

#if SPARK_SUPPORTED

#include "connection.hpp"

#include <utility>
#include <vector>

namespace spark::events {
    template <typename T>
    class pipeline {
    public:
        pipeline() noexcept = default;
        ~pipeline() noexcept = default;

        pipeline(const pipeline&) = default;
        pipeline(pipeline&&) noexcept = default;

        pipeline& operator=(const pipeline&) = default;
        pipeline& operator=(pipeline&&) noexcept = default;

        template <typename U, auto Fn>
        void connect(U& instance) noexcept {
            connections_.emplace_back(connection<T>::template create<U, Fn>(instance));
        }

        template <auto Fn>
        void connect() noexcept {
            connections_.emplace_back(connection<T>::template create<Fn>());
        }

        template <typename U, auto Fn>
        void disconnect(U& instance) noexcept {
            connection<T> target = connection<T>::template create<U, Fn>(instance);

            erase(target);
        }

        template <auto Fn>
        void disconnect() noexcept {
            connection<T> target = connection<T>::template create<Fn>();

            erase(target);
        }

        template <typename... Args>
        void enqueue(Args&&... args) noexcept {
            events_.emplace_back(std::forward<Args>(args)...);
        }

        template <typename... Args>
        void trigger(Args&&... args) noexcept {
            T temporary = T(std::forward<Args>(args)...);

            for (connection<T>& connection : connections_) {
                connection(temporary);
            }
        }

        void dispatch() noexcept {
            for (T& event : events_) {
                for (connection<T>& connection : connections_) {
                    connection(event);
                }
            }

            events_.clear();
        }

        void reset() noexcept {
            events_.clear();
            connections_.clear();
        }

    private:
        void erase(connection<T>& target) noexcept {
            for (std::size_t i = connections_.size(); i-- > 0;) {
                connection<T>& current = connections_[i];

                if (current == target) {
                    std::swap(current, connections_.back());
                    connections_.pop_back();

                    break;
                }
            }
        }

        std::vector<T> events_;
        std::vector<connection<T>> connections_;
    };
}

#endif