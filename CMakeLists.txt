cmake_minimum_required(VERSION 3.12)
project(uxs-tests)

option(USE_ZLIB "Use ZLib" FALSE)
option(USE_LIBZIP "Use LibZip" FALSE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG_REDUCED_BUFFERS")
if(MSVC)
  if(${MSVC_VERSION} GREATER_EQUAL "1920")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STRICT_ANSI__ -Wall -pedantic")
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
  set(sources "${sources};3rd_party/src/fmt/format.cc")
endif()

###############################################################################
## target definitions #########################################################
###############################################################################

# add the data to the target, so it becomes visible in some IDE
add_executable(uxs-tests
  .clang-format
  main.cpp
  ${uxs_includes}
  ${uxs_sources}
  ${uxs_platform_sources}
  ${includes}
  ${sources}
  ${gcc_testsuite_sources}
)

# link with these libraries
if(MSVC)
  set(3RD_PARTY_DIR_PATH "${PROJECT_SOURCE_DIR}/../3rd_party" CACHE PATH "3rdParty directory path")
  if(USE_ZLIB)
    set(ZLIB_INCLUDE_DIR_PATH "${3RD_PARTY_DIR_PATH}/zlib/include" CACHE PATH "ZLib include directory path")
    set(ZLIB_LIBRARY_PATH "${3RD_PARTY_DIR_PATH}/zlib/lib/zlibstatic.lib" CACHE FILEPATH "ZLib library path")
    target_include_directories(uxs-tests PUBLIC ${ZLIB_INCLUDE_DIR_PATH})
    target_link_libraries(uxs-tests ${ZLIB_LIBRARY_PATH})
  endif()
  if(USE_LIBZIP)
    set(LIBZIP_INCLUDE_DIR_PATH "${3RD_PARTY_DIR_PATH}/libzip/include" CACHE PATH "LibZip include directory path")
    set(LIBZIP_LIBRARY_PATH "${3RD_PARTY_DIR_PATH}/libzip/lib/zip.lib" CACHE FILEPATH "LibZip library path")
    target_include_directories(uxs-tests PUBLIC ${LIBZIP_INCLUDE_DIR_PATH})
    target_link_libraries(uxs-tests ${LIBZIP_LIBRARY_PATH})
  endif()
else()
  target_link_libraries(uxs-tests stdc++ m pthread)
  if(USE_ZLIB)
    target_link_libraries(uxs-tests z)
  endif()
  if(USE_LIBZIP)
    target_link_libraries(uxs-tests zip)
  endif()
endif()

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(uxs-tests PUBLIC uxs/include 3rd_party/include include)
