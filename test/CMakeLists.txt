
cmake_minimum_required(VERSION 3.12)
project(test)

aux_source_directory(. SRC)

find_package(glfw3 CONFIG REQUIRED)

add_executable(main ${SRC})
target_sources(main PRIVATE ${SRC})
target_link_libraries(main PRIVATE vmcore VMEffects VMEffectsVk glfw)
target_include_directories(main PRIVATE "../include" ${glfw_INCLUDE_DIRS})

install(TARGETS main LIBRARY DESTINATION "lib" RUNTIME DESTINATION "bin" ARCHIVE DESTINATION "lib")
