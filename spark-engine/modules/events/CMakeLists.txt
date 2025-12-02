add_library(spark_events INTERFACE)

target_include_directories(spark_events INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
) 

target_link_libraries(spark_events INTERFACE
    spark::events
)

add_library(spark::events ALIAS spark_events)