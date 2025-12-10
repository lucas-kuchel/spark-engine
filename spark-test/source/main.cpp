#include <entt/entt.hpp>
#include <spark/spark.hpp>

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdlib>

struct Entity {
    float health;
};

struct Position {
    float x;
    float y;
    float z;
};

struct BaseTag {
};

std::atomic<std::size_t> total_alloc = 0;
std::atomic<std::size_t> total_dealloc = 0;

void* operator new(std::size_t sz) {
    total_alloc += sz;
    void* ptr = std::malloc(sz);
    if (!ptr)
        throw std::bad_alloc{};
    return ptr;
}

void operator delete(void* ptr, std::size_t sz) noexcept {
    total_dealloc += sz;
    std::free(ptr);
}

constexpr std::size_t MAX = 1'048'575;

template <typename Fn>
double time(Fn&& fn) {
    auto start = std::chrono::high_resolution_clock::now();
    fn();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

template <typename Registry>
double create_test(Registry& registry) {
    return time([&]() {
        for (std::size_t i = 0; i < MAX; i++) {
            auto e = registry.create();
            registry.template emplace<BaseTag>(e);
        }
    });
}

template <typename Registry, typename Fn>
double create_then_time(Registry& registry, Fn&& fn) {
    create_test<Registry>(registry);

    auto start = std::chrono::high_resolution_clock::now();
    fn();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

template <typename Registry, typename E>
double destroy_test(Registry& registry) {
    return time([&]() {
        std::vector<E> entities;

        for (auto entity : registry.template view<BaseTag>()) {
            entities.push_back(entity);
        }

        for (auto entity : entities) {
            registry.destroy(entity);
        }
    });
}

// Iteration test
template <typename Registry, typename Component>
double iteration_test(Registry& registry) {
    return time([&]() {
        auto view = registry.template view<Component>();
        for (auto [e, c] : view.each()) {
            c.x += 2.0;
        }
    });
}

// Component add/remove test
template <typename Registry, typename Component>
double add_test(Registry& registry) {
    return time([&]() {
        for (auto entity : registry.template view<BaseTag>()) {
            registry.template emplace<Component>(entity);
        }
    });
}

template <typename Registry, typename Component>
double remove_test(Registry& registry) {
    return time([&]() {
        for (auto entity : registry.template view<BaseTag>()) {
            registry.template remove<Component>(entity);
        }
    });
}

int main() {
    entt::registry entt;
    spark::registry spark;

    spark::println("entt create + tag test: {} ms", create_test(entt));
    spark::println("spark create + tag test: {} ms", create_test(spark));

    spark::println("entt add test: {} ms", add_test<entt::registry, Position>(entt));
    spark::println("spark add test: {} ms", add_test<spark::registry, Position>(spark));

    spark::println("entt iteration test: {} ms", iteration_test<entt::registry, Position>(entt));
    spark::println("spark iteration test: {} ms", iteration_test<spark::registry, Position>(spark));

    spark::println("entt remove test: {} ms", remove_test<entt::registry, Position>(entt));
    spark::println("spark remove test: {} ms", remove_test<spark::registry, Position>(spark));

    spark::println("entt destroy test: {} ms", destroy_test<entt::registry, entt::entity>(entt));
    spark::println("spark destroy test: {} ms", destroy_test<spark::registry, spark::entity>(spark));

    spark::println("total allocated: {} bytes", total_alloc.load());
    spark::println("total deallocated: {} bytes", total_dealloc.load());
}