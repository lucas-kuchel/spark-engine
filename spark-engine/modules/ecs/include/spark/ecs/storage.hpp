#pragma once

#include <cstring>

#include <spark/ecs/sparse_set.hpp>

namespace spark::ecs {
    class sparse_set_storage {
    public:
        sparse_set_storage() = default;

        ~sparse_set_storage() {
            if (destructor_) {
                destructor_(storage_);
                destructor_ = nullptr;
                remove_ = nullptr;
                size_ = nullptr;
                entities_ = nullptr;
                contains_ = nullptr;
            }
        }

        sparse_set_storage(const sparse_set_storage&) = delete;

        sparse_set_storage(sparse_set_storage&& other) noexcept {
            std::memcpy(storage_, other.storage_, sizeof(storage_));

            destructor_ = other.destructor_;
            remove_ = other.remove_;
            size_ = other.size_;
            entities_ = other.entities_;
            contains_ = other.contains_;

            other.destructor_ = nullptr;
            other.remove_ = nullptr;
            other.size_ = nullptr;
            other.entities_ = nullptr;
            other.contains_ = nullptr;
        }

        sparse_set_storage& operator=(const sparse_set_storage&) = delete;

        sparse_set_storage& operator=(sparse_set_storage&& other) noexcept {
            std::memcpy(storage_, other.storage_, sizeof(storage_));

            destructor_ = other.destructor_;
            remove_ = other.remove_;
            size_ = other.size_;
            entities_ = other.entities_;
            contains_ = other.contains_;

            other.destructor_ = nullptr;
            other.remove_ = nullptr;
            other.size_ = nullptr;
            other.entities_ = nullptr;
            other.contains_ = nullptr;

            return *this;
        }

        template <typename T>
        [[nodiscard]] constexpr sparse_set<T>& get() {
            return *reinterpret_cast<sparse_set<T>*>(&storage_);
        }

        template <typename T>
        [[nodiscard]] constexpr const sparse_set<T>& get() const {
            return *reinterpret_cast<const sparse_set<T>*>(&storage_);
        }

        template <typename T>
        constexpr void init() {
            ::new (storage_) sparse_set<T>();

            destructor_ = destruct<T>;
            remove_ = remove<T>;
            size_ = size<T>;
            entities_ = entities<T>;
            contains_ = contains<T>;
        }

        [[nodiscard]] constexpr bool needs_init() const {
            return destructor_ == nullptr;
        }

        constexpr void remove(std::size_t index) {
            remove_(storage_, index);
        }

        constexpr std::size_t size() const {
            return size_(storage_);
        }

        constexpr const std::vector<entity::size_type>& entities() {
            return entities_(storage_);
        }

        constexpr bool contains(entity::size_type index) const {
            return contains_(storage_, index);
        }

    private:
        using destructor_type = void (*)(void*);
        using size_type = std::size_t (*)(const void*);
        using remove_type = void (*)(void*, std::size_t);
        using entities_type = const std::vector<entity::size_type>& (*)(void*);
        using contains_type = bool (*)(const void*, entity::size_type);

        alignas(sparse_set<void*>) std::byte storage_[sizeof(sparse_set<void*>)];

        destructor_type destructor_ = nullptr;
        remove_type remove_ = nullptr;
        size_type size_ = nullptr;
        entities_type entities_ = nullptr;
        contains_type contains_ = nullptr;

        template <typename T>
        constexpr static void destruct(void* storage) {
            reinterpret_cast<sparse_set<T>*>(storage)->~sparse_set<T>();
        }

        template <typename T>
        constexpr static void remove(void* storage, std::size_t index) {
            auto& set = *reinterpret_cast<sparse_set<T>*>(storage);

            if (set.contains(index)) {
                set.remove(index);
            }
        }

        template <typename T>
        constexpr static std::size_t size(const void* storage) {
            auto& set = *reinterpret_cast<const sparse_set<T>*>(storage);

            return set.size();
        }

        template <typename T>
        constexpr static const std::vector<entity::size_type>& entities(void* storage) {
            auto& set = *reinterpret_cast<sparse_set<T>*>(storage);

            return set.entities();
        }

        template <typename T>
        constexpr static bool contains(const void* storage, entity::size_type index) {
            auto& set = *reinterpret_cast<const sparse_set<T>*>(storage);

            return set.contains(index);
        }
    };
}