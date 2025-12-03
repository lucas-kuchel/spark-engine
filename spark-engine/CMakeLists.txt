add_library(spark_spark INTERFACE)

add_subdirectory(modules/core)
add_subdirectory(modules/ecs)
add_subdirectory(modules/events)

target_include_directories(spark_spark INTERFACE
    "include"
)

target_link_libraries(spark_spark INTERFACE
    spark::core
    spark::ecs
    spark::events
)

add_library(spark::spark ALIAS spark_spark)