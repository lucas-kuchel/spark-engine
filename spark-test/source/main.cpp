#include <spark/spark.hpp>

int main() {
    spark::math::matrix4x4<float> a = spark::math::matrix4x4<float>::identity();
    spark::math::matrix4x4<float> b = spark::math::matrix4x4<float>::identity();

    for (std::size_t c = 0; c < 4; c++) {
        spark::core::println("[{} {} {} {}]", a(0, c), a(1, c), a(2, c), a(3, c));
    }

    spark::core::println();

    for (std::size_t c = 0; c < 4; c++) {
        spark::core::println("[{} {} {} {}]", b(0, c), b(1, c), b(2, c), b(3, c));
    }
}