function(declare_submodule name alias)
    add_library(${name} INTERFACE)

    target_include_directories(${name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${name} INTERFACE ${ARGN})

    add_library(${alias} ALIAS ${name})
endfunction()

add_subdirectory(modules/core)
add_subdirectory(modules/ecs)
add_subdirectory(modules/events)
add_subdirectory(modules/math)

declare_submodule(spark_spark spark::spark
    spark::core
    spark::ecs
    spark::events
    spark::math
)
