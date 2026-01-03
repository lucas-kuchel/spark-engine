cmake_minimum_required(VERSION 3.21)

set(SPARK_VERSION_MAJOR 0)
set(SPARK_VERSION_MINOR 0)
set(SPARK_VERSION_PATCH 0)

project(spark LANGUAGES CXX VERSION ${SPARK_VERSION_MAJOR}.${SPARK_VERSION_MINOR}.${SPARK_VERSION_PATCH})

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

define_property(
    GLOBAL PROPERTY SPARK_MODULE_TARGETS
    BRIEF_DOCS "Spark module targets"
    FULL_DOCS "All Spark submodule targets"
)

function(create_submodule name)
    set(target ${name})

    add_library(${target} INTERFACE)
    add_library(spark::${name} ALIAS ${target})

    target_include_directories(${target}
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    string(TOUPPER ${name} NAME_UPPER)

    target_compile_definitions(${target}
        INTERFACE SPARK_${NAME_UPPER}_AVAILABLE
    )

    set_property(GLOBAL APPEND PROPERTY SPARK_MODULE_TARGETS ${target})
endfunction()

function(add_module name)
    string(TOUPPER ${name} NAME_UPPER)
    option(SPARK_ENABLE_${NAME_UPPER} "Enable Spark ${name} module" ON)

    if(NOT SPARK_ENABLE_${NAME_UPPER})
        return()
    endif()

    if(DEFINED SPARK_${NAME_UPPER}_FOUND AND NOT SPARK_${NAME_UPPER}_FOUND)
        message(STATUS "Spark module '${name}' disabled (dependencies not found)")
        return()
    endif()

    add_subdirectory(modules/${name})
endfunction()

add_library(spark INTERFACE)
add_library(spark::spark ALIAS spark)

target_compile_definitions(spark
    INTERFACE
    SPARK_VERSION_MAJOR=${SPARK_VERSION_MAJOR}
    SPARK_VERSION_MINOR=${SPARK_VERSION_MINOR}
    SPARK_VERSION_PATCH=${SPARK_VERSION_PATCH}
)

target_include_directories(spark
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_module(ecs)
add_module(events)
add_module(graphics)
add_module(math)
add_module(utility)

get_property(SPARK_MODULE_TARGETS GLOBAL PROPERTY SPARK_MODULE_TARGETS)

if(SPARK_MODULE_TARGETS)
    target_link_libraries(spark INTERFACE ${SPARK_MODULE_TARGETS})
endif()

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    TARGETS spark ${SPARK_MODULE_TARGETS}
    EXPORT sparkTargets
)

install(
    EXPORT sparkTargets
    NAMESPACE spark::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/spark
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/sparkConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/sparkConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/spark
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/sparkConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/spark
)
