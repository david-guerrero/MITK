SET(SRC_CPP_FILES
  
)

SET(INTERNAL_CPP_FILES
  mitkImageCropperPluginActivator.cpp
  QmitkImageCropper.cpp
  mitkImageCropperEventInterface.cpp
)

SET(UI_FILES
  src/internal/QmitkImageCropperControls.ui
)

SET(MOC_H_FILES
  src/internal/mitkImageCropperPluginActivator.h
  src/internal/QmitkImageCropper.h
)

SET(CACHED_RESOURCE_FILES
  resources/icon.xpm
  plugin.xml
)

SET(QRC_FILES
  resources/imagecropper.qrc
)

SET(CPP_FILES)

foreach(file ${SRC_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
