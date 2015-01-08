
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

# Handle the -g flag and the optimization level depending on the build type.
set(CMAKE_CXX_FLAGS_DEBUG          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# And now we set our generic C++ flags.
# TODO: -Werror
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wpedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wswitch-default -Wundef")

