PROJECT(TINY_XML)

SET(TINY_XML_HEADERS
  tinystr.h
  tinyxml.h
)
  
SET(TINY_XML_SOURCES
  tinyxml.cpp
  tinyxmlparser.cpp
  tinyxmlerror.cpp
  tinystr.cpp
)

ADD_DEFINITIONS(-DTIXML_USE_STL)
ADD_LIBRARY(tinyxml${LIBPOSTFIX} ${MITK_WIN32_FORCE_STATIC} ${TINY_XML_HEADERS} ${TINY_XML_SOURCES})
MITK_CREATE_MODULE_CONF( tinyxml )

