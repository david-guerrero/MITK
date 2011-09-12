SET(SRC_CPP_FILES
  QmitkSegmentationPreferencePage.cpp
)

SET(INTERNAL_CPP_FILES
  mitkPluginActivator.cpp
  QmitkSegmentationView.cpp
  QmitkSegmentationPostProcessing.cpp
  QmitkThresholdAction.cpp
  QmitkCreatePolygonModelAction.cpp
  QmitkStatisticsAction.cpp
  QmitkAutocropAction.cpp  
)

SET(UI_FILES
  src/internal/QmitkSegmentationControls.ui
)

SET(MOC_H_FILES
  src/QmitkSegmentationPreferencePage.h
  src/internal/mitkPluginActivator.h
  src/internal/QmitkSegmentationView.h
  src/internal/QmitkSegmentationPostProcessing.h
  src/internal/QmitkThresholdAction.h
  src/internal/QmitkCreatePolygonModelAction.h
  src/internal/QmitkStatisticsAction.h
  src/internal/QmitkAutocropAction.h
)

SET(CACHED_RESOURCE_FILES
  resources/segmentation.png
  plugin.xml
)

SET(QRC_FILES
  resources/segmentation.qrc
)


SET(CPP_FILES)

foreach(file ${SRC_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
