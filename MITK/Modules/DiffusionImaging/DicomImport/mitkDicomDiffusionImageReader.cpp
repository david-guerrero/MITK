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

#ifndef __mitkDicomDiffusionImageReader_cpp
#define __mitkDicomDiffusionImageReader_cpp

#include "mitkDicomDiffusionImageReader.h"

#include "itkImageSeriesReader.h"
//#include "itkImageFileReader.h"
//#include "itkImageRegion.h"
//#include "itkImageRegionIterator.h"
//#include "itkImageRegionConstIterator.h"
//#include "itkExceptionObject.h"
//#include "itkArray.h"
//#include "vnl/vnl_math.h"
//#include "itkProgressReporter.h"
//#include "itkMetaDataObject.h"

namespace mitk
{

// Destructor
  //template <class TPixelType, int TDimension>
  //DicomDiffusionImageReader<TPixelType, TDimension>
  //  ::~DicomDiffusionImageReader()
  //{
  //  //// Clear the eventual previous content of the MetaDictionary array
  //  //if( m_MetaDataDictionaryArray.size() )
  //  //  {
  //  //  for(unsigned int i=0; i<m_MetaDataDictionaryArray.size(); i++)
  //  //    {
  //  //    // each element is a raw pointer, delete them.
  //  //    delete m_MetaDataDictionaryArray[i];
  //  //    }
  //  //  }
  //  //m_MetaDataDictionaryArray.clear();
  //}


template <class TPixelType, const int TDimension>
void DicomDiffusionImageReader<TPixelType, TDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <class TPixelType, const int TDimension>
void DicomDiffusionImageReader<TPixelType, TDimension>
::GenerateOutputInformation(void)
{
  typename OutputImageType::Pointer output = this->GetOutput();
  typedef itk::ImageSeriesReader<InputImageType> ReaderType;

  // Read the first (or last) volume and use its size.
  if (m_Headers.size() > 0)
  {
    typename ReaderType::Pointer reader = ReaderType::New();

    try
    {
      // Read the image
      reader->SetFileNames (m_Headers[0]->m_DicomFilenames);
      reader->UpdateOutputInformation();

      output->SetSpacing( reader->GetOutput()->GetSpacing() );   // Set the image spacing
      output->SetOrigin( reader->GetOutput()->GetOrigin() );     // Set the image origin
      output->SetDirection( reader->GetOutput()->GetDirection() );  // Set the image direction
      output->SetLargestPossibleRegion( reader->GetOutput()->GetLargestPossibleRegion() );
      output->SetVectorLength( m_Headers.size() );
    }
    catch (itk::ExceptionObject &e)
    {
      throw e;
    }
  }
  else
  {
    itkExceptionMacro(<< "At least one filename is required." );
  }
}


template <class TPixelType, const int TDimension>
  void
    DicomDiffusionImageReader<TPixelType, TDimension>
    ::EnlargeOutputRequestedRegion(itk::DataObject *output)
  {
    typename OutputImageType::Pointer out = dynamic_cast<OutputImageType*>(output);
    out->SetRequestedRegion( out->GetLargestPossibleRegion() );
}


template <class TPixelType, const int TDimension>
void DicomDiffusionImageReader<TPixelType, TDimension>
::GenerateData()
{
  typedef itk::ImageSeriesReader<InputImageType> ReaderType;

  typename OutputImageType::Pointer output = this->GetOutput();

  typedef typename OutputImageType::RegionType   RegionType;
  RegionType requestedRegion = output->GetRequestedRegion();

  // Each file must have the same size.
  SizeType validSize = requestedRegion.GetSize();

  int numberOfVolumes = static_cast<int>(m_Headers.size());

  // Allocate the output buffer
  output->SetBufferedRegion( requestedRegion );
  output->Allocate();

  itk::ProgressReporter progress(this, 0, 
                            m_Headers.size(),
                            m_Headers.size());

  itk::ImageRegionIterator<OutputImageType> ot (output, requestedRegion );

  // Clear the eventual previous content of the MetaDictionary array
  //if( m_MetaDataDictionaryArray.size() )
  //  {
  //  for(unsigned int i=0; i<m_MetaDataDictionaryArray.size(); i++)
  //    {
  //    // each element is a raw pointer, delete them.
  //    delete m_MetaDataDictionaryArray[i];
  //    }
  //  }
  //m_MetaDataDictionaryArray.clear();

  typename OutputImageType::PixelType vec;

  for (int i = 0; i < numberOfVolumes; i ++)
    {
    
    MITK_INFO << "Loading volume " << i+1 << "/" << numberOfVolumes;
    typename ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileNames(m_Headers[i]->m_DicomFilenames);
    reader->UpdateLargestPossibleRegion();

    if (reader->GetOutput()->GetRequestedRegion().GetSize() != validSize)
      {
      itkExceptionMacro(<< "Size mismatch!");
      }

    itk::ImageRegionConstIterator<InputImageType> it (reader->GetOutput(),
                                               reader->GetOutput()->GetLargestPossibleRegion());

    while (!it.IsAtEnd())
      {
        vec = ot.Get();
        vec.SetElement(i, it.Get());
        ot.Set(vec);
        ++it;
        ++ot;
      }
    ot = ot.Begin();
    progress.CompletedPixel();
    }
}

} //namespace MITK

#endif
