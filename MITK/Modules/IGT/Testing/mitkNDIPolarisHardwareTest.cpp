/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 7837 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkNDITrackingDevice.h"
#include "mitkNDIPassiveTool.h"
#include "mitkTestingMacros.h"
#include "mitkTrackingTypes.h"
#include "mitkTrackingTool.h"
#include "mitkStandardFileLocations.h"


int mitkNDIPolarisHardwareTest(int argc , char* argv[])
{
  // always start with this!
  MITK_TEST_BEGIN("NDIPolarisHardware (An NDI tracking device has to be connected to this system!)");

  if (argc<2) {MITK_TEST_FAILED_MSG(<<"Error: test must be called with the com port as second parameter!");}

  int comPort = *argv[1] - '0';

  //create a polaris tracking device
  mitk::NDITrackingDevice::Pointer myNDITrackingDevice = mitk::NDITrackingDevice::New();
  myNDITrackingDevice->SetType(mitk::NDIPolaris);

  //set port
  switch (comPort)
    {
    case 1:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM1);
      break;
    case 2:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM2);
      break;
    case 3:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM3);
      break;
    case 4:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM4);
      break;
    case 5:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM5);
      break;
    case 6:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM6);
      break;
    case 7:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM7);
      break;
    case 8:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM8);
      break;
    case 9:
      myNDITrackingDevice->SetPortNumber(mitk::SerialCommunication::COM9);
      break;
    }
  
  //TODO: add tools

  //OpenConnection
  MITK_TEST_CONDITION( (myNDITrackingDevice->OpenConnection()), "Testing method OpenConnection().");

  //StartTracking
  MITK_TEST_CONDITION( (myNDITrackingDevice->StartTracking()), "Testing method StartTracking().");

  //TODO: test if tools are updated correctly

  //Stop tracking
  MITK_TEST_CONDITION( (myNDITrackingDevice->StopTracking()), "Testing method StopTracking().");

  //CloseConnection
  MITK_TEST_CONDITION( (myNDITrackingDevice->CloseConnection()), "Testing behavior of method CloseConnection().");

  // always end with this!
  MITK_TEST_END();
}