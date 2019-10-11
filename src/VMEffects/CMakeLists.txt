
cmake_minimum_required(VERSION 3.12)
project(VMEffects)

aux_source_directory(. SRC)

add_library(VMEffects)
target_sources(VMEffects PRIVATE ${SRC})
target_link_libraries(VMEffects vmcore)
target_include_directories(VMEffects PUBLIC "../../include")

install(TARGETS VMEffects LIBRARY DESTINATION "lib" RUNTIME DESTINATION "bin" ARCHIVE DESTINATION "lib")
install(DIRECTORY "../../include/VMEffects" DESTINATION "include")
