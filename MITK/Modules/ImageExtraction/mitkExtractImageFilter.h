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

#ifndef mitkExtractImageFilter_h_Included
#define mitkExtractImageFilter_h_Included

#include "mitkCommon.h"
#include "ImageExtractionExports.h"
#include "mitkImageToImageFilter.h"

#include "itkImage.h"

namespace mitk
{

/**
  \brief Extracts a 2D slice from a 3D image.

  \sa SegTool2D
  \sa OverwriteSliceImageFilter

  \ingroup Process
  \ingroup ToolManagerEtAl

  There is a separate page describing the general design of QmitkInteractiveSegmentation: \ref QmitkSegmentationTechnicalPage

  This class takes a 3D mitk::Image as input and tries to extract one slice from it.

  Two parameters determine which slice is extracted: the "slice dimension" is that one, which is constant for all points in the plane, e.g. transversal would mean 2.
  The "slice index" is the slice index in the image direction you specified with "affected dimension". Indices count from zero.

  Output will not be set if there was a problem extracting the desired slice.

  Last contributor: $Author$
*/
class ImageExtraction_EXPORT ExtractImageFilter : public ImageToImageFilter
{
  public:

    mitkClassMacro(ExtractImageFilter, ImageToImageFilter);
    itkNewMacro(ExtractImageFilter);

    /**
      \brief Which slice to extract (first one has index 0).
    */
    itkSetMacro(SliceIndex, unsigned int);
    itkGetConstMacro(SliceIndex, unsigned int);

    /**
      \brief The orientation of the slice to be extracted.

      \a Parameter SliceDimension Number of the dimension which is constant for all pixels of the desired slice (e.g. 2 for transversal)
    */
    itkSetMacro(SliceDimension, unsigned int);
    itkGetConstMacro(SliceDimension, unsigned int);

    /**
      \brief Time step of the image to be extracted.
     */
    itkSetMacro(TimeStep, unsigned int);
    itkGetConstMacro(TimeStep, unsigned int);

  protected:

    ExtractImageFilter(); // purposely hidden
    virtual ~ExtractImageFilter();

    virtual void GenerateOutputInformation();
    virtual void GenerateInputRequestedRegion();

    virtual void GenerateData();

    template<typename TPixel, unsigned int VImageDimension>
    void ItkImageProcessing( itk::Image<TPixel,VImageDimension>* image );

    unsigned int m_SliceIndex;
    unsigned int m_SliceDimension;
    unsigned int m_TimeStep;
};

} // namespace

#endif


