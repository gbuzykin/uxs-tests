cmake_minimum_required(VERSION 3.21)

project(
  uxs-tests
  DESCRIPTION "Project for UXS library testing"
  LANGUAGES CXX)

include(FetchContent)
include(GenerateExportHeader)
include(GNUInstallDirs)

option(UXS_USE_ZLIB "Use ZLib" ON)
option(UXS_USE_LIBZIP "Use LibZip" ON)
option(OPTION_USE_LIBCPP "Use LibC++" OFF)
option(OPTION_DEBUG_USE_SANITIZERS "Use Sanitizers for Debug" ON)
option(OPTION_TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(OPTION_EXPORT_COMPILE_DEFS_AND_INCLUDE_DIRS
       "Export compile definitions and include directories" OFF)

set(UXS_DEBUG_REDUCED_BUFFERS
    1
    CACHE STRING "Use reduced buffers for Debug")

message(
  "Using ${CMAKE_CXX_COMPILER} C++ compiler (BUILD_TYPE=${CMAKE_BUILD_TYPE})")
message("CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message("USE_ZLIB = ${UXS_USE_ZLIB}")
message("USE_LIBZIP = ${UXS_USE_LIBZIP}")
message("USE_LIBCPP = ${OPTION_USE_LIBCPP}")

set(INSTALL_WIN32_SYMBOLS_DIR ${CMAKE_INSTALL_BINDIR})

set(CMAKE_CXX_STANDARD
    20
    CACHE STRING "Default C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("Using C++${CMAKE_CXX_STANDARD} standard")

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if(UNIX)
  set(CMAKE_SKIP_RPATH OFF)
  set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
  set(CMAKE_INSTALL_RPATH "$ORIGIN:$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
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
    add_compile_options(-Wno-nullability-extension)
    if(CMAKE_CXX_STANDARD LESS 17)
      add_compile_options(-Wno-ignored-attributes)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
      add_compile_options($<$<CONFIG:Release>:-Wno-use-after-free>)
    endif()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
      add_compile_options($<$<CONFIG:Release>:-Wno-stringop-overflow>)
    endif()
    add_compile_options($<$<CONFIG:Release>:-Wno-array-bounds>)
    add_compile_options($<$<CONFIG:Release>:-Wno-nonnull>)
  endif()
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options($<$<CONFIG:Debug>:-fstandalone-debug>)
    if(OPTION_USE_LIBCPP)
      add_compile_options(-stdlib=libc++)
      add_link_options(-stdlib=libc++)
    endif()
  endif()
endif()

if(OPTION_DEBUG_USE_SANITIZERS)
  message("Using sanitizers for Debug")
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
set(UXS_DO_INSTALL_SDK OFF)
FetchContent_Declare(uxs SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/uxs)
FetchContent_MakeAvailable(uxs)
endblock()

# ##############################################################################
# Add `uxs-legacy` build target

file(GLOB_RECURSE UXS_LEGACY_HEADERS uxs-legacy/include/*.h)
file(GLOB_RECURSE UXS_LEGACY_SOURCES uxs-legacy/src/*.h;uxs-legacy/src/*.cpp)

add_library(uxs-legacy SHARED ${UXS_LEGACY_HEADERS} ${UXS_LEGACY_SOURCES})

generate_export_header(
  uxs-legacy EXPORT_FILE_NAME
  ${PROJECT_BINARY_DIR}/generated/uxs-legacy/config.h INCLUDE_GUARD_NAME
  UXS_LEGACY_CONFIG_H)

target_include_directories(uxs-legacy PUBLIC uxs-legacy/include
                                             ${PROJECT_BINARY_DIR}/generated)
target_link_libraries(uxs-legacy PRIVATE UXS::UXS)

install(
  TARGETS uxs-legacy
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT binaries
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries)
install_win32_pdb(uxs-legacy)

# ##############################################################################
# Add `uxs-tests` build target

file(GLOB_RECURSE TEST_HEADERS include/*.h)
file(GLOB_RECURSE TEST_SOURCES src/*.h;src/*.cpp)
if(NOT MSVC AND NOT OPTION_USE_LIBCPP)
  file(GLOB_RECURSE GCC_TESTSUITE_SOURCES gcc_testsuite/*.h;gcc_testsuite/*.cpp)
endif()

add_executable(uxs-tests main.cpp ${TEST_HEADERS} ${TEST_SOURCES}
                         ${GCC_TESTSUITE_SOURCES} 3rdparty/fmt/src/format.cc)

target_include_directories(uxs-tests PRIVATE include 3rdparty/fmt/include)
target_link_libraries(uxs-tests PRIVATE UXS::UXS uxs-legacy)

install(TARGETS uxs-tests RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                                  COMPONENT binaries)
install_win32_pdb(uxs-tests)

# ##############################################################################
# Auxiliary

add_compile_defs_and_include_dirs_targets(uxs-tests ${CMAKE_CURRENT_SOURCE_DIR})
