/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2010-05-27 16:06:53 +0200 (Do, 27 Mai 2010) $
Version:   $Revision: $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mitkToFCameraPMDController.h"
#include <pmdsdk2.h>
#include <string.h>

PMDHandle m_PMDHandle; //TODO
PMDDataDescription m_DataDescription; //TODO

struct SourceDataStruct {
   PMDDataDescription dataDescription;
   char sourceData;
}; 

namespace mitk
{
  ToFCameraPMDController::ToFCameraPMDController(): m_PMDRes(0), m_PixelNumber(40000), m_NumberOfBytes(0), 
    m_CaptureWidth(200), m_CaptureHeight(200), m_SourceDataSize(0), m_SourceDataStructSize(0), m_ConnectionCheck(false), 
    m_InputFileName("")
  {
  }

  ToFCameraPMDController::~ToFCameraPMDController()
  {
  }

  bool ToFCameraPMDController::CloseCameraConnection()
  {
    m_PMDRes = pmdClose(m_PMDHandle);
    m_ConnectionCheck = ErrorText(m_PMDRes);
    m_PMDHandle = 0;
    return m_ConnectionCheck;
  } 

  bool ToFCameraPMDController::ErrorText(int error)
  {
    if(error != PMD_OK)
    {
      pmdGetLastError (m_PMDHandle, m_PMDError, 128);
      MITK_ERROR << "Camera Error " << m_PMDError;
      return false;
    }
    else return true;
  }

  bool ToFCameraPMDController::UpdateCamera()
  {
    m_PMDRes = pmdUpdate(m_PMDHandle);                
    return ErrorText(m_PMDRes);
  }

  bool ToFCameraPMDController::GetAmplitudes(float* amplitudeArray)
  {
    this->m_PMDRes = pmdGetAmplitudes(m_PMDHandle, amplitudeArray, this->m_NumberOfBytes);
    return ErrorText(this->m_PMDRes);
  }

  bool ToFCameraPMDController::GetAmplitudes(char* sourceData, float* amplitudeArray)
  {
    this->m_PMDRes = pmdCalcAmplitudes(m_PMDHandle, amplitudeArray, this->m_NumberOfBytes, m_DataDescription, &((SourceDataStruct*)sourceData)->sourceData);
    return ErrorText(this->m_PMDRes);
  }

  bool ToFCameraPMDController::GetIntensities(float* intensityArray)
  {
    this->m_PMDRes = pmdGetIntensities(m_PMDHandle, intensityArray, this->m_NumberOfBytes);
    return ErrorText(this->m_PMDRes);
  }

  bool ToFCameraPMDController::GetIntensities(char* sourceData, float* intensityArray)
  {
    this->m_PMDRes = pmdCalcIntensities(m_PMDHandle, intensityArray, this->m_NumberOfBytes, m_DataDescription, &((SourceDataStruct*)sourceData)->sourceData);
    return ErrorText(this->m_PMDRes);
  }

  bool ToFCameraPMDController::GetDistances(float* distanceArray)
  {
    this->m_PMDRes = pmdGetDistances(m_PMDHandle, distanceArray, this->m_NumberOfBytes);
    return ErrorText(this->m_PMDRes);
  }

  bool ToFCameraPMDController::GetSourceData(char* sourceDataArray)
  {
    this->m_PMDRes = pmdGetSourceDataDescription(m_PMDHandle, &m_DataDescription);
    if (!ErrorText(this->m_PMDRes))
    {
      return false;
    }
    memcpy(&((SourceDataStruct*)sourceDataArray)->dataDescription, &m_DataDescription, sizeof(m_DataDescription));
    this->m_PMDRes = pmdGetSourceData(m_PMDHandle, &((SourceDataStruct*)sourceDataArray)->sourceData, this->m_SourceDataSize);
    return ErrorText(this->m_PMDRes);
  }


  bool ToFCameraPMDController::GetDistances(char* sourceData, float* distanceArray)
  {
    this->m_PMDRes = pmdCalcDistances(m_PMDHandle, distanceArray, this->m_NumberOfBytes, m_DataDescription, &((SourceDataStruct*)sourceData)->sourceData);
    return ErrorText(this->m_PMDRes);
  }

  int ToFCameraPMDController::SetIntegrationTime(unsigned int integrationTime)
  {
    unsigned int result;
    this->m_PMDRes = pmdGetValidIntegrationTime(m_PMDHandle, &result, 0, AtLeast, integrationTime);
    ErrorText(this->m_PMDRes);
    if (this->m_PMDRes != 0)
    {
      return 0;
    }
    this->m_PMDRes = pmdSetIntegrationTime(m_PMDHandle, 0, result);
    ErrorText(this->m_PMDRes);
    return result;
  }

  int ToFCameraPMDController::GetIntegrationTime()
  {
    unsigned int integrationTime = 0;
    this->m_PMDRes = pmdGetIntegrationTime(m_PMDHandle, &integrationTime, 0);
    ErrorText(this->m_PMDRes);
    return integrationTime;
  }

  int ToFCameraPMDController::SetModulationFrequency(unsigned int modulationFrequency)
  {
    unsigned int result;
    this->m_PMDRes = pmdGetValidModulationFrequency(m_PMDHandle, &result, 0, AtLeast, (modulationFrequency*1000000));
    ErrorText(this->m_PMDRes);
    if (this->m_PMDRes != 0)
    {
      return 0;
    }
    this->m_PMDRes = pmdSetModulationFrequency(m_PMDHandle, 0, result);
    ErrorText(this->m_PMDRes);
    return (result/1000000);;
  }

  int ToFCameraPMDController::GetModulationFrequency()
  {
    unsigned int modulationFrequency = 0;
    this->m_PMDRes = pmdGetModulationFrequency (m_PMDHandle, &modulationFrequency, 0);
    ErrorText(this->m_PMDRes);
    return (modulationFrequency/1000000);
  }

  void ToFCameraPMDController::SetInputFileName(std::string inputFileName)
  {
    this->m_InputFileName = inputFileName;
  }
}
