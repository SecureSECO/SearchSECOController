cmake_minimum_required(VERSION 3.10)

# get the project version
#execute_process(COMMAND CMD /c cd ${CMAKE_CURRENT_SOURCE_DIR} && type VERSION && cd ${CMAKE_BINARY_DIR} OUTPUT_VARIABLE version)

# set the project name
project(SearchSECO)

add_executable(
    SearchSECO
    "src/Controller.cpp"
    "src/Commands.cpp"
    "src/Commands.h"
    "src/Parser.cpp"
    "src/Parser.h"
    "src/Print.cpp"
    "src/Print.h"
    "src/Utils.cpp"
    "src/Utils.h"
)

