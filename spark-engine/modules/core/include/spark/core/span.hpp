#pragma once

#include <spark/core/primitives.hpp>
#include <spark/core/traits.hpp>
#include <spark/core/utility.hpp>

namespace spark::core {
    // @brief non-owning view over contiguous memory
    template <typename T, typename U = uint64>
    requires(is_unsigned<U>)
    class span {
    public:
        using size_type = U;
        using type = T;

        span() = default;

        ~span() {
            data_ = nullptr;
            size_ = 0;
        }

        span(type* ptr, size_type size)
            : data_(ptr), size_(size) {
        }

        span(const span& other)
            : data_(other.data_), size_(other.size_) {
        }

        span(span&& other)
            : data_(other.data_), size_(other.size_) {
            other.data_ = nullptr;
            other.size_ = 0;
        }

        span& operator=(const span& other) {
            data_ = other.data_;
            size_ = other.size_;
        }

        span& operator=(span&& other) {
            data_ = other.data_;
            size_ = other.size_;

            other.data_ = nullptr;
            other.size_ = 0;
        }

        type& operator[](size_type index) {
            return data_[index];
        }

        const type& operator[](size_type index) const {
            return data_[index];
        }

        // @brief provides a span over this span
        // @param the offset into this span to start from
        // @param the size of the product span
        span subspan(size_type offset, size_type size) {
            return span<T>(data_ + offset, size);
        }

        // @brief provides a span over this span
        // @param the offset into this span to start from
        // @param the size of the product span
        span subspan(size_type offset, size_type size) const {
            return span<T>(data_ + offset, size);
        }

        // @brief sorts elements with a provided algorithm and condition
        template <typename S, typename C>
        void sort() {
            S::template sort<T, C>(data_, size_);
        }

        // @brief swaps the elements at the provided locations
        void swap(size_type a, size_type b) {
            if (a == b) {
                return;
            }

            type temporary = data_[a];
            data_[a] = data_[b];
            data_[b] = temporary;
        }

        // @brief checks if the span is currently viewing valid memory
        bool valid() const {
            return data_ != nullptr && size_ > 0;
        }

        // @brief provides access to the raw memory pointer
        type* data() {
            return data_;
        }

        // @brief provides access to the raw memory pointer
        const type* data() const {
            return data_;
        }

        // @brief provides the first element in the list
        type& first() {
            return *data_;
        }

        // @brief provides the last element in the list
        type& last() {
            return *(data_ + size_ - 1);
        }

        // @brief provides the first element in the list
        const type& first() const {
            return *data_;
        }

        // @brief provides the last element in the list
        const type& last() const {
            return *(data_ + size_ - 1);
        }

        type* begin() {
            return data_;
        }

        type* end() {
            return data_ + size_;
        }

        const type* begin() const {
            return data_;
        }

        const type* end() const {
            return data_ + size_;
        }

    public:
    private:
        type* data_ = nullptr;
        size_type size_ = 0;
    };
}