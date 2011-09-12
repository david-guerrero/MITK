/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
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

#include <mitkToFCompositeFilter.h>
#include <mitkInstantiateAccessFunctions.h>
//#include <mitkOclToFCompositeFilter.h>

#include <itkImage.h>

mitk::ToFCompositeFilter::ToFCompositeFilter() : m_ImageWidth(0), m_ImageHeight(0), m_ImageSize(0),
m_IplDistanceImage(NULL), m_IplOutputImage(NULL), m_ItkInputImage(NULL), m_ApplyTemporalMedianFilter(false),
m_ApplyMedianFilter(false), m_ApplyThresholdFilter(false), m_ApplyBilateralFilter(false), m_DataBuffer(NULL),
m_DataBufferCurrentIndex(0), m_DataBufferMaxSize(0), m_TemporalMedianFilterNumOfFrames(10), m_ThresholdFilterMin(1),
m_ThresholdFilterMax(7000), m_BilateralFilterDomainSigma(2), m_BilateralFilterRangeSigma(60), m_BilateralFilterKernelRadius(0)
{
}

mitk::ToFCompositeFilter::~ToFCompositeFilter()
{
  cvReleaseImage(&(this->m_IplDistanceImage));
  cvReleaseImage(&(this->m_IplOutputImage));
  if (m_DataBuffer!=NULL)
  {
    delete [] m_DataBuffer;
  }
}

void mitk::ToFCompositeFilter::SetInput(  mitk::Image* distanceImage )
{
  this->SetInput(0, distanceImage);
}

void mitk::ToFCompositeFilter::SetInput( unsigned int idx,  mitk::Image* distanceImage )
{
  if ((distanceImage == NULL) && (idx == this->GetNumberOfInputs() - 1)) // if the last input is set to NULL, reduce the number of inputs by one
  {
    this->SetNumberOfInputs(this->GetNumberOfInputs() - 1);
  }
  else
  {
    if (idx==0) //create IPL image holding distance data
    {
      if (distanceImage->GetData())
      {
        this->m_ImageWidth = distanceImage->GetDimension(0);
        this->m_ImageHeight = distanceImage->GetDimension(1);
        this->m_ImageSize = this->m_ImageWidth * this->m_ImageHeight * sizeof(float);

        if (this->m_IplDistanceImage != NULL)
        {
          cvReleaseImage(&(this->m_IplDistanceImage));
        }
        float* distanceFloatData = (float*)distanceImage->GetSliceData(0, 0, 0)->GetData();
        this->m_IplDistanceImage = cvCreateImage(cvSize(this->m_ImageWidth, this->m_ImageHeight), IPL_DEPTH_32F, 1);
        memcpy(this->m_IplDistanceImage->imageData, (void*)distanceFloatData, this->m_ImageSize);

        if (this->m_IplOutputImage != NULL)
        {
          cvReleaseImage(&(this->m_IplOutputImage));
        }
        this->m_IplOutputImage = cvCreateImage(cvSize(this->m_ImageWidth, this->m_ImageHeight), IPL_DEPTH_32F, 1);

        CreateItkImage(this->m_ItkInputImage);
      }
    }
    this->ProcessObject::SetNthInput(idx, distanceImage);   // Process object is not const-correct so the const_cast is required here
  }

  this->CreateOutputsForAllInputs();
}

 mitk::Image* mitk::ToFCompositeFilter::GetInput()
{
  return this->GetInput(0);
}

 mitk::Image* mitk::ToFCompositeFilter::GetInput( unsigned int idx )
{
  if (this->GetNumberOfInputs() < 1)
    return NULL; //TODO: geeignete exception werfen
  return static_cast< mitk::Image*>(this->ProcessObject::GetInput(idx));
}

void mitk::ToFCompositeFilter::GenerateData()
{
  // copy input 1...n to output 1...n
  for (unsigned int idx=0; idx<this->GetNumberOfOutputs(); idx++)
  {
    mitk::Image::Pointer outputImage = this->GetOutput(idx);
    mitk::Image::Pointer inputImage = this->GetInput(idx);
    if (outputImage.IsNotNull()&&inputImage.IsNotNull())
    {
      outputImage->CopyInformation(inputImage);
      outputImage->Initialize(inputImage);
      outputImage->SetSlice(inputImage->GetSliceData()->GetData());
    }
  }
  mitk::Image::Pointer outputDistanceImage = this->GetOutput(0);
  float* outputDistanceFloatData = (float*)outputDistanceImage->GetSliceData(0, 0, 0)->GetData();

  mitk::Image::Pointer inputDistanceImage = this->GetInput();

  // copy initial distance image to ipl image
  float* distanceFloatData = (float*)inputDistanceImage->GetSliceData(0, 0, 0)->GetData();
  memcpy(this->m_IplDistanceImage->imageData, (void*)distanceFloatData, this->m_ImageSize);
  if (m_ApplyThresholdFilter)
  {
    ProcessThresholdFilter(this->m_IplDistanceImage, this->m_ThresholdFilterMin, this->m_ThresholdFilterMax);
  }
  if (this->m_ApplyTemporalMedianFilter)
  {
    ProcessStreamedQuickSelectMedianImageFilter(this->m_IplDistanceImage, this->m_TemporalMedianFilterNumOfFrames);
  }
  if (this->m_ApplyMedianFilter)
  {
    ProcessCVMedianFilter(this->m_IplDistanceImage, this->m_IplOutputImage);
    memcpy( this->m_IplDistanceImage->imageData, this->m_IplOutputImage->imageData, this->m_ImageSize );
  }
  if (this->m_ApplyBilateralFilter)
  {
      float* itkFloatData = this->m_ItkInputImage->GetBufferPointer();
      memcpy(itkFloatData, this->m_IplDistanceImage->imageData, this->m_ImageSize );
      ItkImageType2D::Pointer itkOutputImage = ProcessItkBilateralFilter(this->m_ItkInputImage,
        this->m_BilateralFilterDomainSigma, this->m_BilateralFilterRangeSigma, this->m_BilateralFilterKernelRadius);
      memcpy( this->m_IplDistanceImage->imageData, itkOutputImage->GetBufferPointer(), this->m_ImageSize );

    //ProcessCVBilateralFilter(this->m_IplDistanceImage, this->m_OutputIplImage, domainSigma, rangeSigma, kernelRadius);
    //memcpy( distanceFloatData, this->m_OutputIplImage->imageData, distanceImageSize );
  }
  memcpy( outputDistanceFloatData, this->m_IplDistanceImage->imageData, this->m_ImageSize );
}

void mitk::ToFCompositeFilter::CreateOutputsForAllInputs()
{
  this->SetNumberOfOutputs(this->GetNumberOfInputs());  // create outputs for all inputs
  for (unsigned int idx = 0; idx < this->GetNumberOfOutputs(); ++idx)
    if (this->GetOutput(idx) == NULL)
    {
      DataObjectPointer newOutput = this->MakeOutput(idx);
      this->SetNthOutput(idx, newOutput);
    }
    this->Modified();
}

void mitk::ToFCompositeFilter::GenerateOutputInformation()
{
  mitk::Image::ConstPointer input = this->GetInput();
  mitk::Image::Pointer output = this->GetOutput();

  if (output->IsInitialized())
    return;

  itkDebugMacro(<<"GenerateOutputInformation()");

  output->Initialize(input->GetPixelType(), *input->GetTimeSlicedGeometry());
  output->SetPropertyList(input->GetPropertyList()->Clone());    
}

void mitk::ToFCompositeFilter::ProcessThresholdFilter(IplImage* inputIplImage, int min, int max)
{
  for(int i=0; i<this->m_ImageWidth*this->m_ImageHeight; i++)
  {
    float *f = (float*)inputIplImage->imageData;
    if (f[i]<=min)
    {
      f[i] = 0.0;
    }
    else if (f[i]>=max) 
    {
      f[i] = 0.0;
    }
  }
}

ItkImageType2D::Pointer mitk::ToFCompositeFilter::ProcessItkBilateralFilter(ItkImageType2D::Pointer inputItkImage, int domainSigma, int rangeSigma, int kernelRadius) 
{
  ItkImageType2D::Pointer outputItkImage;
  BilateralFilterType::Pointer bilateralFilter = BilateralFilterType::New();
  bilateralFilter->SetInput(inputItkImage);
  bilateralFilter->SetDomainSigma(domainSigma);
  bilateralFilter->SetRangeSigma(rangeSigma);
  //bilateralFilter->SetRadius(kernelRadius);
  outputItkImage = bilateralFilter->GetOutput();
  outputItkImage->Update();
  return outputItkImage;
}

void mitk::ToFCompositeFilter::ProcessCVBilateralFilter(IplImage* inputIplImage, IplImage* outputIplImage, int domainSigma, int rangeSigma, int kernelRadius) 
{
  int diameter = kernelRadius;
  double sigmaColor = rangeSigma;
  double sigmaSpace = domainSigma;
  cvSmooth(inputIplImage, outputIplImage, CV_BILATERAL, diameter, 0, sigmaColor, sigmaSpace);
}

void mitk::ToFCompositeFilter::ProcessCVMedianFilter(IplImage* inputIplImage, IplImage* outputIplImage, int radius) 
{
  cvSmooth(inputIplImage, outputIplImage, CV_MEDIAN, radius, 0, 0, 0);
}

void mitk::ToFCompositeFilter::ProcessStreamedQuickSelectMedianImageFilter(IplImage* inputIplImage, int numOfImages) 
{
  float* data = (float*)inputIplImage->imageData;

  int imageSize = inputIplImage->width * inputIplImage->height;
  float* tmpArray;

  if (numOfImages == 0)
  {
    return;
  }

  if (numOfImages != this->m_DataBufferMaxSize) // reset
  {
    //delete current buffer
    for( int i=0; i<this->m_DataBufferMaxSize; i++ ) {
      delete[] this->m_DataBuffer[i];
    }
    if (this->m_DataBuffer != NULL)
    {
      delete[] this->m_DataBuffer;
    }

    this->m_DataBufferMaxSize = numOfImages;

    // create new buffer with current size
    this->m_DataBuffer = new float*[this->m_DataBufferMaxSize];
    for(int i=0; i<this->m_DataBufferMaxSize; i++)
    {
      this->m_DataBuffer[i] = NULL;
    }
    this->m_DataBufferCurrentIndex = 0;
  }

  int currentBufferSize = this->m_DataBufferMaxSize;
  tmpArray = new float[this->m_DataBufferMaxSize];

  // copy data to buffer
  if (this->m_DataBuffer[this->m_DataBufferCurrentIndex] == NULL)
  {
    this->m_DataBuffer[this->m_DataBufferCurrentIndex] = new float[imageSize];
    currentBufferSize = this->m_DataBufferCurrentIndex + 1;
  }

  for(int j=0; j<imageSize; j++) 
  {
    this->m_DataBuffer[this->m_DataBufferCurrentIndex][j] = data[j];
  }

  for(int i=0; i<imageSize; i++) 
  {
    for(int j=0; j<currentBufferSize; j++)
    {
      tmpArray[j] = this->m_DataBuffer[j][i];
    }
    data[i] = quick_select(tmpArray, currentBufferSize);
  }

  this->m_DataBufferCurrentIndex = (this->m_DataBufferCurrentIndex + 1) % this->m_DataBufferMaxSize;

  delete[] tmpArray;
}

#define ELEM_SWAP(a,b) { register float t=(a);(a)=(b);(b)=t; }
float mitk::ToFCompositeFilter::quick_select(float arr[], int n)
{
  int low = 0;
  int high = n-1;
  int median = (low + high)/2;
  int middle = 0;
  int ll = 0;
  int hh = 0;
  for (;;) {
    if (high <= low) /* One element only */
      return arr[median] ;
    if (high == low + 1) { /* Two elements only */
      if (arr[low] > arr[high])
        ELEM_SWAP(arr[low], arr[high]) ;
      return arr[median] ;
    }
    /* Find median of low, middle and high items; swap into position low */
    middle = (low + high) / 2;
    if (arr[middle] > arr[high]) ELEM_SWAP(arr[middle], arr[high]) ;
    if (arr[low] > arr[high]) ELEM_SWAP(arr[low], arr[high]) ;
    if (arr[middle] > arr[low]) ELEM_SWAP(arr[middle], arr[low]) ;
    /* Swap low item (now in position middle) into position (low+1) */
    ELEM_SWAP(arr[middle], arr[low+1]) ;
    /* Nibble from each end towards middle, swapping items when stuck */
    ll = low + 1;
    hh = high;
    for (;;) {
      do ll++; while (arr[low] > arr[ll]) ;
      do hh--; while (arr[hh] > arr[low]) ;
      if (hh < ll)
        break;
      ELEM_SWAP(arr[ll], arr[hh]) ;
    }
    /* Swap middle item (in position low) back into correct position */
    ELEM_SWAP(arr[low], arr[hh]) ;
    /* Re-set active partition */
    if (hh <= median)
      low = ll;
    if (hh >= median)
      high = hh - 1;
  }
}
#undef ELEM_SWAP

bool mitk::ToFCompositeFilter::GetApplyTemporalMedianFilter()
{
  return this->m_ApplyTemporalMedianFilter;
}

bool mitk::ToFCompositeFilter::GetApplyMedianFilter()
{
  return this->m_ApplyMedianFilter;
}

bool mitk::ToFCompositeFilter::GetApplyThresholdFilter()
{
  return this->m_ApplyThresholdFilter;
}

bool mitk::ToFCompositeFilter::GetApplyBilateralFilter()
{
  return this->m_ApplyBilateralFilter;
}

void mitk::ToFCompositeFilter::SetApplyTemporalMedianFilter(bool flag)
{
  this->m_ApplyTemporalMedianFilter = flag;
}

void mitk::ToFCompositeFilter::SetApplyMedianFilter(bool flag)
{
  this->m_ApplyMedianFilter = flag;
}

void mitk::ToFCompositeFilter::SetApplyThresholdFilter(bool flag)
{
  this->m_ApplyThresholdFilter = flag;
}

void mitk::ToFCompositeFilter::SetApplyBilateralFilter(bool flag)
{
  this->m_ApplyBilateralFilter = flag;
}

void mitk::ToFCompositeFilter::SetTemporalMedianFilterParameter(int tmporalMedianFilterNumOfFrames)
{
  this->m_TemporalMedianFilterNumOfFrames = tmporalMedianFilterNumOfFrames;
}

void mitk::ToFCompositeFilter::SetThresholdFilterParameter(int min, int max)
{
  if (min > max)
  {
    min = max;
  }
  this->m_ThresholdFilterMin = min;
  this->m_ThresholdFilterMax = max;
}

void mitk::ToFCompositeFilter::SetBilateralFilterParameter(int domainSigma, int rangeSigma, int kernelRadius = 0)
{
  this->m_BilateralFilterDomainSigma = domainSigma;
  this->m_BilateralFilterRangeSigma = rangeSigma;
  this->m_BilateralFilterKernelRadius = kernelRadius;
}

void mitk::ToFCompositeFilter::CreateItkImage(ItkImageType2D::Pointer &itkInputImage)
{
  itkInputImage = ItkImageType2D::New();
  ItkImageType2D::IndexType startIndex;
  startIndex[0] =   0;  // first index on X
  startIndex[1] =   0;  // first index on Y
  ItkImageType2D::SizeType  size;
  size[0]  = this->m_ImageWidth;  // size along X
  size[1]  = this->m_ImageHeight;  // size along Y
  ItkImageType2D::RegionType region;
  region.SetSize( size );
  region.SetIndex( startIndex );
  itkInputImage->SetRegions( region );
  itkInputImage->Allocate();

}
