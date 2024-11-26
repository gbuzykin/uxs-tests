cmake_minimum_required(VERSION 3.12)
project(uxs-tests)

include(GenerateExportHeader)
include(ExternalProject)

option(USE_ZLIB "Use ZLib" On)
option(USE_LIBZIP "Use LibZip" On)
option(USE_LIBCPP "Use LibC++" Off)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(USE_LIBCPP Off)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(
  "Using ${CMAKE_CXX_COMPILER} C++ compiler (BUILD_TYPE=${CMAKE_BUILD_TYPE})")
message("Using C++${CMAKE_CXX_STANDARD}")
message("USE_ZLIB = ${USE_ZLIB}")
message("USE_LIBZIP = ${USE_LIBZIP}")
message("USE_LIBCPP = ${USE_LIBCPP}")

if(USE_ZLIB OR USE_LIBZIP)
  set(LIBZ_STATIC_LIB $<IF:$<BOOL:${MSVC}>,zlibstatic$<$<CONFIG:Debug>:d>.lib,libz.a>)
  ExternalProject_Add(
    libz
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libz
    BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/libz/lib/${LIBZ_STATIC_LIB}
    CMAKE_ARGS -DZLIB_BUILD_EXAMPLES=Off #
               -DSKIP_INSTALL_FILES=On #
               -DCMAKE_POSITION_INDEPENDENT_CODE=On #
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} #
               -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} #
               -DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS} #
               -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> #
               ${BUILD_TYPE_CMAKE_ARG})
  ExternalProject_Get_Property(libz INSTALL_DIR)
  set(LIBZ_INSTALL_DIR ${INSTALL_DIR})
endif()

if(USE_LIBZIP)
  set(LIBZIP_STATIC_LIB $<IF:$<BOOL:${MSVC}>,zip.lib,libzip.a>)
  ExternalProject_Add(
    libzip
    DEPENDS libz
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libzip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libzip
    BUILD_BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/libzip/lib/${LIBZIP_STATIC_LIB}
    CMAKE_ARGS -DENABLE_COMMONCRYPTO=Off #
               -DENABLE_GNUTLS=Off #
               -DENABLE_MBEDTLS=Off #
               -DENABLE_OPENSSL=Off #
               -DENABLE_WINDOWS_CRYPTO=Off #
               -DENABLE_BZIP2=Off #
               -DENABLE_LZMA=Off #
               -DENABLE_ZSTD=Off #
               -DBUILD_TOOLS=Off #
               -DBUILD_REGRESS=Off #
               -DBUILD_OSSFUZZ=Off #
               -DBUILD_EXAMPLES=Off #
               -DBUILD_DOC=Off #
               -DBUILD_SHARED_LIBS=Off #
               -DCMAKE_POSITION_INDEPENDENT_CODE=On #
               -DZLIB_ROOT=${LIBZ_INSTALL_DIR}
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} #
               -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} #
               -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> #
               ${BUILD_TYPE_CMAKE_ARG})
  ExternalProject_Get_Property(libzip INSTALL_DIR)
  set(LIBZIP_INSTALL_DIR ${INSTALL_DIR})
endif()

add_compile_definitions($<$<CONFIG:Debug>:_DEBUG_REDUCED_BUFFERS>)

if(MSVC)
  add_compile_options(/Zc:__cplusplus)
  add_compile_options($<$<CONFIG:Debug>:/WX>)
else()
  if(USE_LIBCPP)
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
  endif()
  add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=2>)
  add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
  add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)
  add_compile_options(-Wall -pedantic -Werror)
  add_compile_options(-Wno-unused-function)
  add_compile_options(-Wno-unused-local-typedefs)
  add_compile_options(-Wno-unused-variable)
  add_compile_options(-Wno-nullability-extension)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wno-unused-private-field)
    add_compile_options(-Wno-ignored-attributes)
    add_compile_options($<$<CONFIG:Debug>:-fstandalone-debug>)
  endif()
endif()

# ##############################################################################
# file globbing ##############################################################
# ##############################################################################

if(WIN32)
  set(uxs_platform_dir uxs/platform/win)
elseif(UNIX)
  set(uxs_platform_dir uxs/platform/posix)
endif()
file(GLOB_RECURSE includes include/*.h)
file(GLOB_RECURSE sources src/*.h;src/*.cpp)
file(GLOB_RECURSE uxs_includes uxs/include/*.h)
file(GLOB_RECURSE uxs_sources uxs/src/*.h;uxs/src/*.cpp)
file(GLOB_RECURSE uxs_platform_sources
     ${uxs_platform_dir}/src/*.h;${uxs_platform_dir}/src/*.cpp)
file(GLOB_RECURSE gcc_testsuite_sources gcc_testsuite/*.h;gcc_testsuite/*.cpp)

if(MSVC OR USE_LIBCPP)
  set(gcc_testsuite_sources "")
endif()

set(sources "${sources};3rd_party/src/fmt/format.cc")

# ##############################################################################
# target definitions #########################################################
# ##############################################################################

add_library(uxs SHARED .clang-format ${uxs_includes} ${uxs_sources}
                       ${uxs_platform_sources})

generate_export_header(uxs BASE_NAME uxs EXPORT_FILE_NAME
                       ${PROJECT_BINARY_DIR}/generated/uxs_export.h)

include_directories(uxs/include include ${PROJECT_BINARY_DIR}/generated)

if(USE_ZLIB OR USE_LIBZIP)
  add_dependencies(uxs libz)
  target_include_directories(uxs PRIVATE ${LIBZ_INSTALL_DIR}/include)
  target_link_libraries(uxs PRIVATE ${LIBZ_INSTALL_DIR}/lib/${LIBZ_STATIC_LIB})
  add_compile_definitions(UXS_USE_ZLIB)
endif()

if(USE_LIBZIP)
  add_dependencies(uxs libzip)
  target_include_directories(uxs PRIVATE ${LIBZIP_INSTALL_DIR}/include)
  target_link_libraries(uxs
                        PRIVATE ${LIBZIP_INSTALL_DIR}/lib/${LIBZIP_STATIC_LIB})
  add_compile_definitions(UXS_USE_LIBZIP)
endif()

add_executable(uxs-tests .clang-format main.cpp ${includes} ${sources}
                         ${gcc_testsuite_sources})

target_include_directories(uxs-tests PUBLIC 3rd_party/include)
target_link_libraries(uxs-tests uxs)
