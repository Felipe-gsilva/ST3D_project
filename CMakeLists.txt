cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME ST3D_project)
 
message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()
 
project(${PROJECT_NAME} C CXX)

if(UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic  -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor")
endif()  

if(WIN32)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror -lglfw3 -lopengl32 -lgdi32")
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2019") # 2.1 Update lib-vc2019 to use same version as your visual studio
  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64") # 2.1 make sure matches glfw mingw subdirectory
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
    message(FATAL_ERROR "Could not find glfw library!")
else()
    message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()


if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") 
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED) 
  message(STATUS "Found Vulkan")
endif()
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# get source files
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
 
if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
 
  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/core/render
    ${PROJECT_SOURCE_DIR}/src/core/logger
    ${CGLM_PATH}
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    )
 
  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )
 
  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${PROJECT_SOURCE_DIR}/src/core
      ${PROJECT_SOURCE_DIR}/src/core/render
      ${PROJECT_SOURCE_DIR}/src/core/logger
      ${CGLM_PATH}
      ${Vulkan_INCLUDE_DIRS}
      ${GLFW_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
endif()


set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)

file(GLOB SHADERS
  ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.comp ${SHADER_DIR}/*.geom ${SHADER_DIR}/*.tesc ${SHADER_DIR}/*.tese ${SHADER_DIR}/*.mesh ${SHADER_DIR}/*.task ${SHADER_DIR}/*.rgen ${SHADER_DIR}/*.rchit ${SHADER_DIR}/*.rmiss
)

foreach(SHADER IN LISTS SHADERS)
  get_filename_component(FILENAME ${SHADER} NAME)
  add_custom_command(
    OUTPUT ${SHADER_DIR}/${FILENAME}.spv
    COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_DIR}/${FILENAME}.spv
    DEPENDS ${SHADER}
    COMMENT "Compiling ${FILENAME}"
  )
  list(APPEND SPV_SHADERS ${SHADER_DIR}/${FILENAME}.spv)
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPV_SHADERS})

add_dependencies(${PROJECT_NAME} shaders)