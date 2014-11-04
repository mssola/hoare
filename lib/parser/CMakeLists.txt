
bison_target(PARSER parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

add_library(parser STATIC
  ${BISON_PARSER_OUTPUTS}
  node.cpp
  driver.cpp
  scanner.cpp
)

target_link_libraries(parser
  backend
  utils
)

