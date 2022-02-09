cmake_minimum_required(VERSION 3.12)
project(std-ext-tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STRICT_ANSI__")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_ITERATOR_DEBUG_LEVEL=2 -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`
file(GLOB_RECURSE includes      std-ext/include/*.h)
file(GLOB_RECURSE sources       std-ext/src/*.h;std-ext/src/*.cpp;src/*.h;src/*.cpp)
file(GLOB_RECURSE gcc_testsuite_sources     gcc_testsuite/*.h;gcc_testsuite/*.cpp)
# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

###############################################################################
## target definitions #########################################################
###############################################################################

# add the data to the target, so it becomes visible in some IDE
add_executable(std-ext-tests
  .clang-format
  main.cpp
  ${includes}
  ${sources}
  ${gcc_testsuite_sources}
)

# this lets me include files relative to the root src dir with a <> pair
target_include_directories(std-ext-tests PUBLIC std-ext/include include)
