MITK_CREATE_MODULE(mitkToFProcessing
    SUBPROJECTS MITK-ToF
    DEPENDS Mitk MitkExt mitkCameraCalibration
    PACKAGE_DEPENDS OpenCV
    PROVIDES mitkToFProcessing
  )
  
IF(BUILD_TESTING)

  ADD_SUBDIRECTORY(Testing)

ENDIF(BUILD_TESTING)

