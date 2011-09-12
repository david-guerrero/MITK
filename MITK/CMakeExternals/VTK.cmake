#-----------------------------------------------------------------------------
# VTK
#-----------------------------------------------------------------------------

IF(WIN32)
  OPTION(VTK_USE_SYSTEM_FREETYPE OFF)
ELSE(WIN32)
  OPTION(VTK_USE_SYSTEM_FREETYPE ON)
ENDIF(WIN32)

# Sanity checks
IF(DEFINED VTK_DIR AND NOT EXISTS ${VTK_DIR})
  MESSAGE(FATAL_ERROR "VTK_DIR variable is defined but corresponds to non-existing directory")
ENDIF()

SET(proj VTK)
SET(proj_DEPENDENCIES )
SET(VTK_DEPENDS ${proj})

IF(NOT DEFINED VTK_DIR)

  SET(additional_cmake_args )
  IF(MINGW)
    SET(additional_cmake_args
        -DCMAKE_USE_WIN32_THREADS:BOOL=ON
        -DCMAKE_USE_PTHREADS:BOOL=OFF
        -DVTK_USE_VIDEO4WINDOWS:BOOL=OFF # no header files provided by MinGW
        )
  ENDIF()

  ExternalProject_Add(${proj}
    URL http://mitk.org/download/thirdparty/vtk-5.6.1.tar.gz
    BINARY_DIR ${proj}-build
    INSTALL_COMMAND ""
    CMAKE_GENERATOR ${gen}
    CMAKE_ARGS
        ${ep_common_args}
        ${additional_cmake_args}
        -DVTK_WRAP_TCL:BOOL=OFF
        -DVTK_WRAP_PYTHON:BOOL=OFF
        -DVTK_WRAP_JAVA:BOOL=OFF
        -DBUILD_SHARED_LIBS:BOOL=ON 
        -DVTK_USE_PARALLEL:BOOL=ON
        -DVTK_USE_CHARTS:BOOL=OFF
        -DVTK_USE_GEOVIS:BOOL=OFF
        -DVTK_USE_SYSTEM_FREETYPE:BOOL=${VTK_USE_SYSTEM_FREETYPE}
        ${vtk_QT_ARGS}
     DEPENDS ${proj_DEPENDENCIES}
    )
  SET(VTK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${proj}-build)

ELSE()

  mitkMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  
ENDIF()
