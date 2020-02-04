
cmake_minimum_required(VERSION 3.12)
project(VMEffects)

include(scripts/external.cmake)

vm_external_module(GIT_REPOSITORY https://github.com/cad420/VMCore.git GIT_TAG master)


option(VMEFFECTS_BUILD_GLSLANG "set on to integrate glslang" ON)
if(VMEFFECTS_BUILD_GLSLANG)
add_subdirectory(3rdparty/glslang)
endif()
add_subdirectory(src)
add_subdirectory(test)
