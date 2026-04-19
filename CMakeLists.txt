CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(algorithm VERSION 0.1.0)

# Export compile_commands.json for editor IntelliSense / diagnostics (VS Code C/C++, clangd, etc.)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# Default to Debug build type for single-config generators (e.g. Unix Makefiles/Ninja).
# This makes `Run/Debug` and backtraces more reliable without needing
# `-DCMAKE_BUILD_TYPE=Debug`.
#
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
# 注意：必须用 CMAKE_CXX_STANDARD（或正确的 CMAKE_CXX_FLAGS）。原先误写 CMAKE_CXX_FLAG，标准从未生效。
IF(COMPILER_SUPPORTS_CXX20)
  SET(CMAKE_CXX_STANDARD 20)
  MESSAGE(STATUS "Algorithm: using C++20 (compiler accepts -std=c++20)")
ELSEIF(COMPILER_SUPPORTS_CXX17)
  SET(CMAKE_CXX_STANDARD 17)
  MESSAGE(STATUS "Algorithm: using C++17 only (-std=c++20 not accepted by ${CMAKE_CXX_COMPILER})")
ELSE()
  MESSAGE(STATUS "Algorithm: ${CMAKE_CXX_COMPILER} does not accept -std=c++17; please use a newer g++/clang++.")
ENDIF()
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
MESSAGE(STATUS "Algorithm: CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} (required)")
INCLUDE(cmake/utils.cmake)
ADD_SUBDIRECTORY(deps)
ADD_SUBDIRECTORY(src)