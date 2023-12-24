cmake_minimum_required(VERSION 3.8)
project(ST3D_project VERSION 1.0)

include(FetchContent)

# Check if GLFW is installed
find_package(glfw3 QUIET)
if(glfw3_FOUND)
  include_directories(${glfw3_INCLUDE_DIR})
else()
  # Download and include GLFW
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.4
    SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW
  )
  FetchContent_MakeAvailable(glfw)
  include_directories(${glfw_SOURCE_DIR}/include)
endif()

# Check if Vulkan is installed
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
  include_directories(${Vulkan_INCLUDE_DIRS})
else()
  # Download and include Vulkan
  FetchContent_Declare(
    vulkan
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
    GIT_TAG        v1.3.268
    SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/vulkan
  )
  FetchContent_MakeAvailable(vulkan)
endif()

#include directories

include_directories(inc)

add_executable(ST3D_project src/main.c)