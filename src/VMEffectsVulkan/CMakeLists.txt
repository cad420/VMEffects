
cmake_minimum_required(VERSION 3.12)
project(VMEffectsVk)


find_package(Vulkan REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

aux_source_directory(. SRC)
aux_source_directory(VulkanWrapper WRAPPER_SRC)

add_library(VMEffectsVk SHARED)
target_sources(VMEffectsVk PRIVATE ${SRC} ${WRAPPER_SRC})
target_link_libraries(VMEffectsVk VMEffects vmcore ${Vulkan_LIBRARIES} glfw)
target_include_directories(VMEffectsVk PUBLIC "../../include")
target_include_directories(VMEffectsVk PRIVATE "../VMEffects")
target_include_directories(VMEffectsVk PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(VMEffectsVk PRIVATE ${glfw_INCLUDE_DIRS})

install(TARGETS VMEffectsVk LIBRARY DESTINATION "lib" RUNTIME DESTINATION "bin/backends" ARCHIVE DESTINATION "lib")
install(DIRECTORY "../../include/VMEffectsVulkan" DESTINATION "include")
