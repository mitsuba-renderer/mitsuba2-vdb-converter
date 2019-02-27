include(CheckCXXCompilerFlag)

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/openvdb")
  message(FATAL_ERROR "OpenVDB not found. The project has to be cloned using --recursive (or call git submodule update --init --recursive) ")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

  if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

add_subdirectory(openvdb)
set_property(TARGET openvdb_static PROPERTY FOLDER "dependencies")
set(OPENVDB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openvdb)
set(OPENVDB_INCLUDE_DIR ${OPENVDB_INCLUDE_DIR} PARENT_SCOPE)
set(OPENVDB_LIBRARY openvdb_static PARENT_SCOPE)
set(OPENVDB_LIBRARY openvdb_static)
set(OPENVDB_LIBRARY ${OPENVDB_LIBRARY} PARENT_SCOPE)