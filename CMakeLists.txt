cmake_minimum_required(VERSION 3.8)
project(ST3D_project VERSION 1.0 )

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/GLFW)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_executable(ST3D_project src/main.c)
target_link_libraries(ST3D_project glfw ${Vulkan_LIBRARIES})