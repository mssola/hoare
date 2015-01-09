project(hoare)
cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Define all the real checks. They all can be found in the `cmake` directory.
include(HFindLibs)
include(HCompiler)

set(HOARE_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}/lib
)

include_directories(${HOARE_INCLUDE_DIRS})
add_subdirectory(lib)
add_executable(hoare main.cpp)
target_link_libraries(hoare
  parser
  codegen

  # Defined by the HFindLibs.cmake module.
  ${llvm_libs}
)

install(TARGETS hoare DESTINATION bin)

