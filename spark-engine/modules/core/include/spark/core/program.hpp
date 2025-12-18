#pragma once

#include <chrono>

namespace spark::core {
    template <typename T>
    class program {
    public:
        void run() {
            ::new (instance_) T();

            std::chrono::time_point last = std::chrono::steady_clock::now();

            while (reinterpret_cast<T*>(instance_)->running()) {
                std::chrono::time_point now = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::ratio<1, 1>> delta = now - last;

                last = now;

                reinterpret_cast<T*>(instance_)->update(delta.count());
            }

            reinterpret_cast<T*>(instance_)->~T();
        }

    private:
        alignas(T) std::byte instance_[sizeof(T)];
    };
}