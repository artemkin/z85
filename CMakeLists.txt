cmake_minimum_required (VERSION 2.8)
project (Z85)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Disable assert() to run all unit tests
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG")

# Compiler-specific C++11 activation
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
execute_process(
  COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
      message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()

  # Add gcov support
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ()

add_subdirectory (src)
add_subdirectory (test)
enable_testing ()
add_test (NAME Z85Test COMMAND Test)

