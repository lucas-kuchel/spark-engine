file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(spark-test)

target_sources(spark-test PRIVATE 
    ${SOURCES}
)

target_include_directories(spark-test PRIVATE
    "include"
)

target_link_libraries(spark-test PRIVATE
    spark::spark
)