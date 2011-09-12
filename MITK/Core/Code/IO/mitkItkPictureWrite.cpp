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


#include "mitkItkPictureWrite.h"
#include <mitkInstantiateAccessFunctions.h>

#include <itkNumericSeriesFileNames.h>
#include <itkImageSeriesWriter.h>
#include <itkRescaleIntensityImageFilter.h>

template < typename TPixel, unsigned int VImageDimension > 
void _mitkItkPictureWrite(itk::Image< TPixel, VImageDimension >* itkImage, const std::string& fileName)
{
  typedef itk::Image< TPixel, VImageDimension > TImageType;

  typedef itk::Image<unsigned char,3> OutputImage3DType;
  typedef itk::Image<unsigned char,2> OutputImage2DType;
  typename itk::RescaleIntensityImageFilter<TImageType, OutputImage3DType>::Pointer rescaler = itk::RescaleIntensityImageFilter<TImageType, OutputImage3DType>::New();
  rescaler->SetInput(itkImage);
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  itk::NumericSeriesFileNames::Pointer numericFileNameWriter = itk::NumericSeriesFileNames::New();
  itk::ImageSeriesWriter<OutputImage3DType, OutputImage2DType>::Pointer writer = itk::ImageSeriesWriter<OutputImage3DType, OutputImage2DType >::New();

  int numberOfSlices = itkImage->GetLargestPossibleRegion().GetSize()[2];
  std::string finalFileName = fileName;
  std::string::size_type pos = fileName.find_last_of(".",fileName.length()-1);
  if(pos==std::string::npos)
    finalFileName.append(".%d.png");
  else
    finalFileName.insert(pos,".%d");

  numericFileNameWriter->SetEndIndex(numberOfSlices);
  numericFileNameWriter->SetSeriesFormat(finalFileName.c_str());
  numericFileNameWriter->Modified();

  writer->SetInput( rescaler->GetOutput() );
  writer->SetFileNames(numericFileNameWriter->GetFileNames());
  writer->Update();
}

#define InstantiateAccessFunction__mitkItkPictureWrite(pixelType, dim) \
  template MITK_CORE_EXPORT void _mitkItkPictureWrite(itk::Image<pixelType,dim>*, const std::string&);

InstantiateAccessFunction(_mitkItkPictureWrite)

// typedef itk::Image<itk::RGBPixel<unsigned char>, 2>  itkImageRGBUC2;
// template <> void _mitkItkImageWrite<itk::RGBPixel<unsigned char>, 2>(itkImageRGBUC2* itkImage, const std::string& fileName)
// {
//   typedef itkImageRGBUC2 TImageType;
// 
//   itk::ImageFileWriter<TImageType>::Pointer writer = itk::ImageFileWriter<TImageType>::New();
//   writer->SetInput( itkImage );
//   writer->SetFileName( fileName.c_str() );
//   writer->Update();
// };
// 
// typedef itk::Image<itk::RGBPixel<unsigned char>, 3>  itkImageRGBUC3;
// template <> void _mitkItkImageWrite<itk::RGBPixel<unsigned char>, 3>(itkImageRGBUC3* itkImage, const std::string& fileName)
// {
//   typedef itkImageRGBUC3 TImageType;
// 
//   itk::ImageFileWriter<TImageType>::Pointer writer = itk::ImageFileWriter<TImageType>::New();
//   writer->SetInput( itkImage );
//   writer->SetFileName( fileName.c_str() );
//   writer->Update();
// };
// 
// typedef itk::Image<itk::DiffusionTensor3D<float>, 3>  itkImageDTIF3;
// template <> void _mitkItkImageWrite<itk::DiffusionTensor3D<float>, 3>(itkImageDTIF3* itkImage, const std::string& fileName)
// {
//   typedef itkImageDTIF3 TImageType;
// 
//   itk::ImageFileWriter<TImageType>::Pointer writer = itk::ImageFileWriter<TImageType>::New();
//   writer->SetInput( itkImage );
//   writer->SetFileName( fileName.c_str() );
//   writer->Update();
// };
// 
// typedef itk::Image<itk::DiffusionTensor3D<double>, 3>  itkImageDTID3;
// template <> void _mitkItkImageWrite<itk::DiffusionTensor3D<double>, 3>(itkImageDTID3* itkImage, const std::string& fileName)
// {
//   typedef itkImageDTID3 TImageType;
// 
//   itk::ImageFileWriter<TImageType>::Pointer writer = itk::ImageFileWriter<TImageType>::New();
//   writer->SetInput( itkImage );
//   writer->SetFileName( fileName.c_str() );
//   writer->Update();
// };
// 
// typedef itk::Image<itk::DiffusionTensor3D<float>, 2>  itkImageDTIF2;
// template <> void _mitkItkImageWrite<itk::DiffusionTensor3D<float>, 2>(itkImageDTIF2* itkImage, const std::string& fileName)
// {
//   typedef itkImageDTIF2 TImageType;
// 
//   itk::ImageFileWriter<TImageType>::Pointer writer = itk::ImageFileWriter<TImageType>::New();
//   writer->SetInput( itkImage );
//   writer->SetFileName( fileName.c_str() );
//   writer->Update();
// };
// 
// typedef itk::Image<itk::DiffusionTensor3D<double>, 2>  itkImageDTID2;
// template <> void _mitkItkImageWrite<itk::DiffusionTensor3D<double>, 2>(itkImageDTID2* itkImage, const std::string& fileName)
// {
//   typedef itkImageDTID2 TImageType;
// 
//   itk::ImageFileWriter<TImageType>::Pointer writer = itk::ImageFileWriter<TImageType>::New();
//   writer->SetInput( itkImage );
//   writer->SetFileName( fileName.c_str() );
//   writer->Update();
// };


