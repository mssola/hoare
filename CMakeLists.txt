project(hoare)

# Find CMake and dependencies.
cmake_minimum_required(VERSION 2.8.0)
find_package(BISON REQUIRED)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Check the compiler.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Require at least GCC 4.9
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "GCC version must be at least 4.8!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # Require at least Clang 3.5
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
    message(FATAL_ERROR "Clang version must be at least 3.4!")
  endif()
else()
  message(WARNING "Compilation has only been tested with Clang and GCC.")
endif()

# Because of the compiler requirements above, it's safe to add C++11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HOARE_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}/lib
)

include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${HOARE_INCLUDE_DIRS}
)

# LLVM
add_definitions(${LLVM_DEFINITIONS})

# TODO: picked from LLVM's site. I have to check which of these modules I
# actually need.
llvm_map_components_to_libnames(llvm_libs support core irreader native engine)

add_subdirectory(lib)

add_executable(hoare
  main.cpp
)

target_link_libraries(hoare
  parser
  backend
  ${llvm_libs}
)

