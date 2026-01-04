#pragma once

#if SPARK_SUPPORTED

#include "memory.hpp"
#include "pipeline.hpp"

namespace spark::events {
    class pipeline_delegate {
    public:
        pipeline_delegate() noexcept = default;

        ~pipeline_delegate() noexcept {
            if (destructor_ != nullptr) {
                destructor_(&memory_);

                destructor_ = nullptr;
                reset_ = nullptr;
                dispatch_ = nullptr;
                move_ = nullptr;
            }
        }

        pipeline_delegate(const pipeline_delegate& other) = delete;

        pipeline_delegate(pipeline_delegate&& other) noexcept {
            if (other) {
                other.move_(&memory_, &other.memory_);

                destructor_ = other.destructor_;
                reset_ = other.reset_;
                dispatch_ = other.dispatch_;
                move_ = other.move_;

                other.destructor_ = nullptr;
                other.reset_ = nullptr;
                other.dispatch_ = nullptr;
                other.move_ = nullptr;
            }
        }

        pipeline_delegate& operator=(const pipeline_delegate& other) = delete;

        pipeline_delegate& operator=(pipeline_delegate&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            if (destructor_ != nullptr) {
                destructor_(&memory_);
            }

            destructor_ = nullptr;
            reset_ = nullptr;
            dispatch_ = nullptr;
            move_ = nullptr;

            if (other) {
                other.move_(&memory_, &other.memory_);

                destructor_ = other.destructor_;
                reset_ = other.reset_;
                dispatch_ = other.dispatch_;
                move_ = other.move_;

                other.destructor_ = nullptr;
                other.reset_ = nullptr;
                other.dispatch_ = nullptr;
                other.move_ = nullptr;
            }

            return *this;
        }

        template <typename T>
        void construct() noexcept {
            destructor_ = destruct<T>;
            reset_ = reset<T>;
            dispatch_ = dispatch<T>;
            move_ = move<T>;

            ::new (&memory_) pipeline<T>();
        }

        template <typename T>
        [[nodiscard]] pipeline<T>& acquire() noexcept {
            return cast<T>(&memory_);
        }

        template <typename T>
        [[nodiscard]] const pipeline<T>& acquire() const noexcept {
            return cast<T>(&memory_);
        }

        void reset() noexcept {
            reset_(&memory_);
        }

        void dispatch() noexcept {
            dispatch_(&memory_);
        }

        [[nodiscard]] explicit operator bool() const {
            return destructor_ != nullptr;
        }

    private:
        using pipeline_storage = pipeline<void*>;

        memory_of<pipeline_storage> memory_;

        void (*destructor_)(void*) = nullptr;
        void (*reset_)(void*) = nullptr;
        void (*dispatch_)(void*) = nullptr;
        void (*move_)(void*, void*) = nullptr;

        template <typename T>
        static pipeline<T>& cast(void* memory) noexcept {
            return *std::launder(reinterpret_cast<pipeline<T>*>(memory));
        }

        template <typename T>
        static const pipeline<T>& cast(const void* memory) noexcept {
            return *std::launder(reinterpret_cast<const pipeline<T>*>(memory));
        }

        template <typename T>
        static void move(void* destination, void* source) noexcept {
            new (destination) pipeline<T>(std::move(cast<T>(source)));
        }

        template <typename T>
        static void destruct(void* memory) noexcept {
            pipeline<T>& pipeline = cast<T>(memory);

            pipeline.~pipeline<T>();
        }

        template <typename T>
        static void reset(void* memory) noexcept {
            pipeline<T>& pipeline = cast<T>(memory);

            pipeline.reset();
        }

        template <typename T>
        static void dispatch(void* memory) noexcept {
            pipeline<T>& pipeline = cast<T>(memory);

            pipeline.dispatch();
        }
    };
}

#endif