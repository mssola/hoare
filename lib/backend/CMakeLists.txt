
add_library(backend STATIC
  hoare.cpp
  context.cpp
  builtins.cpp
  blocks.cpp
  printer.cpp
)

target_link_libraries(backend
  parser
)

