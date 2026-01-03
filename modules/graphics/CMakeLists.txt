create_submodule(graphics)

target_link_libraries(spark_graphics
    INTERFACE Vulkan::Vulkan
)
