add_library(spark_ecs INTERFACE)

target_include_directories(spark_ecs INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
) 

target_link_libraries(spark_ecs INTERFACE
    spark::core
)

add_library(spark::ecs ALIAS spark_ecs)