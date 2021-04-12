# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

add_subdirectory(../parser/Parser ./Parser)
add_subdirectory(../spider/SearchSECOSpider ./SearchSECOSpider)

set(CONTROLLER_HEADER_LIST
  Commands.h
  Parser2.h
  Print.h
  Utils.h
)

add_library(controller
  Commands.cpp
  Parser2.cpp
  Print.cpp
  Utils.cpp
  ${CONTROLLER_HEADER_LIST}
)

target_link_libraries(controller spider parser)
target_include_directories(spider PUBLIC "${PROJECT_BINARY_DIR}" ../parser/Parser ../spider/SearchSECOSpider)