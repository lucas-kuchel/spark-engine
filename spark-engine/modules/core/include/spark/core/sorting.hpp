#pragma once

#include <spark/core/primitives.hpp>
#include <spark/core/utility.hpp>

namespace spark::core {
    // @brief simple comparison-based sorting algorithm with O(n^2) time complexity
    // @note stable, in-place, and best suited for very small datasets
    struct bubble_sort {
        template <typename T, typename C>
        static void sort(T* data, uint64 size) {
            if (size < 2) {
                return;
            }

            for (uint64 i = 0; i < size; i++) {
                for (uint64 j = 0; j + 1 < size - i; j++) {
                    if (C::compare(data[j + 1], data[j])) {
                        T temporary = move(data[j]);

                        data[j] = move(data[j + 1]);
                        data[j + 1] = move(temporary);
                    }
                }
            }
        }
    };

    // @brief sorting algorithm that repeatedly selects the smallest remaining element
    // @note in-place, not stable, always O(n^2) regardless of input order
    struct selection_sort {
        template <typename T, typename C>
        static void sort(T* data, uint64 size) {
            if (size < 2) {
                return;
            }

            for (uint64 i = 0; i < size; i++) {
                uint64 minIndex = i;

                for (uint64 j = i + 1; j < size; j++) {
                    if (C::compare(data[j], data[minIndex])) {
                        minIndex = j;
                    }
                }
                if (i != minIndex) {
                    T temporary = move(data[i]);

                    data[i] = move(data[minIndex]);
                    data[minIndex] = move(temporary);
                }
            }
        }
    };

    // @brief efficient insertion-based sorting algorithm for small or nearly-sorted ranges
    // @note stable, in-place, O(n) best case, O(n^2) worst case
    struct insertion_sort {
        template <typename T, typename C>
        static void sort(T* data, uint64 size) {
            if (size < 2) {
                return;
            }

            for (uint64 i = 1; i < size; i++) {
                T key = move(data[i]);

                uint64 j = i;

                while (j > 0 && C::compare(key, data[j - 1])) {
                    data[j] = move(data[j - 1]);
                    --j;
                }

                data[j] = move(key);
            }
        }
    };

    // @brief bidirectional variant of bubble sort that sweeps from both ends
    // @note in-place, stable, slightly faster than bubble sort on some datasets
    struct cocktail_sort {
        template <typename T, typename C>
        static void sort(T* data, uint64 size) {
            if (size < 2) {
                return;
            }

            bool swapped = true;
            uint64 start = 0;
            uint64 end = size - 1;

            while (swapped) {
                swapped = false;

                for (uint64 i = start; i < end; i++) {
                    if (C::compare(data[i + 1], data[i])) {
                        T temporary = move(data[i]);
                        data[i] = move(data[i + 1]);
                        data[i + 1] = move(temporary);
                        swapped = true;
                    }
                }

                if (!swapped) {
                    break;
                }

                swapped = false;
                --end;

                for (uint64 i = end; i > start; i--) {
                    if (C::compare(data[i], data[i - 1])) {
                        T temporary = move(data[i]);
                        data[i] = move(data[i - 1]);
                        data[i - 1] = move(temporary);
                        swapped = true;
                    }
                }

                ++start;
            }
        }
    };

    // @brief comparator for less-than comparisons
    class less {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a < b;
        }
    };

    // @brief comparator for greater-than comparisons
    class greater {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a > b;
        }
    };

    // @brief comparator for less-or-equal comparisons
    class less_equal {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a <= b;
        }
    };

    // @brief comparator for greater-or-equal comparisons
    class greater_equal {
    public:
        template <typename T>
        static bool compare(T& a, T& b) {
            return a >= b;
        }
    };
}