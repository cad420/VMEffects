

add_subdirectory(VMEffects)
if(WIN32 OR UNIX OR APPLE)
add_subdirectory(VMEffectsVulkan)
endif()