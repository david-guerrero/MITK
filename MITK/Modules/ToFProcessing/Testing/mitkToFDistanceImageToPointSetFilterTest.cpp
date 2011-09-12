/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: $
Version:   $Revision: $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <mitkTestingMacros.h>
#include <mitkToFDistanceImageToPointSetFilter.h>
#include <mitkToFDistanceImageToSurfaceFilter.h>

#include <mitkImage.h>
#include <mitkPointSet.h>
#include <mitkSurface.h>
#include <mitkToFProcessingCommon.h>
#include <mitkVector.h>

#include <itkImage.h>
#include <itkImageRegionIterator.h>

#include <vtkPolyData.h>

/**Documentation
*  test for the class "ToFDistanceImageToPointSetFilter".
*/
mitk::PointSet::Pointer CreateTestPointSet()
{
  mitk::PointSet::Pointer subSet = mitk::PointSet::New();
  mitk::Point3D point;
  point[0] = 10;
  point[1] = 20;
  point[2] = 0;
  subSet->InsertPoint(0,point);
  point[0] = 100;
  point[1] = 150;
  point[2] = 0;
  subSet->InsertPoint(1,point);
  point[0] = 110;
  point[1] = 30;
  point[2] = 0;
  subSet->InsertPoint(2,point);
  point[0] = 40;
  point[1] = 200;
  point[2] = 0;
  subSet->InsertPoint(3,point);
  return subSet;
}

inline static mitk::Image::Pointer CreateTestImageWithPointSet(mitk::ScalarType pixelValue, unsigned int dimX, unsigned int dimY, mitk::PointSet::Pointer subSet)
{
  typedef itk::Image<mitk::ScalarType,2> ItkImageType2D;
  typedef itk::ImageRegionIterator<ItkImageType2D> ItkImageRegionIteratorType2D;

  ItkImageType2D::Pointer image = ItkImageType2D::New();
  ItkImageType2D::IndexType start;
  start[0] = 0;
  start[1] = 0;
  ItkImageType2D::SizeType size;
  size[0] = dimX;
  size[1] = dimY;
  ItkImageType2D::RegionType region;
  region.SetSize(size);
  region.SetIndex( start);
  ItkImageType2D::SpacingType spacing;
  spacing[0] = 1.0;
  spacing[1] = 1.0;

  image->SetRegions( region );
  image->SetSpacing ( spacing );
  image->Allocate();

  //Obtaining image data from ToF camera//

  //Correlate inten values to PixelIndex//
  ItkImageRegionIteratorType2D imageIterator(image,image->GetLargestPossibleRegion());
  imageIterator.GoToBegin();

  while (!imageIterator.IsAtEnd())
  {
    imageIterator.Set(pixelValue);
    ++imageIterator;
  }
  // distances varying from pixelValue
  std::vector<mitk::ScalarType> distances;
  distances.push_back(50);
  distances.push_back(500);
  distances.push_back(2050);
  distances.push_back(300);
  // set the pixel values for the subset
  for (unsigned int i=0; i<subSet->GetSize(); i++)
  {
    mitk::Point3D point = subSet->GetPoint(i);
    ItkImageType2D::IndexType index;
    index[0] = point[0];
    index[1] = point[1];
    mitk::ScalarType distance = distances.at(i);
    image->SetPixel(index,distance);
  }
  mitk::Image::Pointer mitkImage = mitk::Image::New();
  mitk::CastToMitkImage(image,mitkImage);
  return mitkImage;
}

bool PointSetsEqual(mitk::PointSet::Pointer pointSet1, mitk::PointSet::Pointer pointSet2)
{
  bool pointSetsEqual = true;
  if (pointSet1->GetSize()==pointSet2->GetSize())
  {
    for (unsigned int i=0; i<pointSet1->GetSize(); i++)
    {
      mitk::Point3D expectedPoint = pointSet1->GetPoint(i);
      mitk::Point3D resultPoint = pointSet2->GetPoint(i);
      if (!mitk::Equal(expectedPoint,resultPoint))
      {
        pointSetsEqual = false;
      }
    }
  }
  else
  {
    pointSetsEqual = false;
  }
  return pointSetsEqual;
}

int mitkToFDistanceImageToPointSetFilterTest(int /* argc */, char* /*argv*/[])
{
  MITK_TEST_BEGIN("ToFDistanceImageToPointSetFilter");

  mitk::ToFDistanceImageToPointSetFilter::Pointer filter = mitk::ToFDistanceImageToPointSetFilter::New();
  //create test sub set
  MITK_INFO<<"Create test pointset";
  mitk::PointSet::Pointer subSet = CreateTestPointSet();
  //create test image
  unsigned int dimX = 204;
  unsigned int dimY = 204;
  MITK_INFO<<"Create test image";
  mitk::Image::Pointer image = CreateTestImageWithPointSet(1000.0f,dimX,dimY,subSet);
  //initialize intrinsic parameters
  //initialize intrinsic parameters with some arbitrary values
  mitk::ToFProcessingCommon::ToFPoint2D interPixelDistance;
  interPixelDistance[0] = 0.04564;
  interPixelDistance[1] = 0.0451564;
  mitk::ToFProcessingCommon::ToFScalarType focalLengthX = 295.78960;
  mitk::ToFProcessingCommon::ToFScalarType focalLengthY = 296.348535;
  mitk::ToFProcessingCommon::ToFScalarType focalLength = (focalLengthX*interPixelDistance[0]+focalLengthY*interPixelDistance[1])/2.0;
  mitk::ToFProcessingCommon::ToFScalarType k1=-0.36,k2=-0.14,p1=0.001,p2=-0.00;
  mitk::ToFProcessingCommon::ToFPoint2D principalPoint;
  principalPoint[0] = 103.576546;
  principalPoint[1] = 100.1532;
  mitk::CameraIntrinsics::Pointer cameraIntrinsics = mitk::CameraIntrinsics::New();
  cameraIntrinsics->SetFocalLength(focalLengthX,focalLengthY);
  cameraIntrinsics->SetPrincipalPoint(principalPoint[0],principalPoint[1]);
  cameraIntrinsics->SetDistorsionCoeffs(k1,k2,p1,p2);
  // test SetCameraIntrinsics()
  filter->SetCameraIntrinsics(cameraIntrinsics);
  MITK_TEST_CONDITION_REQUIRED((focalLengthX==filter->GetCameraIntrinsics()->GetFocalLengthX()),"Testing SetCameraIntrinsics with focalLength");
  mitk::ToFProcessingCommon::ToFPoint2D pp;
  pp[0] = filter->GetCameraIntrinsics()->GetPrincipalPointX();
  pp[1] = filter->GetCameraIntrinsics()->GetPrincipalPointY();
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(principalPoint,pp),"Testing SetCameraIntrinsics with principalPoint()");
  // test SetInterPixelDistance()

  filter->SetInterPixelDistance(interPixelDistance);
  mitk::ToFProcessingCommon::ToFPoint2D ipD = filter->GetInterPixelDistance();
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(ipD,interPixelDistance),"Testing Set/GetInterPixelDistance()");

  // test Set/GetInput()
  filter->SetInput(image);
  MITK_TEST_CONDITION_REQUIRED((image==filter->GetInput()),"Testing Set/GetInput()");

  // test filter without subset
  MITK_INFO<<"Test filter without subset";
  mitk::PointSet::Pointer expectedResult = mitk::PointSet::New();
  unsigned int counter = 0;
  for (unsigned int j=0; j<dimY; j++)
  {
    for (unsigned int i=0; i<dimX; i++)
    {
      mitk::Index3D index;
      index[0] = i;
      index[1] = j;
      index[2] = 0;
      mitk::ScalarType distance = image->GetPixelValueByIndex(index);
      mitk::Point3D coordinate = mitk::ToFProcessingCommon::IndexToCartesianCoordinates(i,j,distance,focalLength,interPixelDistance,principalPoint);
      expectedResult->InsertPoint(counter,coordinate);
      counter++;
    }
  }
  filter->Update();
  mitk::PointSet::Pointer result = filter->GetOutput();
  MITK_TEST_CONDITION_REQUIRED((expectedResult->GetSize()==result->GetSize()),"Test if point set size is equal");
  MITK_TEST_CONDITION_REQUIRED(PointSetsEqual(expectedResult,result),"Testing filter without subset");

  // compare filter result with ToFDistanceImageToSurfaceFilter
  mitk::ToFDistanceImageToSurfaceFilter::Pointer surfaceFilter = mitk::ToFDistanceImageToSurfaceFilter::New();
  surfaceFilter->SetInput(image);
  surfaceFilter->SetInterPixelDistance(interPixelDistance);
  surfaceFilter->SetCameraIntrinsics(cameraIntrinsics);  
  mitk::Surface::Pointer surface = surfaceFilter->GetOutput();
  surface->Update();
  // create point set from surface
  vtkPolyData* polyData = surface->GetVtkPolyData();
  int numberOfPoints = polyData->GetNumberOfPoints();
  mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
  for (int i=0; i<numberOfPoints; i++)
  {
    double* currentPoint = polyData->GetPoint(i);
    mitk::Point3D point;
    point[0] = currentPoint[0];
    point[1] = currentPoint[1];
    point[2] = currentPoint[2];
    pointSet->InsertPoint(i,point);
  }
  MITK_TEST_CONDITION_REQUIRED((pointSet->GetSize()==result->GetSize()),"Test if point set size is equal");
  MITK_TEST_CONDITION_REQUIRED(PointSetsEqual(pointSet,result),"Compare with surface points");
  
  // test filter with subset
  MITK_INFO<<"Test filter with subset";
  filter = mitk::ToFDistanceImageToPointSetFilter::New();
  filter->SetInput(image);
  filter->SetInterPixelDistance(interPixelDistance);
  filter->SetCameraIntrinsics(cameraIntrinsics);
  expectedResult = mitk::PointSet::New();
  counter = 0;
  for (unsigned int i=0; i<subSet->GetSize(); i++)
  {
    mitk::Point3D point = subSet->GetPoint(i);
    mitk::Index3D index;
    index[0] = point[0];
    index[1] = point[1];
    index[2] = 0;
    mitk::ScalarType distance = image->GetPixelValueByIndex(index);
    mitk::Point3D coordinate = mitk::ToFProcessingCommon::IndexToCartesianCoordinates(point[0],point[1],
                                                                                      distance,focalLength,interPixelDistance,principalPoint);
    expectedResult->InsertPoint(counter,coordinate);
    counter++;
  }
  filter->SetSubset(subSet);
  filter->Modified();
  filter->Update();
  result = filter->GetOutput();
  MITK_TEST_CONDITION_REQUIRED((expectedResult->GetSize()==result->GetSize()),"Test if point set size is equal");
  MITK_TEST_CONDITION_REQUIRED(PointSetsEqual(expectedResult,result),"Testing filter with subset");

  MITK_TEST_END();

}

