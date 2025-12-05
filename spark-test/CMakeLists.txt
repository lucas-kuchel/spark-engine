file(GLOB_RECURSE SOURCES "source/*.cpp")

add_executable(spark-test)

target_sources(spark-test PRIVATE
    ${SOURCES}
)

target_include_directories(spark-test PRIVATE
    "include"
)


include(FetchContent)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.16.0
)

FetchContent_MakeAvailable(entt)



target_link_libraries(spark-test PRIVATE
    spark::spark
    EnTT::EnTT
)

enable_project_optimisations(spark-test)
