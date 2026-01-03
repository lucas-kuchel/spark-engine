create_submodule(graphics)

target_link_libraries(graphics
    INTERFACE Vulkan::Vulkan
)
