# This program has been developed by students from the bachelor Computer Science at
# Utrecht University within the Software Project course.
# © Copyright Utrecht University (Department of Information and Computing Sciences)

add_subdirectory(../parser/Parser ./Parser)
add_subdirectory(../spider-revisited/Spider-Revisited ./spider-revisited)
add_subdirectory(../crawler/Crawler ./crawler)

if (WIN32)
  set(Boost_USE_STATIC_LIBS ON) 
  set(Boost_USE_MULTITHREADED ON)  
  set(Boost_USE_STATIC_RUNTIME OFF) 

  set(BOOST_ROOT "C:/Program Files/Boost/boost_1_76_0") 
endif()


find_package(Boost REQUIRED COMPONENTS regex date_time)

set(CONTROLLER_HEADER_LIST
  Commands.h
  DatabaseRequests.h
  Flag.h
  FlagParser.h
  Flags.h
  Input.h
  Networking.h
  NetworkUtils.h
  Parser2.h
  Print.h
  Utils.h
  loguru/loguru.hpp
)

add_library(controller
  Commands.cpp  
  DatabaseRequests.cpp
  Error.cpp
  Flags.cpp
  Input.cpp
  Networking.cpp
  NetworkUtils.cpp
  Parser2.cpp
  Print.cpp
  Utils.cpp
  loguru/loguru.cpp
  ${CONTROLLER_HEADER_LIST}
)

target_link_libraries(controller spider parser ${Boost_LIBRARIES})
target_include_directories(spider PUBLIC "${PROJECT_BINARY_DIR}" ../parser/Parser ../spider/SearchSECOSpider ${Boost_INCLUDE_DIRS})

# copy version files
if (WIN32)
  execute_process(COMMAND CMD /c cd ../bin && copy_versions.bat && cd ../build)
endif (WIN32)
if (UNIX)
  execute_process(COMMAND bash "../bin/copy_versions.sh")
endif (UNIX)
