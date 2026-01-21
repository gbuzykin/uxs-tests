cmake_minimum_required(VERSION 3.21)
project(
  uxs-tests
  DESCRIPTION "Project for UXS library testing"
  LANGUAGES CXX)

include(FetchContent)
include(GenerateExportHeader)

option(USE_ZLIB "Use ZLib" ON)
option(USE_LIBZIP "Use LibZip" ON)
option(USE_LIBCPP "Use LibC++" OFF)
option(USE_SANITIZERS_FOR_DEBUG "Use Sanitizers for Debug build" ON)
option(OPTION_TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(OPTION_EXPORT_COMPILE_DEFS_AND_INCLUDE_DIRS
       "Export compile definitions and include directories" OFF)

message(
  "Using ${CMAKE_CXX_COMPILER} C++ compiler (BUILD_TYPE=${CMAKE_BUILD_TYPE})")
message("CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message("USE_ZLIB = ${USE_ZLIB}")
message("USE_LIBZIP = ${USE_LIBZIP}")
message("USE_LIBCPP = ${USE_LIBCPP}")

set(SYMBOLS_DIR bin)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Using C++${CMAKE_CXX_STANDARD} standard")

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if(UNIX)
  set(CMAKE_SKIP_RPATH OFF)
  set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
  set(CMAKE_INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib")
endif()

# ##############################################################################
# Compilation flags

if(MSVC)
  add_compile_options(/Zc:__cplusplus /utf-8)
  if(OPTION_TREAT_WARNINGS_AS_ERRORS)
    add_compile_options(/WX)
  endif()
else()
  add_compile_options(-Wall -Wextra -pedantic)
  if(OPTION_TREAT_WARNINGS_AS_ERRORS)
    add_compile_options(-Werror)
  endif()
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
# Add `uxs` build target

block()
set(ZLIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib)
set(LIBZIP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libzip)
FetchContent_Declare(uxs SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/uxs PREFIX
                         ${CMAKE_CURRENT_BINARY_DIR}/uxs)
FetchContent_MakeAvailable(uxs)
endblock()

# ##############################################################################
# Add `uxs-legacy` build target

file(GLOB_RECURSE uxs_legacy_headers uxs-legacy/include/*.h)
file(GLOB_RECURSE uxs_legacy_sources uxs-legacy/src/*.h;uxs-legacy/src/*.cpp)

add_library(uxs-legacy SHARED ${uxs_legacy_headers} ${uxs_legacy_sources})

generate_export_header(
  uxs-legacy EXPORT_FILE_NAME
  ${PROJECT_BINARY_DIR}/generated/uxs-legacy/config.h INCLUDE_GUARD_NAME
  UXS_LEGACY_CONFIG_H)

target_include_directories(uxs-legacy PUBLIC uxs-legacy/include
                                             ${PROJECT_BINARY_DIR}/generated)
target_link_libraries(uxs-legacy PUBLIC uxs)

install(
  TARGETS uxs-legacy
  RUNTIME DESTINATION bin COMPONENT binaries
  LIBRARY DESTINATION lib COMPONENT libraries)
install_win32_pdb(uxs-legacy)

# ##############################################################################
# Add `uxs-tests` build target

file(GLOB_RECURSE test_headers include/*.h)
file(GLOB_RECURSE test_sources src/*.h;src/*.cpp)
if(NOT MSVC AND NOT USE_LIBCPP)
  file(GLOB_RECURSE gcc_testsuite_sources gcc_testsuite/*.h;gcc_testsuite/*.cpp)
endif()

add_executable(uxs-tests main.cpp ${test_headers} ${test_sources}
                         ${gcc_testsuite_sources} 3rdparty/fmt/src/format.cc)

target_include_directories(uxs-tests PRIVATE include 3rdparty/fmt/include)
target_link_libraries(uxs-tests PRIVATE uxs-legacy)

install(TARGETS uxs-tests RUNTIME DESTINATION bin COMPONENT binary)
install_win32_pdb(uxs-tests)

# ##############################################################################
# Auxiliary

add_compile_defs_and_include_dirs_targets(uxs-tests ${CMAKE_CURRENT_SOURCE_DIR})
