/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-07-14 19:11:20 +0200 (Tue, 14 Jul 2009) $
Version:   $Revision: 18127 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkNrrdTensorImageReader.h"

#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"
#include "itkMetaDataObject.h"
#include "itkNrrdImageIO.h"
#include "itkDiffusionTensor3D.h"

#include "mitkITKImageImport.h"
#include "mitkImageDataItem.h"

namespace mitk
{

  void NrrdTensorImageReader
    ::GenerateData()
  {
    if ( m_FileName == "")
    {
      throw itk::ImageFileReaderException(__FILE__, __LINE__, "Sorry, the filename is empty!");
    }
    else
    {
      try
      {
        typedef itk::VectorImage<float,3> ImageType;
        itk::NrrdImageIO::Pointer io = itk::NrrdImageIO::New();
        typedef itk::ImageFileReader<ImageType> FileReaderType;
        FileReaderType::Pointer reader = FileReaderType::New();
        reader->SetImageIO(io);
        reader->SetFileName(this->m_FileName);
        reader->Update();
        ImageType::Pointer img = reader->GetOutput();

        typedef itk::Image<itk::DiffusionTensor3D<float>,3> VecImgType;
        VecImgType::Pointer vecImg = VecImgType::New();
        vecImg->SetSpacing( img->GetSpacing() );   // Set the image spacing
        vecImg->SetOrigin( img->GetOrigin() );     // Set the image origin
        vecImg->SetDirection( img->GetDirection() );  // Set the image direction
        vecImg->SetRegions( img->GetLargestPossibleRegion());
        vecImg->Allocate();

        itk::ImageRegionIterator<VecImgType> ot (vecImg, vecImg->GetLargestPossibleRegion() );
        ot = ot.Begin();

        itk::ImageRegionIterator<ImageType> it (img, img->GetLargestPossibleRegion() );
        it = it.Begin();

        typedef ImageType::PixelType  VarPixType;
        typedef VecImgType::PixelType FixPixType;
        int numComponents = img->GetNumberOfComponentsPerPixel();

        if (numComponents==6)
        {
          while (!it.IsAtEnd())
          {
            VarPixType vec = it.Get();
            FixPixType fixVec(vec.GetDataPointer());
            ot.Set(fixVec);
            ++ot;
            ++it;
          }
        }
        else if(numComponents==9)
        {
          while (!it.IsAtEnd())
          {
            VarPixType vec = it.Get();
            itk::DiffusionTensor3D<float> tensor;
            tensor.SetElement(0, vec.GetElement(0));
            tensor.SetElement(1, vec.GetElement(1));
            tensor.SetElement(2, vec.GetElement(2));
            tensor.SetElement(3, vec.GetElement(4));
            tensor.SetElement(4, vec.GetElement(5));
            tensor.SetElement(5, vec.GetElement(8));

            FixPixType fixVec(tensor);
            ot.Set(fixVec);
            ++ot;
            ++it;
          }
        }
        else
        {
          throw itk::ImageFileReaderException(__FILE__, __LINE__, "Image has wrong number of pixel components!");
        }

        this->GetOutput()->InitializeByItk(vecImg.GetPointer());
        this->GetOutput()->SetVolume(vecImg->GetBufferPointer());

      }
      catch(std::exception& e)
      {
        throw itk::ImageFileReaderException(__FILE__, __LINE__, e.what());
      }
      catch(...)
      {
        throw itk::ImageFileReaderException(__FILE__, __LINE__, "Sorry, an error occurred while reading the requested DTI file!");
      }
    }
  }

  void NrrdTensorImageReader::GenerateOutputInformation()
  {

  }



  const char* NrrdTensorImageReader
    ::GetFileName() const
  {
    return m_FileName.c_str();
  }


  void NrrdTensorImageReader
    ::SetFileName(const char* aFileName)
  {
    m_FileName = aFileName;
  }


  const char* NrrdTensorImageReader
    ::GetFilePrefix() const
  {
    return m_FilePrefix.c_str();
  }


  void NrrdTensorImageReader
    ::SetFilePrefix(const char* aFilePrefix)
  {
    m_FilePrefix = aFilePrefix;
  }


  const char* NrrdTensorImageReader
    ::GetFilePattern() const
  {
    return m_FilePattern.c_str();
  }


  void NrrdTensorImageReader
    ::SetFilePattern(const char* aFilePattern)
  {
    m_FilePattern = aFilePattern;
  }


  bool NrrdTensorImageReader
    ::CanReadFile(const std::string filename, const std::string /*filePrefix*/, const std::string /*filePattern*/)
  {
    // First check the extension
    if(  filename == "" )
    {
      return false;
    }
    std::string ext = itksys::SystemTools::GetFilenameLastExtension(filename);
    ext = itksys::SystemTools::LowerCase(ext);

    if (ext == ".hdti" || ext == ".dti")
    {
      return true;
    }

    return false;
  }

} //namespace MITK
