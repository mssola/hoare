
add_library(codegen STATIC
  blocks.cpp
  builtins.cpp
  context.cpp
  hoare.cpp
)

target_link_libraries(codegen
  ast
  parser
  utils
)

