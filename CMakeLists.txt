CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(algorithm VERSION 0.1.0)

INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
IF(COMPILER_SUPPORTS_CXX17)
  SET(CMAKE_CXX_FLAG "${CMAKE_CXX_FLAGS} -std=c++17")
ELSE()
  MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a C++ compiler with at least a version of 8.4.")
ENDIF()
INCLUDE(cmake/utils.cmake)
ADD_SUBDIRECTORY(deps)
ADD_SUBDIRECTORY(src)