SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MITK_SOURCE_DIR}/BlueBerry/CMake)
FIND_PACKAGE(Poco)
LIST(APPEND ALL_INCLUDE_DIRECTORIES ${Poco_INCLUDE_DIRS})
LIST(APPEND ALL_LIBRARIES optimized PocoFoundation 
                          debug PocoFoundationd 
                          optimized PocoUtil 
                          debug PocoUtild
                          optimized PocoXML
                          debug PocoXMLd)
link_directories(${Poco_LIBRARY_DIRS})

 
