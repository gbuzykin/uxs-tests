cmake_minimum_required(VERSION 3.12)
project(uxs-tests)

include(GenerateExportHeader)

option(USE_ZLIB "Use ZLib" FALSE)
option(USE_LIBZIP "Use LibZip" FALSE)
option(USE_LIBCXX "Use libc++" FALSE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG_REDUCED_BUFFERS")
if(MSVC)
  if(${MSVC_VERSION} GREATER "1800")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
  endif()
else()
  if(USE_LIBCXX)
    add_compile_definitions(__cpp_lib_format=202106L)
    add_compile_definitions(__cpp_lib_to_chars=201601L)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_ITERATOR_DEBUG_LEVEL=2 -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-unused-local-typedefs -Wno-unused-variable")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fstandalone-debug")
  endif()
endif()

if(USE_ZLIB)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUXS_USE_ZLIB")
endif()
if(USE_LIBZIP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUXS_USE_LIBZIP")
endif()

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables
# `sources` and `data`
if(WIN32)
  set(uxs_platform_dir    uxs/platform/win)
elseif(UNIX)
  set(uxs_platform_dir    uxs/platform/posix)
endif()
file(GLOB_RECURSE includes   include/*.h)
file(GLOB_RECURSE sources    src/*.h;src/*.cpp)
file(GLOB_RECURSE uxs_includes    uxs/include/*.h)
file(GLOB_RECURSE uxs_sources     uxs/src/*.h;uxs/src/*.cpp)
file(GLOB_RECURSE uxs_platform_sources    ${uxs_platform_dir}/src/*.h;${uxs_platform_dir}/src/*.cpp)
file(GLOB_RECURSE gcc_testsuite_sources   gcc_testsuite/*.h;gcc_testsuite/*.cpp)
# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

if(MSVC)
  set(gcc_testsuite_sources "")
  if(${MSVC_VERSION} GREATER_EQUAL "1920")
    set(sources "${sources};3rd_party/src/fmt/format.cc")
  endif()
else()
  if(USE_LIBCXX)
    set(gcc_testsuite_sources "")
  endif()
  set(sources "${sources};3rd_party/src/fmt/format.cc")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
include_directories(uxs/include include ${PROJECT_BINARY_DIR}/generated)

###############################################################################
## target definitions #########################################################
###############################################################################

# add the data to the target, so it becomes visible in some IDE
add_library(uxs SHARED
  .clang-format
  ${uxs_includes}
  ${uxs_sources}
  ${uxs_platform_sources}
)

# link with these libraries
if(MSVC)
  set(3RD_PARTY_DIR_PATH "${PROJECT_SOURCE_DIR}/../3rd_party" CACHE PATH "3rdParty directory path")
  if(USE_ZLIB)
    set(ZLIB_INCLUDE_DIR_PATH "${3RD_PARTY_DIR_PATH}/zlib/include" CACHE PATH "ZLib include directory path")
    set(ZLIB_LIBRARY_PATH "${3RD_PARTY_DIR_PATH}/zlib/lib/zlibstatic.lib" CACHE FILEPATH "ZLib library path")
    target_include_directories(uxs PUBLIC ${ZLIB_INCLUDE_DIR_PATH})
    target_link_libraries(uxs ${ZLIB_LIBRARY_PATH})
  endif()
  if(USE_LIBZIP)
    set(LIBZIP_INCLUDE_DIR_PATH "${3RD_PARTY_DIR_PATH}/libzip/include" CACHE PATH "LibZip include directory path")
    set(LIBZIP_LIBRARY_PATH "${3RD_PARTY_DIR_PATH}/libzip/lib/zip.lib" CACHE FILEPATH "LibZip library path")
    target_include_directories(uxs PUBLIC ${LIBZIP_INCLUDE_DIR_PATH})
    target_link_libraries(uxs ${LIBZIP_LIBRARY_PATH})
  endif()
else()
  if(USE_ZLIB)
    target_link_libraries(uxs z)
  endif()
  if(USE_LIBZIP)
    target_link_libraries(uxs zip)
  endif()
endif()

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(uxs PUBLIC 3rd_party/include)

generate_export_header(uxs BASE_NAME uxs EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/generated/uxs_export.h)

add_executable(uxs-tests
  .clang-format
  main.cpp
  ${includes}
  ${sources}
  ${gcc_testsuite_sources}
)

target_link_libraries(uxs-tests uxs)
if(USE_LIBCXX)
  target_link_libraries(uxs-tests c++abi)
endif()
