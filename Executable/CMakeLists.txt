cmake_minimum_required(VERSION 3.10)

# get the project version
#execute_process(COMMAND CMD /c cd ${CMAKE_CURRENT_SOURCE_DIR} && type VERSION && cd ${CMAKE_BINARY_DIR} OUTPUT_VARIABLE version)


# set the project name
project(SearchSECO)

add_subdirectory(../src ./src)

set(EXECUTABLE_PROCESS SearchSECO)

add_executable(
    ${EXECUTABLE_PROCESS}
    "Controller.cpp"
)

target_link_libraries(${EXECUTABLE_PROCESS} controller)

if(CMAKE_DL_LIBS)
    target_link_libraries(${TEST_NAME} ${CMAKE_DL_LIBS})
endif()

target_include_directories(${EXECUTABLE_PROCESS} PUBLIC "${PROJECT_BINARY_DIR}" ../src)

