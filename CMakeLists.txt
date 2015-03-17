
project(hoare)
cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Define all the real checks. They all can be found in the `cmake` directory.
include(HFindLibs)
include(HCompiler)

# Version
set(HOARE_MAJOR 0)
set(HOARE_MINOR 1)
set(HOARE_PATCH 0)
ecm_setup_version(${HOARE_MAJOR}.${HOARE_MINOR}.${HOARE_PATCH}
                  VARIABLE_PREFIX HOARE
                  VERSION_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/version.h" )

# Applying the version string to the man page.
string(TIMESTAMP DATE "%Y-%m-%d")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/hoare.1.in
    ${CMAKE_CURRENT_BINARY_DIR}/hoare.1
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_BINARY_DIR}/lib
)

add_subdirectory(lib)
add_executable(hoare main.cpp)
target_link_libraries(hoare
  parser
  codegen

  # Defined by the HFindLibs.cmake module.
  ${llvm_libs}
)

install(TARGETS hoare DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hoare.1 DESTINATION share/man/man1)
