#include <chrono>
#include <entt/entt.hpp>
#include <print>
#include <spark/spark.hpp>

static constexpr int Iterations = 100'000'000;

struct Event {
    float val;
};

void handler(const Event& e) {
    (void)e; // avoid dead-code elimination
}

template <typename F>
double timeit(F&& fn) {
    auto begin = std::chrono::steady_clock::now();
    fn();
    auto end = std::chrono::steady_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
    return ns / 1'000'000.0; // convert to milliseconds as double
}

int main() {
    std::println("iterations: {}\n", Iterations);

    {
        std::println("=== Immediate Dispatch Benchmark ===");

        spark::dispatcher sdisp;
        entt::dispatcher edisp;

        sdisp.sink<Event>().connect<handler>();
        edisp.sink<Event>().connect<handler>();

        auto e_time = timeit([&] {
            for (int i = 0; i < Iterations; i++)
                edisp.trigger<Event>(1.0f);
        });

        auto s_time = timeit([&] {
            for (int i = 0; i < Iterations; i++)
                sdisp.trigger<Event>(1.0f);
        });

        std::println("spark::dispatcher: {} ms", s_time);
        std::println("entt::dispatcher : {} ms", e_time);

        std::println("spark::dispatcher [average call]: {} ns", (s_time * 1000000.0) / Iterations);
        std::println("entt::dispatcher  [average call]: {} ns", (e_time * 1000000.0) / Iterations);
    }

    {
        std::println("\n=== Queued Dispatch Benchmark ===");

        spark::dispatcher sdisp;
        entt::dispatcher edisp;

        sdisp.sink<Event>().connect<handler>();
        edisp.sink<Event>().connect<handler>();

        auto e_time = timeit([&] {
            for (int i = 0; i < Iterations; i++)
                edisp.enqueue<Event>(1.0f);
            edisp.update<Event>();
        });

        auto s_time = timeit([&] {
            for (int i = 0; i < Iterations; i++)
                sdisp.enqueue<Event>(1.0f);
            sdisp.update();
        });

        std::println("spark::dispatcher: {} ms", s_time);
        std::println("entt::dispatcher : {} ms", e_time);

        std::println("spark::dispatcher [average call]: {} ns", (s_time * 1000000.0) / Iterations);
        std::println("entt::dispatcher  [average call]: {} ns", (e_time * 1000000.0) / Iterations);
    }

    {
        std::println("\n=== Connect/Disconnect Benchmark ===");

        entt::dispatcher ed;
        auto ec_time = timeit([&] {
            for (int i = 0; i < Iterations; i++) {
                ed.sink<Event>().connect<handler>();
            }
        });

        spark::dispatcher sd;
        auto sc_time = timeit([&] {
            for (int i = 0; i < Iterations; i++) {
                sd.sink<Event>().connect<handler>();
            }
        });

        auto ed_time = timeit([&] {
            for (int i = 0; i < Iterations; i++) {
                ed.sink<Event>().disconnect<handler>();
            }
        });

        auto sd_time = timeit([&] {
            for (int i = 0; i < Iterations; i++) {
                sd.sink<Event>().disconnect<handler>();
            }
        });

        std::println("spark::dispatcher [connect]: {} ms", sc_time);
        std::println("entt::dispatcher  [connect]: {} ms", ec_time);

        std::println("spark::dispatcher [disconnect]: {} ms", sd_time);
        std::println("entt::dispatcher  [disconnect]: {} ms", ed_time);

        std::println("spark::dispatcher [average call] [connect]: {} ns", (sc_time * 1000000.0) / Iterations);
        std::println("entt::dispatcher  [average call] [connect]: {} ns", (ec_time * 1000000.0) / Iterations);

        std::println("spark::dispatcher [average call] [disconnect]: {} ns", (sd_time * 1000000.0) / Iterations);
        std::println("entt::dispatcher  [average call] [disconnect]: {} ns", (ed_time * 1000000.0) / Iterations);
    }

    return 0;
}
