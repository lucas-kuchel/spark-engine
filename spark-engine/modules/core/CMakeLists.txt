add_library(spark_core INTERFACE)

target_include_directories(spark_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
) 

add_library(spark::core ALIAS spark_core)