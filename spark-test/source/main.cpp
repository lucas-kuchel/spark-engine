#include <spark/spark.hpp>

int main() {
    spark::math::vector2<float> a(2, 3);
    spark::math::vector2<float> b(8, 7);

    spark::math::vector2<int> c = b;

    spark::core::println("[{}, {}]", c[0], c[1]);
}