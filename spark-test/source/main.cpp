#include <chrono>
#include <iostream>
#include <spark/spark.hpp>
#include <vector>

using namespace std::chrono;

class new_growth_policy {
public:
    template <typename T = spark::uint64>
    requires(spark::is_unsigned<T>)
    inline constexpr static T expand(T minimum, T current) noexcept {
        return spark::max(spark::max(static_cast<T>(16), minimum), static_cast<spark::uint64>(4 * current));
    }
};

// Timing helper
template <typename F>
long long benchmark(F&& func, size_t iterations = 1) {
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i)
        func();
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

// ------------------- spark::list benchmarks -------------------
long long benchmark_spark_push(size_t n) {
    return benchmark([&] {
        spark::list<int, spark::uint64, new_growth_policy> l;
        for (size_t i = 0; i < n; ++i)
            l.push(i);
    });
}

long long benchmark_spark_resize_clear(size_t n) {
    return benchmark([&] {
        spark::list<int, spark::uint64, new_growth_policy> l;
        for (size_t i = 0; i < n; ++i) {
            l.resize(n * 2, 42);
            l.resize(n / 2);
            while (!l.empty())
                l.pop();
        }
    });
}

long long benchmark_spark_swap_pop(size_t n) {
    return benchmark([&] {
        spark::list<int, spark::uint64, new_growth_policy> l;
        for (size_t i = 0; i < n; ++i)
            l.push(i);

        // Swap from front
        size_t half = n / 2;
        for (size_t i = 0; i < half; ++i)
            l.swap(i, half + i);

        // Pop all
        while (!l.empty())
            l.pop();
    });
}

// ------------------- std::vector benchmarks -------------------
long long benchmark_vector_push(size_t n) {
    return benchmark([&] {
        std::vector<int> v;
        for (size_t i = 0; i < n; ++i)
            v.push_back(i);
    });
}

long long benchmark_vector_resize_clear(size_t n) {
    return benchmark([&] {
        std::vector<int> v;
        for (size_t i = 0; i < n; ++i) {
            v.resize(n * 2, 42);
            v.resize(n / 2);
            while (!v.empty())
                v.pop_back();
        }
    });
}

long long benchmark_vector_swap_pop(size_t n) {
    return benchmark([&] {
        std::vector<int> v;
        for (size_t i = 0; i < n; ++i)
            v.push_back(i);

        // Swap from front
        size_t half = n / 2;
        for (size_t i = 0; i < half; ++i)
            std::swap(v[i], v[half + i]);

        // Pop all
        while (!v.empty())
            v.pop_back();
    });
}

// ------------------- main -------------------
int main() {
    constexpr size_t n = 65536 * 2;

    auto spark_push = benchmark_spark_push(n) / 1'000'000.0;
    auto spark_resize = benchmark_spark_resize_clear(n) / 1'000'000.0;
    auto spark_swap = benchmark_spark_swap_pop(n) / 1'000'000.0;

    auto vector_push = benchmark_vector_push(n) / 1'000'000.0;
    auto vector_resize = benchmark_vector_resize_clear(n) / 1'000'000.0;
    auto vector_swap = benchmark_vector_swap_pop(n) / 1'000'000.0;

    std::cout << "spark::list:\n";
    std::cout << "Push:       " << spark_push << " ms\n";
    std::cout << "Resize/Clear: " << spark_resize << " ms\n";
    std::cout << "Swap/Pop:   " << spark_swap << " ms\n\n";

    std::cout << "std::vector:\n";
    std::cout << "Push:       " << vector_push << " ms\n";
    std::cout << "Resize/Clear: " << vector_resize << " ms\n";
    std::cout << "Swap/Pop:   " << vector_swap << " ms\n";

    return 0;
}