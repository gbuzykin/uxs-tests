cmake_minimum_required(VERSION 3.15)
project(
  uxs-tests
  DESCRIPTION "Project for UXS library testing"
  LANGUAGES CXX)

include(ExternalProject)
include(GenerateExportHeader)

option(USE_LIBCPP "Use LibC++" OFF)
option(USE_SANITIZERS_FOR_DEBUG "Use Sanitizers for Debug build" ON)
option(OPTION_EXPORT_COMPILE_DEFS_AND_INCLUDE_DIRS
       "Export compile definitions and include directories" OFF)

message(
  "Using ${CMAKE_CXX_COMPILER} C++ compiler (BUILD_TYPE=${CMAKE_BUILD_TYPE})")
message("CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message("USE_LIBCPP = ${USE_LIBCPP}")

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Using C++${CMAKE_CXX_STANDARD} standard")

foreach(
  var_name
  CMAKE_C_COMPILER
  CMAKE_C_FLAGS
  CMAKE_CXX_COMPILER
  CMAKE_CXX_STANDARD
  CMAKE_CXX_FLAGS
  CMAKE_SHARED_LINKER_FLAGS
  CMAKE_BUILD_TYPE)
  if(DEFINED ${var_name})
    list(APPEND EXT_PROJECT_CMAKE_ARGS "-D${var_name}=${${var_name}}")
  endif()
endforeach()

# ##############################################################################
# Prepare `uxs` library

set(UXS_LIBRARY_NAME $<IF:$<BOOL:${WIN32}>,uxs.lib,libuxs.so>)
ExternalProject_Add(
  uxs
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/uxs
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/uxs
  BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/uxs/lib/${UXS_LIBRARY_NAME}
  CMAKE_ARGS
    -DUSE_ZLIB=On #
    -DUSE_LIBZIP=On #
    -DUSE_LIBCPP=${USE_LIBCPP} #
    -DUSE_SANITIZERS_FOR_DEBUG=${USE_SANITIZERS_FOR_DEBUG} #
    -DOPTION_EXPORT_COMPILE_DEFS_AND_INCLUDE_DIRS=${OPTION_EXPORT_COMPILE_DEFS_AND_INCLUDE_DIRS} #
    -DZLIB_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/zlib #
    -DLIBZIP_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/libzip #
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> #
    ${EXT_PROJECT_CMAKE_ARGS})
ExternalProject_Get_Property(uxs INSTALL_DIR)
set(UXS_INCLUDE_DIR ${INSTALL_DIR}/include)
set(UXS_LIBRARY ${INSTALL_DIR}/lib/${UXS_LIBRARY_NAME})

# ##############################################################################
# Compilation flags

if(MSVC)
  add_compile_options(/Zc:__cplusplus /utf-8 /WX)
else()
  add_compile_options(-Wall -pedantic -Werror)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_STANDARD LESS 17)
      add_compile_options(-Wno-ignored-attributes)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options($<$<CONFIG:Release>:-Wno-array-bounds>)
    add_compile_options($<$<CONFIG:Release>:-Wno-nonnull>)
  endif()
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options($<$<CONFIG:Debug>:-fstandalone-debug>)
    if(USE_LIBCPP)
      add_compile_options(-stdlib=libc++)
      add_link_options(-stdlib=libc++)
    endif()
  endif()
endif()

if(USE_SANITIZERS_FOR_DEBUG)
  message("Using sanitizers for Debug build")
  if(MSVC)
    add_compile_options($<$<CONFIG:Debug>:/fsanitize=address>)
  else()
    add_compile_options($<$<CONFIG:Debug>:-fsanitize=address,undefined,leak>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=address,undefined,leak>)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      add_link_options($<$<CONFIG:Debug>:-shared-libsan>)
    endif()
  endif()
endif()

# ##############################################################################
# Add `uxs-legacy` build target

file(GLOB_RECURSE uxs_legacy_headers uxs-legacy/include/*.h)
file(GLOB_RECURSE uxs_legacy_sources uxs-legacy/src/*.h;uxs-legacy/src/*.cpp)

add_library(uxs-legacy SHARED .clang-format ${uxs_legacy_headers}
                              ${uxs_legacy_sources})

add_dependencies(uxs-legacy uxs)

generate_export_header(
  uxs-legacy EXPORT_FILE_NAME
  ${PROJECT_BINARY_DIR}/generated/uxs-legacy/config.h INCLUDE_GUARD_NAME
  UXS_LEGACY_CONFIG_H)

target_include_directories(
  uxs-legacy PUBLIC uxs-legacy/include ${PROJECT_BINARY_DIR}/generated
                    ${UXS_INCLUDE_DIR})
target_link_libraries(uxs-legacy PUBLIC ${UXS_LIBRARY})

# ##############################################################################
# Add `uxs-tests` build target

file(GLOB_RECURSE test_headers include/*.h)
file(GLOB_RECURSE test_sources src/*.h;src/*.cpp)
if(NOT MSVC AND NOT USE_LIBCPP)
  file(GLOB_RECURSE gcc_testsuite_sources gcc_testsuite/*.h;gcc_testsuite/*.cpp)
endif()

add_executable(uxs-tests .clang-format main.cpp ${test_headers} ${test_sources}
                         ${gcc_testsuite_sources} 3rdparty/fmt/src/format.cc)

target_include_directories(uxs-tests PRIVATE include 3rdparty/fmt/include)
target_link_libraries(uxs-tests PRIVATE uxs-legacy)

# ##############################################################################
# Auxiliary

include("uxs/cmake/uxs/macros.cmake")
add_compile_defs_and_include_dirs_targets(uxs-tests ${CMAKE_CURRENT_SOURCE_DIR})
