/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-03-12 14:05:50 +0100 (Fr, 12 Mrz 2010) $
Version:   $Revision: 16010 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <mitkTestingMacros.h>
#include <mitkToFOpenCVImageGrabber.h>

#include <mitkImageDataItem.h>
#include <mitkPicFileReader.h>
#include <mitkToFConfig.h>
#include <mitkToFImageGrabberCreator.h>

static bool CompareImages(mitk::Image::Pointer mitkImage, cv::Mat openCVImage)
{
  float equal = true;
  if ((mitkImage->GetDimension(0)!=openCVImage.cols)||(mitkImage->GetDimension(1)!=openCVImage.rows))
  {
    equal = false;
  }
  for (unsigned int i=0; i<openCVImage.cols; i++)
  {
    for (unsigned int j=0; j<openCVImage.rows; j++)
    {
      mitk::Index3D currentIndex;
      currentIndex[0] = i;
      currentIndex[1] = j;
      currentIndex[2] = 0;
      float mitkImageValue = mitkImage->GetPixelValueByIndex(currentIndex);
      float openCVImageValue = openCVImage.at<float>(j,i);
      if (!mitk::Equal(mitkImageValue,openCVImageValue))
      {
        equal = false;
      }
    }
  }
  return equal;
}

/**Documentation
 *  test for the class "ToFOpenCVImageGrabber".
 */
int mitkToFOpenCVImageGrabberTest(int /* argc */, char* /*argv*/[])
{
  MITK_TEST_BEGIN("ToFOpenCVImageGrabber");

  std::string dirName = MITK_TOF_DATA_DIR;
  mitk::ToFImageGrabber::Pointer tofImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetMITKPlayerImageGrabber();
  std::string distanceFileName = dirName + "/PMDCamCube2_MF0_IT0_1Images_DistanceImage.pic";
  tofImageGrabber->SetProperty("DistanceImageFileName",mitk::StringProperty::New(distanceFileName));
  std::string amplitudeFileName = dirName + "/PMDCamCube2_MF0_IT0_1Images_AmplitudeImage.pic";
  tofImageGrabber->SetProperty("AmplitudeImageFileName",mitk::StringProperty::New(amplitudeFileName));
  std::string intensityFileName = dirName + "/PMDCamCube2_MF0_IT0_1Images_IntensityImage.pic";
  tofImageGrabber->SetProperty("IntensityImageFileName",mitk::StringProperty::New(intensityFileName));

  mitk::PicFileReader::Pointer mitkFileReader = mitk::PicFileReader::New();
  mitkFileReader->SetFileName(distanceFileName);
  mitkFileReader->Update();
  mitk::Image::Pointer image = mitkFileReader->GetOutput();

  mitk::ToFOpenCVImageGrabber::Pointer tofOpenCVImageGrabber = mitk::ToFOpenCVImageGrabber::New();
  tofOpenCVImageGrabber->SetToFImageGrabber(tofImageGrabber);
  MITK_TEST_CONDITION_REQUIRED(tofImageGrabber==tofOpenCVImageGrabber->GetToFImageGrabber(),"Test Set/GetToFImageGrabber()");
  MITK_TEST_OUTPUT(<<"Call StartCapturing()");
  tofOpenCVImageGrabber->StartCapturing();
  cv::Mat cvImage = tofOpenCVImageGrabber->GetImage();
  MITK_TEST_CONDITION_REQUIRED(CompareImages(image,cvImage),"Test distance image");
  mitkFileReader->SetFileName(amplitudeFileName);
  mitkFileReader->Update();
  image = mitkFileReader->GetOutput();
  tofOpenCVImageGrabber->SetImageType(1);
  cvImage = tofOpenCVImageGrabber->GetImage();
  MITK_TEST_CONDITION_REQUIRED(CompareImages(image,cvImage),"Test amplitude image");
  mitkFileReader->SetFileName(intensityFileName);
  mitkFileReader->Update();
  image = mitkFileReader->GetOutput();
  tofOpenCVImageGrabber->SetImageType(2);
  cvImage = tofOpenCVImageGrabber->GetImage();
  MITK_TEST_CONDITION_REQUIRED(CompareImages(image,cvImage),"Test intensity image");
  MITK_TEST_OUTPUT(<<"Call StopCapturing()");
  tofOpenCVImageGrabber->StopCapturing();

  MITK_TEST_END();

}


