cmake_minimum_required(VERSION 3.21)

project(spark-engine)

function(enable_project_optimisations target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:/O2 /Ot /Ob2 /DNDEBUG>
        )
    else()
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:-O3 -flto -DNDEBUG>
        )
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Release>:-flto>
        )
    endif()
endfunction()

add_subdirectory(spark-engine)
add_subdirectory(spark-test)
