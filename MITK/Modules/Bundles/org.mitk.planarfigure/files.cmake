SET(MOC_H_FILES
  src/internal/mitkPlanarFigureActivator.h
)

SET(SRC_CPP_FILES
  
)

SET(INTERNAL_CPP_FILES
  mitkPlanarFigureActivator.cpp
)

SET(CPP_FILES )

foreach(file ${SRC_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})

