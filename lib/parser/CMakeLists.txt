
bison_target(PARSER parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser_gen.cpp)

add_library(parser STATIC
  ${BISON_PARSER_OUTPUTS}
  parser.cpp
  node.cpp
)

target_link_libraries(parser
  backend
  utils
)

