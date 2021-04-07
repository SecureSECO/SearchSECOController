cmake_minimum_required(VERSION 3.10)

# get the project version
#execute_process(COMMAND CMD /c cd ${CMAKE_CURRENT_SOURCE_DIR} && type VERSION && cd ${CMAKE_BINARY_DIR} OUTPUT_VARIABLE version)


# set the project name
project(SearchSECO)

add_subdirectory(parser/Parser)

set(EXECUTABLE_PROCESS SearchSECO)

add_executable(
    ${EXECUTABLE_PROCESS}
    "src/Controller.cpp"
    "src/Commands.cpp"
    "src/Commands.h"
    "src/Parser.cpp"
    "src/Parser.h"   # Rename to new file
    "src/Print.cpp"
    "src/Print.h"
    "src/Utils.cpp"
    "src/Utils.h"
)

target_link_libraries(${EXECUTABLE_PROCESS} parser)
target_include_directories(${EXECUTABLE_PROCESS} PUBLIC "${PROJECT_BINARY_DIR}" parser/Parser)

