# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

add_subdirectory(../parser/Parser ./Parser)
add_subdirectory(../spider-revisited/Spider-Revisited ./Spider-Revisited)
add_subdirectory(../crawler/Crawler ./Crawler)

if (WIN32)
  set(Boost_USE_STATIC_LIBS ON) 
  set(Boost_USE_MULTITHREADED ON)  
  set(Boost_USE_STATIC_RUNTIME OFF) 

  set(BOOST_ROOT "C:/Program Files/Boost/boost_1_76_0") 
endif()


find_package(Boost REQUIRED COMPONENTS regex date_time)

set(CONTROLLER_HEADER_LIST
  commands.h
  database_requests.h
  flag_parser.h
  flags.h
  input.h
  network_utils.h
  networking.h
  parser2.h
  print.h
  project_metadata.h
  regex_wrapper.h
  utils.h
  loguru/loguru.hpp
)

add_library(controller
  commands.cpp  
  database_requests.cpp
  error.cpp
  flags.cpp
  input.cpp
  network_utils.cpp
  networking.cpp
  parser2.cpp
  print.cpp
  regex_wrapper.cpp
  utils.cpp
  loguru/loguru.cpp
  ${CONTROLLER_HEADER_LIST}
)

target_link_libraries(controller spider parser crawler ${Boost_LIBRARIES})
target_include_directories(controller PUBLIC "${PROJECT_BINARY_DIR}" ../parser/Parser ../spider-revisited/Spider-Revisited ../crawler/Crawler ${Boost_INCLUDE_DIRS})

# copy version files
if (WIN32)
  execute_process(COMMAND CMD /c cd ../bin && copy_versions.bat && cd ../build)
endif (WIN32)
if (UNIX)
  execute_process(COMMAND bash "../bin/copy_versions.sh")
endif (UNIX)
