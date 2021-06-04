# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

add_subdirectory(../parser/Parser ./Parser)
add_subdirectory(../spider-revisited/Spider-Revisited ./Spider-Revisited)
add_subdirectory(../crawler/Crawler ./Crawler)

set(CMAKE_CXX_STANDARD 17)

message("Building the controller...")
if(NOT DEFINED LOGURU_INCLUDED)
   set(LOGURU_FILES
   loguru/loguru.hpp
   loguru/loguru.cpp
   )
   set(LOGURU_INCLUDED true)
   message("Including loguru files...")
endif()

if (WIN32)
  set(Boost_USE_STATIC_LIBS ON) 
  set(Boost_USE_MULTITHREADED ON)  
  set(Boost_USE_STATIC_RUNTIME OFF) 

  set(BOOST_ROOT "C:/Program Files/Boost/boost_1_76_0") 
endif()


find_package(Boost REQUIRED COMPONENTS regex date_time)

set(CONTROLLER_HEADER_LIST
  loguru/loguru.hpp
  commandFactory.h
  commands.h
  databaseRequests.h
  flagParser.h
  flags.h
  input.h
  moduleFacades.h
  networkUtils.h
  networking.h
  parser2.h
  print.h
  projectMetadata.h
  regexWrapper.h
  termination.h
  utils.h
)

add_library(controller
  loguru/loguru.cpp
  commandFactory.cpp
  commands.cpp  
  databaseRequests.cpp
  error.cpp
  flags.cpp
  input.cpp
  moduleFacades.cpp
  networkUtils.cpp
  networking.cpp
  parser2.cpp
  print.cpp
  regexWrapper.cpp
  termination.cpp
  utils.cpp
  ${CONTROLLER_HEADER_LIST}
  ${LOGURU_FILES}
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
