cmake_minimum_required(VERSION 3.10)

# set the project name
project(searchseco)

configure_file(".env" ".env" COPYONLY)

add_subdirectory(../libSearchSECOController ./libSearchSECOController)

set(EXECUTABLE_PROCESS searchseco)

add_executable(
    ${EXECUTABLE_PROCESS}
    "controller.cpp"
)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

target_link_libraries(${EXECUTABLE_PROCESS} controller)

if(CMAKE_DL_LIBS)
    target_link_libraries(${EXECUTABLE_PROCESS} ${CMAKE_DL_LIBS})
endif()

target_include_directories(${EXECUTABLE_PROCESS} PUBLIC "${PROJECT_BINARY_DIR}" ../libSearchSECOController)
