
add_library(codegen STATIC
  hoare.cpp
  context.cpp
  builtins.cpp
  blocks.cpp
  printer.cpp
)

target_link_libraries(codegen
  ast
  parser
  utils
)

