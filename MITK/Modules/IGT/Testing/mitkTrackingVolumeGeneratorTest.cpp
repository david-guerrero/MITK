/*====================================================================
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <mitkTrackingVolumeGenerator.h>
#include <mitkClaronTrackingDevice.h>
#include "mitkCommon.h"
#include "mitkTestingMacros.h"
#include "vtkPolyData.h"
#include <iostream>
#include <mitkSurface.h>

class mitkTrackingVolumeGeneratorTestClass
{
public:

    static void TestInstantiation()
    {
        // let's create an object of our class
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New();
        MITK_TEST_CONDITION_REQUIRED(myTVGenerator.IsNotNull(),"Testing instantiation");

    }

    static void TestTrackingSystemNotSpecified()
    {
        MITK_TEST_OUTPUT(<<"---- Testing Trackingsystem not specified ----");
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New();
        MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::TrackingSystemNotSpecified), "Tracking System not specified:");
        myTVGenerator->Update();
        mitk::Surface::Pointer volume = myTVGenerator->GetOutput();
        MITK_TEST_CONDITION((volume->IsEmptyTimeStep(0) == false),"Output contains data");
    }

    static void TestClaronTrackingVolume()
    {
        MITK_TEST_OUTPUT(<< "---- Testing MicronTracker 2 Tracking Volume ----");
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New ();
        myTVGenerator->SetTrackingDeviceType(mitk::ClaronMicron);
        MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::ClaronMicron),"loading MicronTracker Volume data:");
        myTVGenerator->Update();
        mitk::Surface::Pointer volume = myTVGenerator->GetOutput();
        MITK_TEST_CONDITION((volume->IsEmptyTimeStep(0) == false),"Output contains data");
    }

    static void TestNDIAuroraTrackingVolume()
    {
        MITK_TEST_OUTPUT(<< "---- Testing NDI Aurora Tracking Volume ----");
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New ();
        myTVGenerator->SetTrackingDeviceType(mitk::NDIAurora);
        MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::NDIAurora),"loading Aurora Volume data:");
        myTVGenerator->Update();
        mitk::Surface::Pointer volume = myTVGenerator->GetOutput();
        MITK_TEST_CONDITION((volume->IsEmptyTimeStep(0) == false),"Output contains data");
    }

    static void TestNDIPolarisTrackingVolume()
    {
        MITK_TEST_OUTPUT(<< "---- Testing NDI Polaris Tracking Volume ----");
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New ();
        myTVGenerator->SetTrackingDeviceType(mitk::NDIPolaris);
        MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::NDIPolaris),"loading Polaris Volume data:");
        myTVGenerator->Update();
        mitk::Surface::Pointer volume = myTVGenerator->GetOutput();
        MITK_TEST_CONDITION((volume->IsEmptyTimeStep(0) == false),"Output contains data");
    }

    static void TestIntuitiveDaVinciTrackingVolume()
    {
        MITK_TEST_OUTPUT(<< "---- Testing Intuitive Da Vinci Tracking Volume ----");
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New ();
        myTVGenerator->SetTrackingDeviceType(mitk::IntuitiveDaVinci);
        MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::IntuitiveDaVinci),"loading Da Vinci Volume data:");
        myTVGenerator->Update();
        mitk::Surface::Pointer volume = myTVGenerator->GetOutput();
        MITK_TEST_CONDITION((volume->IsEmptyTimeStep(0) == false),"Output contains data");
    }

    static void TestInvalidInputBehaviour()
    {
        MITK_TEST_OUTPUT(<< "---- Testing Invalid Inputs (errors should occure) ----");
        mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New ();
        myTVGenerator->SetTrackingDeviceType(mitk::TrackingSystemInvalid); //MicroBird not implemented yet, so using as test dummy
        MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::TrackingSystemInvalid),"testing device type");
        myTVGenerator->Update();
        MITK_TEST_CONDITION(myTVGenerator->GetOutput()->GetVtkPolyData()->GetNumberOfVerts()==0,"testing (invalid) output");   
    }

    static void TestSetTrackingDevice()
    {
      MITK_TEST_OUTPUT(<< "---- Testing method SetTrackingDevice() ----");
      mitk::ClaronTrackingDevice::Pointer testTrackingDevice = mitk::ClaronTrackingDevice::New();
      mitk::TrackingVolumeGenerator::Pointer myTVGenerator = mitk::TrackingVolumeGenerator::New ();
      myTVGenerator->SetTrackingDevice(dynamic_cast<mitk::TrackingDevice*>(testTrackingDevice.GetPointer()));
      MITK_TEST_CONDITION((myTVGenerator->GetTrackingDeviceType() == mitk::ClaronMicron),"testing SetTrackingDevice()");
    
    }


    /* The isInside() method is not implemented so far. So please activate is as soon as this is done. Then we could load
     * the different Trackingvolumens (Polaris, MicronTracker, etc) and test different points inside and outside in this method.
    static void TestIsInside()
    {
    MITK_TEST_OUTPUT(<< "---- Testing IsInside-Method ----")
    mitk::TrackingVolume::Pointer myTrackingVolume = mitk::TrackingVolume::New();

    mitk::Point3D p1;
    mitk::FillVector3D(p1,(float)0,(float)0,(float)0);
    MITK_TEST_CONDITION(myTrackingVolume->IsInside(p1)==false,"... successfull")
    }
    */


};

/** This function is testing the TrackingVolume class. */
int mitkTrackingVolumeGeneratorTest(int /* argc */, char* /*argv*/[])
{
    MITK_TEST_BEGIN("TrackingVolumeGenerator");

    mitkTrackingVolumeGeneratorTestClass::TestInstantiation();
    mitkTrackingVolumeGeneratorTestClass::TestTrackingSystemNotSpecified ();
    mitkTrackingVolumeGeneratorTestClass::TestClaronTrackingVolume();
    mitkTrackingVolumeGeneratorTestClass::TestNDIAuroraTrackingVolume();
    mitkTrackingVolumeGeneratorTestClass::TestNDIPolarisTrackingVolume();
    mitkTrackingVolumeGeneratorTestClass::TestIntuitiveDaVinciTrackingVolume();
    mitkTrackingVolumeGeneratorTestClass::TestInvalidInputBehaviour();
    mitkTrackingVolumeGeneratorTestClass::TestSetTrackingDevice();
    //mitkTrackingVolumeTestClass::TestIsInside(); Activate this code when method isInside() is implemented!

    MITK_TEST_END() ;
}



