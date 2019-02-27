cmake_minimum_required(VERSION 3.2.0)
project(volumeconverter)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(MSVC)
  add_definitions(/D _CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(ext ext_build)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
endif()

add_executable(convertvdb main.cpp)
target_link_libraries(convertvdb LINK_PRIVATE IlmImf tbb glfw blosc openvdb_static)


