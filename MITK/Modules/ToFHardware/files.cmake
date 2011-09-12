SET(CPP_FILES
  mitkToFImageGrabberCreator.cpp
  mitkToFImageGrabber.cpp
  mitkToFOpenCVImageGrabber.cpp
  mitkToFCameraDevice.cpp
  mitkToFCameraMITKPlayerController.cpp
  mitkToFCameraMITKPlayerDevice.cpp
  mitkToFCameraPMDDevice.cpp
  mitkToFCameraPMDPlayerDevice.cpp
  mitkToFCameraPMDMITKPlayerDevice.cpp
  mitkToFCameraPMDO3Device.cpp
  mitkToFCameraPMDCamCubeDevice.cpp
  mitkToFCameraPMDCamBoardDevice.cpp
  mitkToFCameraMESADevice.cpp
  mitkToFCameraMESASR4000Device.cpp
  mitkToFImageRecorder.cpp
  mitkToFImageRecorderFilter.cpp
  mitkToFImageWriter.cpp
  mitkToFImageCsvWriter.cpp
)

IF(MITK_USE_TOF_PMDCAMCUBE)
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraPMDMITKPlayerController.cpp
  mitkToFCameraPMDCamCubeController.cpp
  mitkToFCameraPMDController.cpp
)
IF(WIN32)
  IF(CMAKE_CL_64)
    SET(CPP_FILES ${CPP_FILES} mitkToFCameraPMDPlayerControllerStub.cpp)
  ELSE(CMAKE_CL_64)
    SET(CPP_FILES ${CPP_FILES} mitkToFCameraPMDPlayerController.cpp)
  ENDIF(CMAKE_CL_64)
ELSE(WIN32)
    SET(CPP_FILES ${CPP_FILES} mitkToFCameraPMDPlayerControllerStub.cpp)
ENDIF(WIN32)
ELSE()
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraPMDPlayerControllerStub.cpp
  mitkToFCameraPMDMITKPlayerControllerStub.cpp
  mitkToFCameraPMDCamCubeControllerStub.cpp
  mitkToFCameraPMDControllerStub.cpp
)
ENDIF(MITK_USE_TOF_PMDCAMCUBE)

IF(MITK_USE_TOF_PMDO3)
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraPMDO3Controller.cpp
  mitkToFCameraPMDController.cpp
)
ELSE()
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraPMDO3ControllerStub.cpp
)
ENDIF(MITK_USE_TOF_PMDO3)

IF(MITK_USE_TOF_PMDCAMBOARD)
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraPMDCamBoardController.cpp
  mitkToFCameraPMDController.cpp
)
ELSE()
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraPMDCamBoardControllerStub.cpp
)
ENDIF(MITK_USE_TOF_PMDCAMBOARD)

IF(MITK_USE_TOF_MESASR4000)
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraMESASR4000Controller.cpp
  mitkToFCameraMESAController.cpp
)
ELSE()
SET(CPP_FILES ${CPP_FILES}
  mitkToFCameraMESAControllerStub.cpp
  mitkToFCameraMESASR4000ControllerStub.cpp
)
ENDIF(MITK_USE_TOF_MESASR4000)


