#pragma once

#if SPARK_SUPPORTED

#include <cstddef>
#include <cstdlib>

namespace spark::events {
    template <std::size_t A, std::size_t S>
    requires(A > 0 && S > 0)
    class memory {
    private:
        alignas(A) std::byte bytes_[S];
    };

    template <typename T>
    using memory_of = memory<alignof(T), sizeof(T)>;
}

#endif