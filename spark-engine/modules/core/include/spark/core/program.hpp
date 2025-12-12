#pragma once

#include <chrono>

namespace spark::core {
    template <typename T>
    class program {
    public:
        program() {
            instance_.startup();

            std::chrono::time_point last = std::chrono::steady_clock::now();

            while (instance_.running()) {
                std::chrono::time_point now = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::ratio<1, 1>> delta = now - last;

                last = now;

                instance_.update(delta.count());
            }
        }

        ~program() {
            instance_.shutdown();
        }

    private:
        T instance_;
    };
}