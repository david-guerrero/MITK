/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <mitkImage.h>
#include <mitkImageDataItem.h>
#include <mitkImageCast.h>

#include <itkImage.h>

#include <fstream>
#include <itkSmartPointerForwardReference.txx>
#include <mitkDataNodeFactory.h>

#include <vtkImageData.h>

#include <mitkTestingMacros.h>


int mitkImageTest(int argc, char* argv[])
{

  MITK_TEST_BEGIN(mitkImageTest);

  //Create Image out of nowhere
  mitk::Image::Pointer imgMem;
  mitk::PixelType pt(typeid(int));
  unsigned int dim[]={100,100,20};

  std::cout << "Testing creation of Image: ";
  imgMem=mitk::Image::New();
  if(imgMem.IsNull())
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing Initialize(const mitk::PixelType& type, unsigned int dimension, unsigned int *dimensions): ";
  imgMem->Initialize(mitk::PixelType(typeid(int)), 3, dim);
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing IsInitialized(): ";
  if(imgMem->IsInitialized()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing GetData(): ";
  int *p = (int*)imgMem->GetData();
  if(p==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Filling image: ";
  unsigned int i;
  unsigned int size = dim[0]*dim[1]*dim[2];
  for(i=0; i<size; ++i, ++p)
    *p= (signed int)i;
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Getting it again and compare with filled values: ";
  int *p2 = (int*)imgMem->GetData();
  if(p2==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  for(i=0; i<size; ++i, ++p2)
  {
    if(*p2!= (signed int)i )
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing IsInitialized(): ";
  if(imgMem->IsInitialized()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;
  
  std::cout << "Testing GetSliceData() and compare with filled values: ";
  p2 = (int*)imgMem->GetSliceData(dim[2]/2)->GetData();
  unsigned int xy_size = dim[0]*dim[1];
  unsigned int start_mid_slice = (dim[2]/2)*xy_size;
  for(i=0; i<xy_size; ++i, ++p2)
  {
    if(*p2!=(signed int)(i+start_mid_slice))
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout<<"[PASSED]"<<std::endl;

  //----
  mitkIpPicDescriptor *pic_slice=mitkIpPicClone(imgMem->GetSliceData(dim[2]/2)->GetPicDescriptor());
  imgMem=mitk::Image::New();

  std::cout << "Testing reinitializing via Initialize(const mitk::PixelType& type, unsigned int dimension, unsigned int *dimensions): ";
  imgMem->Initialize(mitk::PixelType(typeid(int)), 3, dim);
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing slice-wise filling via SetPicSlice(): ";
  for(i=0;i<dim[2];++i)
  {
    imgMem->SetPicSlice(pic_slice, i, 0, 0);
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Getting it again and compare with filled values: ";
  p2 = (int*)imgMem->GetData();
  if(p2==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  for(i=0; i<size; ++i, ++p2)
  {
    if(*p2!= (signed int)((i%xy_size)+start_mid_slice))
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing IsInitialized(): ";
  if(imgMem->IsInitialized()==false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Setting a copy of the volume once again: ";
  imgMem->SetPicVolume(mitkIpPicClone(imgMem->GetVolumeData(0)->GetPicDescriptor()),0);
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Set a slice with different content via SetPicSlice(): ";
  memset(pic_slice->data,0,xy_size*sizeof(int));
  imgMem->SetPicSlice(pic_slice, 1);

  std::cout << "Getting the volume again and compare the check the changed slice: ";
  p2 = (int*)imgMem->GetData();
  if(p2==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  p2+=xy_size;
  for(i=0; i<xy_size; ++i, ++p2)
  {
    if(*p2!=0)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout<<"[PASSED]"<<std::endl;


  std::cout << "Setting volume again: ";
  imgMem->SetVolume(imgMem->GetData());
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Set a slice with different content via SetSlice(): ";
  memset(pic_slice->data,0,xy_size*sizeof(int));
  imgMem->SetSlice(pic_slice->data, 0);

  std::cout << "Getting the volume again and compare the check the changed slice: ";
  p2 = (int*)imgMem->GetData();
  if(p2==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  for(i=0; i<xy_size; ++i, ++p2)
  {
    if(*p2!=0)
    {
      std::cout<<"[FAILED]"<<std::endl;
      return EXIT_FAILURE;
    }
  }
  std::cout<<"[PASSED]"<<std::endl;



  //std::cout << "Testing SetVolume(): ";
  //imgMem->SetVolume(data);
  //std::cout<<"[PASSED]"<<std::endl;

  mitkIpPicFree(pic_slice);

  //-----------------
  // geometry information for image
  mitk::Point3D origin;
  mitk::Vector3D right, bottom;
  mitk::Vector3D spacing;
  mitk::FillVector3D(origin, 17.0, 19.92, 7.83);
  mitk::FillVector3D(right, 1.0, 2.0, 3.0);
  mitk::FillVector3D(bottom, 0.0, -3.0, 2.0);
  mitk::FillVector3D(spacing, 0.78, 0.91, 2.23);

  std::cout << "Testing InitializeStandardPlane(rightVector, downVector, spacing): " << std::flush;
  mitk::PlaneGeometry::Pointer planegeometry = mitk::PlaneGeometry::New();
  planegeometry->InitializeStandardPlane(100, 100, right, bottom, &spacing);
  planegeometry->SetOrigin(origin);
  std::cout << "done" << std::endl;

  std::cout << "Testing Initialize(const mitk::PixelType& type, const mitk::Geometry3D& geometry, unsigned int slices) with PlaneGeometry and GetData(): ";
  imgMem->Initialize(mitk::PixelType(typeid(int)), *planegeometry);
  p = (int*)imgMem->GetData();
  if(p==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing Initialize(const mitk::PixelType& type, int sDim, const mitk::PlaneGeometry& geometry) and GetData(): ";
  imgMem->Initialize(mitk::PixelType(typeid(int)), 40, *planegeometry);
  p = (int*)imgMem->GetData();
  if(p==NULL)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  //-----------------
  // testing origin information and methods
  std::cout << "Testing correctness of origin via GetGeometry()->GetOrigin(): ";
  if( mitk::Equal(imgMem->GetGeometry()->GetOrigin(), origin) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of origin via GetTimeSlicedGeometry()->GetOrigin(): ";
  if( mitk::Equal(imgMem->GetTimeSlicedGeometry()->GetOrigin(), origin) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  mitk::FillVector3D(origin, 37.0, 17.92, 27.83);
  std::cout << "Setting origin via SetOrigin(origin): ";
  imgMem->SetOrigin(origin);
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of changed origin via GetGeometry()->GetOrigin(): ";
  if( mitk::Equal(imgMem->GetGeometry()->GetOrigin(), origin) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of changed origin via GetTimeSlicedGeometry()->GetOrigin(): ";
  if( mitk::Equal(imgMem->GetTimeSlicedGeometry()->GetOrigin(), origin) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of changed origin via GetSlicedGeometry()->GetGeometry2D(0)->GetOrigin(): ";
  if( mitk::Equal(imgMem->GetSlicedGeometry()->GetGeometry2D(0)->GetOrigin(), origin) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  //-----------------
  // testing spacing information and methods
  std::cout << "Testing correctness of spacing via GetGeometry()->GetSpacing(): ";
  if( mitk::Equal(imgMem->GetGeometry()->GetSpacing(), spacing) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of spacing via GetTimeSlicedGeometry()->GetSpacing(): ";
  if( mitk::Equal(imgMem->GetTimeSlicedGeometry()->GetSpacing(), spacing) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  mitk::FillVector3D(spacing, 7.0, 0.92, 1.83);
  std::cout << "Setting spacing via SetSpacing(spacing): ";
  imgMem->SetSpacing(spacing);
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of changed spacing via GetGeometry()->GetSpacing(): ";
  if( mitk::Equal(imgMem->GetGeometry()->GetSpacing(), spacing) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of changed spacing via GetTimeSlicedGeometry()->GetSpacing(): ";
  if( mitk::Equal(imgMem->GetTimeSlicedGeometry()->GetSpacing(), spacing) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  std::cout << "Testing correctness of changed spacing via GetSlicedGeometry()->GetGeometry2D(0)->GetSpacing(): ";
  if( mitk::Equal(imgMem->GetSlicedGeometry()->GetGeometry2D(0)->GetSpacing(), spacing) == false)
  {
    std::cout<<"[FAILED]"<<std::endl;
    return EXIT_FAILURE;
  }
  std::cout<<"[PASSED]"<<std::endl;

  //-----------------
  MITK_TEST_OUTPUT(<< "Testing SetImportChannel");
  mitk::Image::Pointer vecImg = mitk::Image::New();
  vecImg->Initialize(*imgMem->GetPixelType().GetTypeId(), *imgMem->GetGeometry(), 2 /* #channels */, 0 /*tDim*/ );
  vecImg->SetImportChannel(imgMem->GetData(), 0, mitk::Image::CopyMemory );
  vecImg->SetImportChannel(imgMem->GetData(), 1, mitk::Image::CopyMemory );
  std::cout<<"[PASSED]"<<std::endl;

  MITK_TEST_OUTPUT(<< " Testing whether IsValidSlice returns valid after SetImportChannel");
  MITK_TEST_CONDITION_REQUIRED( vecImg->IsValidSlice(0,0,1) , "");

  MITK_TEST_OUTPUT(<< " Testing whether CopyMemory worked");
  MITK_TEST_CONDITION_REQUIRED(imgMem->GetData() != vecImg->GetData(), "");

  MITK_TEST_OUTPUT(<< " Testing destruction after SetImportChannel");
  vecImg = NULL; 
  std::cout<<"[PASSED]"<<std::endl;

  //-----------------
  MITK_TEST_OUTPUT(<< "Testing initialization via vtkImageData");
  MITK_TEST_OUTPUT(<< " Setting up vtkImageData");
  vtkImageData* vtkimage = vtkImageData::New();
  vtkimage->Initialize();
  vtkimage->SetDimensions( 2, 3, 4);
  double vtkorigin[] =  {-350,-358.203, -1363.5};
  vtkimage->SetOrigin(vtkorigin);
  mitk::Point3D vtkoriginAsMitkPoint;
  mitk::vtk2itk(vtkorigin, vtkoriginAsMitkPoint);
  double vtkspacing[] =  {1.367, 1.367, 2};
  vtkimage->SetSpacing(vtkspacing);
  vtkimage->SetScalarType( VTK_SHORT );
  vtkimage->AllocateScalars();
  std::cout<<"[PASSED]"<<std::endl;
  
  MITK_TEST_OUTPUT(<< " Testing mitk::Image::Initialize(vtkImageData*, ...)");
  mitk::Image::Pointer mitkByVtkImage = mitk::Image::New();
  mitkByVtkImage ->Initialize(vtkimage);
  MITK_TEST_CONDITION_REQUIRED(mitkByVtkImage->IsInitialized(), "");

  MITK_TEST_OUTPUT(<< " vtkimage->Delete");
  vtkimage->Delete();
  std::cout<<"[PASSED]"<<std::endl;

  MITK_TEST_OUTPUT(<< " Testing whether spacing has been correctly initialized from vtkImageData");
  mitk::Vector3D spacing2 = mitkByVtkImage->GetGeometry()->GetSpacing();
  mitk::Vector3D vtkspacingAsMitkVector;
  mitk::vtk2itk(vtkspacing, vtkspacingAsMitkVector);
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(spacing2,vtkspacingAsMitkVector), "");

  MITK_TEST_OUTPUT(<< " Testing whether GetSlicedGeometry(0)->GetOrigin() has been correctly initialized from vtkImageData");
  mitk::Point3D origin2 = mitkByVtkImage->GetSlicedGeometry(0)->GetOrigin();
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(origin2,vtkoriginAsMitkPoint), "");

  MITK_TEST_OUTPUT(<< " Testing whether GetGeometry()->GetOrigin() has been correctly initialized from vtkImageData");
  origin2 = mitkByVtkImage->GetGeometry()->GetOrigin();
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(origin2,vtkoriginAsMitkPoint), "");

  MITK_TEST_OUTPUT(<< " Testing whether GetTimeSlicedGeometry()->GetOrigin() has been correctly initialized from vtkImageData");
  origin2 = mitkByVtkImage->GetTimeSlicedGeometry()->GetOrigin();
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(origin2,vtkoriginAsMitkPoint), "");

  // TODO test the following initializers on channel-incorporation
  //  void mitk::Image::Initialize(const mitk::PixelType& type, unsigned int dimension, unsigned int *dimensions, unsigned int channels)
  //  void mitk::Image::Initialize(const mitk::PixelType& type, int sDim, const mitk::Geometry2D& geometry2d, bool flipped, unsigned int channels, int tDim )
  //  void mitk::Image::Initialize(const mitk::Image* image) 
  //  void mitk::Image::Initialize(const mitkIpPicDescriptor* pic, int channels, int tDim, int sDim)

  //mitk::Image::Pointer vecImg = mitk::Image::New();
  //vecImg->Initialize(PixelType(typeid(float), 6, itk::ImageIOBase::SYMMETRICSECONDRANKTENSOR), *imgMem->GetGeometry(), 2 /* #channels */, 0 /*tDim*/, false /*shiftBoundingBoxMinimumToZero*/ );
  //vecImg->Initialize(PixelType(typeid(itk::Vector<float,6>)), *imgMem->GetGeometry(), 2 /* #channels */, 0 /*tDim*/, false /*shiftBoundingBoxMinimumToZero*/ );

  // testing access by index coordinates and by world coordinates
  
  mitk::DataNode::Pointer node;      
  mitk::DataNodeFactory::Pointer nodeReader = mitk::DataNodeFactory::New();
  MITK_TEST_CONDITION_REQUIRED(argc == 2, "Check if test image is accessible!"); 
  const std::string filename = std::string(argv[1]);
  try
  {
    nodeReader->SetFileName(filename);
    nodeReader->Update();
    node = nodeReader->GetOutput();      
  }
  catch(...) {
    MITK_TEST_FAILED_MSG(<< "Could not read file for testing: " << filename);
    return NULL;
  }  

  mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(node->GetData());
    
  // test by index coordinates
  mitk::Index3D index;
  mitk::FillVector3D(index, 55, 39, 50);
  MITK_TEST_OUTPUT(<< "Testing mitk::Image::GetPixelValueByIndex");
  double val = image->GetPixelValueByIndex(index);
  MITK_TEST_CONDITION_REQUIRED( mitk::Equal(val,112.22475433349609), "");
  
  //test by world coordinates
  MITK_TEST_OUTPUT(<< "Testing mitk::Image::GetPixelValueByWorldCoordinate");
  mitk::Point3D point;
  mitk::FillVector3D(point, -5.93752, 18.7199, 6.74218);
  val = image->GetPixelValueByWorldCoordinate(point);
  MITK_TEST_CONDITION_REQUIRED( mitk::Equal(val,94.456184387207031), "");

  MITK_TEST_OUTPUT(<< "Convert to index and access value by mitk::Image::GetPixelValueByIndex again");
  mitk::Index3D index2;
  image->GetGeometry()->WorldToIndex(point, index2);
  float val2 = image->GetPixelValueByIndex(index2);
  MITK_TEST_CONDITION_REQUIRED( mitk::Equal(val2,94.456184387207031), "");

  //access via itk
  MITK_TEST_OUTPUT(<< "Test conversion to itk::Image");
  typedef itk::Image<float,3> ItkFloatImage3D;
  ItkFloatImage3D::Pointer itkimage;
  mitk::CastToItkImage(image, itkimage);
  std::cout<<"[PASSED]"<<std::endl;
 
  MITK_TEST_OUTPUT(<< "Testing world->itk-physical->world consistency");
  mitk::Point3D itkPhysicalPoint;
  image->GetGeometry()->WorldToItkPhysicalPoint(point, itkPhysicalPoint);

  mitk::Point3D backTransformedPoint;
  image->GetGeometry()->ItkPhysicalPointToWorld(itkPhysicalPoint, backTransformedPoint);
  MITK_TEST_CONDITION_REQUIRED( mitk::Equal(point,backTransformedPoint), "");

  MITK_TEST_OUTPUT(<< "Compare value of pixel returned by mitk in comparison to itk");
  itk::Index<3> idx;  
  itkimage->TransformPhysicalPointToIndex(itkPhysicalPoint, idx);
  float valByItk = itkimage->GetPixel(idx);

  MITK_TEST_CONDITION_REQUIRED( mitk::Equal(valByItk,94.456184387207031), "");

  mitk::Image::Pointer cloneImage = image->Clone();
  MITK_TEST_CONDITION_REQUIRED(cloneImage->GetDimension() == image->GetDimension(), "Clone (testing dimension)");
  MITK_TEST_CONDITION_REQUIRED(cloneImage->GetPixelType() == image->GetPixelType(), "Clone (testing pixel type)");
  
  for (unsigned int i = 0u; i < cloneImage->GetDimension(); ++i)
  {
    MITK_TEST_CONDITION_REQUIRED(cloneImage->GetDimension(i) == image->GetDimension(i), "Clone (testing dimension " << i << ")");
  }

  
  MITK_TEST_END();

  return EXIT_SUCCESS;
}

