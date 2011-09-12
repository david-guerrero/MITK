/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2010-05-27 16:06:53 +0200 (Do, 27 Mai 2010) $
Version:   $Revision:  $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mitkToFImageGrabber_h
#define __mitkToFImageGrabber_h

#include "mitkToFHardwareExports.h"
#include "mitkCommon.h"
#include "mitkImageSource.h"
#include "mitkToFCameraDevice.h"

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace mitk
{
  /**Documentation
  * \brief Image source providing ToF images. Interface for filters provided in ToFProcessing module
  *
  * This class internally holds a ToFCameraDevice to access the images acquired by a ToF camera.
  * A pre-configured instance for a specific ToF camera (e.g. PMD CamCube 3) can be obtained using
  * the class ToFImageGrabberCreator.
  * Provided images include: distance image (output 0), amplitude image (output 1), intensity image (output 2)
  *
  * \ingroup ToFHardware
  */
  class MITK_TOFHARDWARE_EXPORT ToFImageGrabber : public mitk::ImageSource
  {
  public: 

    mitkClassMacro( ToFImageGrabber , ImageSource );

    itkNewMacro( Self );

    /*!
    \brief Establish a connection to the ToF camera
    \param device specifies the actually used ToF Camera. 0: PMD O3D, 1: PMD CamCube 2.0
    */
    virtual bool ConnectCamera();
    /*!
    \brief Disconnects the ToF camera
    */
    virtual bool DisconnectCamera();
    /*!
    \brief Starts the continuous updating of the camera. 
    A separate thread updates the source data, the main thread processes the source data and creates images and coordinates
    */
    virtual void StartCamera();
    /*!
    \brief Stops the continuous updating of the camera
    */
    virtual void StopCamera();
    /*!
    \brief Returns true if the camera is connected and started
    */
    virtual bool IsCameraActive();
    /*!
    \brief Sets the ToF device, the image grabber is grabbing the images from
    \param aToFCameraDevice device internally used for grabbing the images from the camera
    */
    void SetCameraDevice(ToFCameraDevice* aToFCameraDevice);
    /*!
    \brief Get the currently set ToF camera device
    \return device currently used for grabbing images from the camera
    */
    ToFCameraDevice* GetCameraDevice();
    /*!
    \brief Set the modulation frequency used by the ToF camera.
    For default values see the corresponding device classes
    \param modulationFrequency modulation frequency in Hz
    */
    int SetModulationFrequency(int modulationFrequency);
    /*!
    \brief Get the modulation frequency used by the ToF camera.
    \return modulation frequency in MHz
    */
    int GetModulationFrequency();
    /*!
    \brief Set the integration time used by the ToF camera.
    For default values see the corresponding device classes
    \param integrationTime integration time in ms
    */
    int SetIntegrationTime(int integrationTime);
    /*!
    \brief Get the integration time in used by the ToF camera.
    \return integration time in ms
    */
    int GetIntegrationTime();
    /*!
    \brief Get the dimension in x direction of the ToF image
    \return width of the image
    */
    int GetCaptureWidth();
    /*!
    \brief Get the dimension in y direction of the ToF image
    \return height of the image
    */
    int GetCaptureHeight();
    /*!
    \brief Get the number of pixel in the ToF image
    \return number of pixel
    */
    int GetPixelNumber();
// properties
    void SetBoolProperty( const char* propertyKey, bool boolValue );

    void SetIntProperty( const char* propertyKey, int intValue );

    void SetFloatProperty( const char* propertyKey, float floatValue );

    void SetStringProperty( const char* propertyKey, const char* stringValue );

    void SetProperty( const char *propertyKey, BaseProperty* propertyValue );

  protected:

    ///
    /// called when the ToFCameraDevice was modified
    ///
    void OnToFCameraDeviceModified();

    /*!
    \brief clean up memory allocated for the image arrays m_IntensityArray, m_DistanceArray, m_AmplitudeArray and m_SourceDataArray
    */
    virtual void CleanUpImageArrays();
    /*!
    \brief Allocate memory for the image arrays m_IntensityArray, m_DistanceArray, m_AmplitudeArray and m_SourceDataArray
    */
    virtual void AllocateImageArrays();

    ToFCameraDevice::Pointer m_ToFCameraDevice; ///< Device allowing acces to ToF image data
    int m_CaptureWidth; ///< Width of the captured ToF image
    int m_CaptureHeight; ///< Height of the captured ToF image
    int m_PixelNumber; ///< Number of pixels in the image
    int m_ImageSequence; ///< counter for currently acquired images
    int m_SourceDataSize; ///< size of the source data in bytes
    float* m_IntensityArray; ///< member holding the current intensity array
    float* m_DistanceArray; ///< member holding the current distance array
    float* m_AmplitudeArray; ///< member holding the current amplitude array
    char* m_SourceDataArray;///< member holding the current source data array
    unsigned long m_DeviceObserverTag; ///< tag of the oberver for the the ToFCameraDevice

    ToFImageGrabber();

    ~ToFImageGrabber();

    /*!
    \brief Method generating the outputs of this filter. Called in the updated process of the pipeline.
    0: distance image
    1: amplitude image
    2: intensity image
    */
    void GenerateData();

  private:

  };
} //END mitk namespace
#endif
