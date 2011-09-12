/*=========================================================================

 Program:   Medical Imaging & Interaction Toolkit
 Language:  C++
 Date:      $Date: 2010-03-31 16:40:27 +0200 (Mi, 31 Mrz 2010) $
 Version:   $Revision: 21975 $

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/


// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>



// Qmitk
#include "QmitkFiberBundleOperationsView.h"
#include "QmitkStdMultiWidget.h"

// Qt
#include <QMessageBox>

//MITK
#include "mitkNodePredicateProperty.h"
//#include "mitkNodePredicateAND.h"
#include "mitkImageCast.h"

#include "mitkPointSet.h"


#include "mitkPlanarCircle.h"
#include "mitkPlanarPolygon.h"
#include <mitkPlanarRectangle.h>
#include "mitkPlanarFigureInteractor.h"
#include "mitkGlobalInteraction.h"
#include <mitkImageAccessByItk.h>
#include <itkResampleImageFilter.h>
#include <itkGaussianInterpolateImageFunction.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkTractsToDWIImageFilter.h>
#include <itkTractsToFiberEndingsImageFilter.h>
#include <itkTractsToProbabilityImageFilter.h>
#include <mitkDiffusionImage.h>


const std::string QmitkFiberBundleOperationsView::VIEW_ID = "org.mitk.views.fiberBundleOperations";
const std::string id_DataManager = "org.mitk.views.datamanager";
using namespace berry;
using namespace mitk;



QmitkFiberBundleOperationsView::QmitkFiberBundleOperationsView()
: QmitkFunctionality()
, m_Controls( 0 )
, m_MultiWidget( NULL )
, m_EllipseCounter(0)
, m_PolygonCounter(0)
//, m_SelectedFBNodes( NULL )
//, m_SelectedPFNodes(0)
, m_UpsamplingFactor(5)
{

}

// Destructor
QmitkFiberBundleOperationsView::~QmitkFiberBundleOperationsView()
{

}


void QmitkFiberBundleOperationsView::CreateQtPartControl( QWidget *parent )
{
  // build up qt view, unless already done
  if ( !m_Controls )
  {
    // create GUI widgets from the Qt Designer's .ui file
    m_Controls = new Ui::QmitkFiberBundleOperationsViewControls;
    m_Controls->setupUi( parent );
    m_Controls->doExtractFibersButton->setDisabled(true);
    m_Controls->PFCompoANDButton->setDisabled(true);
    m_Controls->PFCompoORButton->setDisabled(true);
    m_Controls->PFCompoNOTButton->setDisabled(true);
    m_Controls->PFCompoDELButton->setDisabled(true);

    connect( m_Controls->doExtractFibersButton, SIGNAL(clicked()), this, SLOT(DoFiberExtraction()) );
    //connect( m_Controls->comboBox_fiberAlgo, SIGNAL(selected()), this, SLOT(handleAlgoSelection() );
    connect( m_Controls->m_CircleButton, SIGNAL( clicked() ), this, SLOT( ActionDrawEllipseTriggered() ) );
    connect( m_Controls->m_PolygonButton, SIGNAL( clicked() ), this, SLOT( ActionDrawPolygonTriggered() ) );
    connect(m_Controls->PFCompoANDButton, SIGNAL(clicked()), this, SLOT(generatePFCompo_AND()) );
    connect(m_Controls->PFCompoORButton, SIGNAL(clicked()), this, SLOT(generatePFCompo_OR()) );
    connect(m_Controls->PFCompoNOTButton, SIGNAL(clicked()), this, SLOT(generatePFCompo_NOT()) );
    connect(m_Controls->PFCompoDELButton, SIGNAL(clicked()), this, SLOT(deletePFCompo()) );

    connect(m_Controls->m_JoinBundles, SIGNAL(clicked()), this, SLOT(JoinBundles()) );
    connect(m_Controls->m_SubstractBundles, SIGNAL(clicked()), this, SLOT(SubstractBundles()) );
    connect(m_Controls->m_GenerateROIImage, SIGNAL(clicked()), this, SLOT(GenerateROIImage()) );

    connect( m_Controls->m_GenerationStartButton, SIGNAL(clicked()), this, SLOT(GenerationStart()) );
  }
}

void QmitkFiberBundleOperationsView::GenerateROIImage(){

  if (m_Image.IsNull() || m_SelectedPF.empty())
    return;

  mitk::Image* image = const_cast<mitk::Image*>(m_Image.GetPointer());

  MaskImage3DType::Pointer temp = MaskImage3DType::New();
  mitk::CastToItkImage<MaskImage3DType>(m_Image, temp);

  m_PlanarFigureImage = MaskImage3DType::New();
  m_PlanarFigureImage->SetSpacing( temp->GetSpacing() );   // Set the image spacing
  m_PlanarFigureImage->SetOrigin( temp->GetOrigin() );     // Set the image origin
  m_PlanarFigureImage->SetDirection( temp->GetDirection() );  // Set the image direction
  m_PlanarFigureImage->SetRegions( temp->GetLargestPossibleRegion() );
  m_PlanarFigureImage->Allocate();
  m_PlanarFigureImage->FillBuffer( 0 );


  for (int i=0; i<m_SelectedPF.size(); i++)
    CompositeExtraction(dynamic_cast<mitk::PlanarFigure*>(m_SelectedPF.at(i)->GetData()), image);

  DataNode::Pointer node = DataNode::New();
  Image::Pointer tmpImage = Image::New();
  tmpImage->InitializeByItk(m_PlanarFigureImage.GetPointer());
  tmpImage->SetVolume(m_PlanarFigureImage->GetBufferPointer());
  node->SetData(tmpImage);
  node->SetName("planarFigureImage");
  this->GetDefaultDataStorage()->Add(node);
}

void QmitkFiberBundleOperationsView::CompositeExtraction(mitk::PlanarFigure::Pointer planarFigure, mitk::Image* image)
{
  if (dynamic_cast<mitk::PlanarFigureComposite*>(planarFigure.GetPointer())){
    mitk::PlanarFigureComposite::Pointer pfcomp = dynamic_cast<mitk::PlanarFigureComposite*>(planarFigure.GetPointer());

    for (int i=0; i<pfcomp->getNumberOfChildren(); ++i)
    {
      CompositeExtraction(pfcomp->getChildAt(i), image);
    }
  }
  else
  {
    m_PlanarFigure = planarFigure;
    AccessFixedDimensionByItk_3(
        image,
        InternalReorientImagePlane, 3,
        image->GetGeometry(),
        m_PlanarFigure->GetGeometry(), -1);

    AccessFixedDimensionByItk_1(
        m_InternalImage,
        InternalCalculateMaskFromPlanarFigure,
        3, 2 );
  }
}

template < typename TPixel, unsigned int VImageDimension >
    void QmitkFiberBundleOperationsView::InternalReorientImagePlane(
        const itk::Image< TPixel, VImageDimension > *image,
        mitk::Geometry3D* imggeo, mitk::Geometry3D* planegeo3D, int additionalIndex )
{

  MITK_INFO << "InternalReorientImagePlane() start";

  typedef itk::Image< TPixel, VImageDimension > ImageType;
  typedef itk::Image< float, VImageDimension > FloatImageType;

  typedef itk::ResampleImageFilter<ImageType, FloatImageType, double> ResamplerType;
  typename ResamplerType::Pointer resampler = ResamplerType::New();

  mitk::PlaneGeometry* planegeo = dynamic_cast<mitk::PlaneGeometry*>(planegeo3D);

  float upsamp = m_UpsamplingFactor;
  float gausssigma = 0.5;

  // Spacing
  typename ResamplerType::SpacingType spacing = planegeo->GetSpacing();
  spacing[0] = image->GetSpacing()[0] / upsamp;
  spacing[1] = image->GetSpacing()[1] / upsamp;
  spacing[2] = image->GetSpacing()[2];
  resampler->SetOutputSpacing( spacing );

  // Size
  typename ResamplerType::SizeType size;
  size[0] = planegeo->GetParametricExtentInMM(0) / spacing[0];
  size[1] = planegeo->GetParametricExtentInMM(1) / spacing[1];
  size[2] = 1;
  resampler->SetSize( size );

  // Origin
  typename mitk::Point3D orig = planegeo->GetOrigin();
  typename mitk::Point3D corrorig;
  planegeo3D->WorldToIndex(orig,corrorig);
  corrorig[0] += 0.5/upsamp;
  corrorig[1] += 0.5/upsamp;
  corrorig[2] += 0;
  planegeo3D->IndexToWorld(corrorig,corrorig);
  resampler->SetOutputOrigin(corrorig );

  // Direction
  typename ResamplerType::DirectionType direction;
  typename mitk::AffineTransform3D::MatrixType matrix = planegeo->GetIndexToWorldTransform()->GetMatrix();
  for(int c=0; c<matrix.ColumnDimensions; c++)
  {
    double sum = 0;
    for(int r=0; r<matrix.RowDimensions; r++)
    {
      sum += matrix(r,c)*matrix(r,c);
    }
    for(int r=0; r<matrix.RowDimensions; r++)
    {
      direction(r,c) = matrix(r,c)/sqrt(sum);
    }
  }
  resampler->SetOutputDirection( direction );

  // Gaussian interpolation
  if(gausssigma != 0)
  {
    double sigma[3];
    for( unsigned int d = 0; d < 3; d++ )
    {
      sigma[d] = gausssigma * image->GetSpacing()[d];
    }
    double alpha = 2.0;

    typedef itk::GaussianInterpolateImageFunction<ImageType, double>
        GaussianInterpolatorType;

    typename GaussianInterpolatorType::Pointer interpolator
        = GaussianInterpolatorType::New();

    interpolator->SetInputImage( image );
    interpolator->SetParameters( sigma, alpha );

    resampler->SetInterpolator( interpolator );
  }
  else
  {
    //      typedef typename itk::BSplineInterpolateImageFunction<ImageType, double>
    //          InterpolatorType;
    typedef typename itk::LinearInterpolateImageFunction<ImageType, double> InterpolatorType;

    typename InterpolatorType::Pointer interpolator
        = InterpolatorType::New();

    interpolator->SetInputImage( image );

    resampler->SetInterpolator( interpolator );

  }

  // Other resampling options
  resampler->SetInput( image );
  resampler->SetDefaultPixelValue(0);

  MITK_INFO << "Resampling requested image plane ... ";
  resampler->Update();
  MITK_INFO << " ... done";

  if(additionalIndex < 0)
  {
    this->m_InternalImage = mitk::Image::New();
    this->m_InternalImage->InitializeByItk( resampler->GetOutput() );
    this->m_InternalImage->SetVolume( resampler->GetOutput()->GetBufferPointer() );
  }
}

template < typename TPixel, unsigned int VImageDimension >
    void QmitkFiberBundleOperationsView::InternalCalculateMaskFromPlanarFigure(
        itk::Image< TPixel, VImageDimension > *image, unsigned int axis )
{

  MITK_INFO << "InternalCalculateMaskFromPlanarFigure() start";

  typedef itk::Image< TPixel, VImageDimension > ImageType;
  typedef itk::CastImageFilter< ImageType, MaskImage3DType > CastFilterType;

  // Generate mask image as new image with same header as input image and
  // initialize with "1".
  MaskImage3DType::Pointer newMaskImage = MaskImage3DType::New();
  newMaskImage->SetSpacing( image->GetSpacing() );   // Set the image spacing
  newMaskImage->SetOrigin( image->GetOrigin() );     // Set the image origin
  newMaskImage->SetDirection( image->GetDirection() );  // Set the image direction
  newMaskImage->SetRegions( image->GetLargestPossibleRegion() );
  newMaskImage->Allocate();
  newMaskImage->FillBuffer( 1 );

  // Generate VTK polygon from (closed) PlanarFigure polyline
  // (The polyline points are shifted by -0.5 in z-direction to make sure
  // that the extrusion filter, which afterwards elevates all points by +0.5
  // in z-direction, creates a 3D object which is cut by the the plane z=0)
  const Geometry2D *planarFigureGeometry2D = m_PlanarFigure->GetGeometry2D();
  const PlanarFigure::PolyLineType planarFigurePolyline = m_PlanarFigure->GetPolyLine( 0 );
  const Geometry3D *imageGeometry3D = m_InternalImage->GetGeometry( 0 );

  vtkPolyData *polyline = vtkPolyData::New();
  polyline->Allocate( 1, 1 );

  // Determine x- and y-dimensions depending on principal axis
  int i0, i1;
  switch ( axis )
  {
  case 0:
    i0 = 1;
    i1 = 2;
    break;

  case 1:
    i0 = 0;
    i1 = 2;
    break;

  case 2:
  default:
    i0 = 0;
    i1 = 1;
    break;
  }

  // Create VTK polydata object of polyline contour
  vtkPoints *points = vtkPoints::New();
  PlanarFigure::PolyLineType::const_iterator it;
  std::vector<vtkIdType> indices;

  unsigned int numberOfPoints = 0;

  for ( it = planarFigurePolyline.begin();
  it != planarFigurePolyline.end();
  ++it )
  {
    Point3D point3D;

    // Convert 2D point back to the local index coordinates of the selected
    // image
    Point2D point2D = it->Point;
    planarFigureGeometry2D->WorldToIndex(point2D, point2D);
    point2D[0] -= 0.5/m_UpsamplingFactor;
    point2D[1] -= 0.5/m_UpsamplingFactor;
    planarFigureGeometry2D->IndexToWorld(point2D, point2D);
    planarFigureGeometry2D->Map( point2D, point3D );

    // Polygons (partially) outside of the image bounds can not be processed
    // further due to a bug in vtkPolyDataToImageStencil
    if ( !imageGeometry3D->IsInside( point3D ) )
    {
      float bounds[2] = {0,0};
      bounds[0] =
          this->m_InternalImage->GetLargestPossibleRegion().GetSize().GetElement(i0);
      bounds[1] =
          this->m_InternalImage->GetLargestPossibleRegion().GetSize().GetElement(i1);

      imageGeometry3D->WorldToIndex( point3D, point3D );

      if (point3D[i0]<0)
        point3D[i0] = 0.5;
      else if (point3D[i0]>bounds[0])
        point3D[i0] = bounds[0]-0.5;

      if (point3D[i1]<0)
        point3D[i1] = 0.5;
      else if (point3D[i1]>bounds[1])
        point3D[i1] = bounds[1]-0.5;

      points->InsertNextPoint( point3D[i0], point3D[i1], -0.5 );
      numberOfPoints++;
    }
    else
    {
      imageGeometry3D->WorldToIndex( point3D, point3D );
      point3D[i0] += 0.5;
      point3D[i1] += 0.5;

      // Add point to polyline array
      points->InsertNextPoint( point3D[i0], point3D[i1], -0.5 );
      numberOfPoints++;
    }
  }
  polyline->SetPoints( points );
  points->Delete();

  vtkIdType *ptIds = new vtkIdType[numberOfPoints];
  for ( vtkIdType i = 0; i < numberOfPoints; ++i )
  {
    ptIds[i] = i;
  }
  polyline->InsertNextCell( VTK_POLY_LINE, numberOfPoints, ptIds );


  // Extrude the generated contour polygon
  vtkLinearExtrusionFilter *extrudeFilter = vtkLinearExtrusionFilter::New();
  extrudeFilter->SetInput( polyline );
  extrudeFilter->SetScaleFactor( 1 );
  extrudeFilter->SetExtrusionTypeToNormalExtrusion();
  extrudeFilter->SetVector( 0.0, 0.0, 1.0 );

  // Make a stencil from the extruded polygon
  vtkPolyDataToImageStencil *polyDataToImageStencil = vtkPolyDataToImageStencil::New();
  polyDataToImageStencil->SetInput( extrudeFilter->GetOutput() );



  // Export from ITK to VTK (to use a VTK filter)
  typedef itk::VTKImageImport< MaskImage3DType > ImageImportType;
  typedef itk::VTKImageExport< MaskImage3DType > ImageExportType;

  typename ImageExportType::Pointer itkExporter = ImageExportType::New();
  itkExporter->SetInput( newMaskImage );

  vtkImageImport *vtkImporter = vtkImageImport::New();
  this->ConnectPipelines( itkExporter, vtkImporter );
  vtkImporter->Update();


  // Apply the generated image stencil to the input image
  vtkImageStencil *imageStencilFilter = vtkImageStencil::New();
  imageStencilFilter->SetInputConnection( vtkImporter->GetOutputPort() );
  imageStencilFilter->SetStencil( polyDataToImageStencil->GetOutput() );
  imageStencilFilter->ReverseStencilOff();
  imageStencilFilter->SetBackgroundValue( 0 );
  imageStencilFilter->Update();


  // Export from VTK back to ITK
  vtkImageExport *vtkExporter = vtkImageExport::New();
  vtkExporter->SetInputConnection( imageStencilFilter->GetOutputPort() );
  vtkExporter->Update();

  typename ImageImportType::Pointer itkImporter = ImageImportType::New();
  this->ConnectPipelines( vtkExporter, itkImporter );
  itkImporter->Update();

  // calculate cropping bounding box
  m_InternalImageMask3D = itkImporter->GetOutput();
  m_InternalImageMask3D->SetDirection(image->GetDirection());

  itk::ImageRegionConstIterator<MaskImage3DType>
      itmask(m_InternalImageMask3D, m_InternalImageMask3D->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType>
      itimage(image, image->GetLargestPossibleRegion());

  itmask = itmask.Begin();
  itimage = itimage.Begin();

  typename ImageType::SizeType lowersize = {{9999999999,9999999999,9999999999}};
  typename ImageType::SizeType uppersize = {{0,0,0}};
  while( !itmask.IsAtEnd() )
  {
    if(itmask.Get() == 0)
    {
      itimage.Set(0);
    }
    else
    {
      typename ImageType::IndexType index = itimage.GetIndex();
      typename ImageType::SizeType signedindex;
      signedindex[0] = index[0];
      signedindex[1] = index[1];
      signedindex[2] = index[2];

      lowersize[0] = signedindex[0] < lowersize[0] ? signedindex[0] : lowersize[0];
      lowersize[1] = signedindex[1] < lowersize[1] ? signedindex[1] : lowersize[1];
      lowersize[2] = signedindex[2] < lowersize[2] ? signedindex[2] : lowersize[2];

      uppersize[0] = signedindex[0] > uppersize[0] ? signedindex[0] : uppersize[0];
      uppersize[1] = signedindex[1] > uppersize[1] ? signedindex[1] : uppersize[1];
      uppersize[2] = signedindex[2] > uppersize[2] ? signedindex[2] : uppersize[2];
    }

    ++itmask;
    ++itimage;
  }

  typename ImageType::IndexType index;
  index[0] = lowersize[0];
  index[1] = lowersize[1];
  index[2] = lowersize[2];

  typename ImageType::SizeType size;
  size[0] = uppersize[0] - lowersize[0] + 1;
  size[1] = uppersize[1] - lowersize[1] + 1;
  size[2] = uppersize[2] - lowersize[2] + 1;

  itk::ImageRegion<3> cropRegion = itk::ImageRegion<3>(index, size);

//  // crop internal image
//  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > ROIFilterType;
//  typename ROIFilterType::Pointer roi = ROIFilterType::New();
//  roi->SetRegionOfInterest(cropRegion);
//  roi->SetInput(image);
//  roi->Update();

//  m_InternalImage = mitk::Image::New();
//  m_InternalImage->InitializeByItk(roi->GetOutput());
//  m_InternalImage->SetVolume(roi->GetOutput()->GetBufferPointer());

  // crop internal mask
  typedef itk::RegionOfInterestImageFilter< MaskImage3DType, MaskImage3DType > ROIMaskFilterType;
  typename ROIMaskFilterType::Pointer roi2 = ROIMaskFilterType::New();
  roi2->SetRegionOfInterest(cropRegion);
  roi2->SetInput(m_InternalImageMask3D);
  roi2->Update();
  m_InternalImageMask3D = roi2->GetOutput();

  DataNode::Pointer node = DataNode::New();
  Image::Pointer tmpImage = Image::New();
  tmpImage->InitializeByItk(m_InternalImageMask3D.GetPointer());
  tmpImage->SetVolume(m_InternalImageMask3D->GetBufferPointer());
  node->SetData(tmpImage);
  this->GetDefaultDataStorage()->Add(node);

  Image::Pointer tmpImage2 = Image::New();
  tmpImage2->InitializeByItk(m_PlanarFigureImage.GetPointer());
  const Geometry3D *pfImageGeometry3D = tmpImage2->GetGeometry( 0 );

  const Geometry3D *intImageGeometry3D = tmpImage->GetGeometry( 0 );

  typedef itk::ImageRegionIteratorWithIndex<MaskImage3DType> IteratorType;
  IteratorType imageIterator (m_InternalImageMask3D, m_InternalImageMask3D->GetRequestedRegion());
  imageIterator.GoToBegin();
  while ( !imageIterator.IsAtEnd() )
  {
    unsigned char val = imageIterator.Value();
    if (val>0)
    {
      itk::Index<3> index = imageIterator.GetIndex();
      Point3D point;
      point[0] = index[0];
      point[1] = index[1];
      point[2] = index[2];

      intImageGeometry3D->IndexToWorld(point, point);
      pfImageGeometry3D->WorldToIndex(point, point);

      index[0] = point[0];
      index[1] = point[1];
      index[2] = point[2];

      m_PlanarFigureImage->SetPixel(index, 1);
    }
    ++imageIterator;
  }

  // Clean up VTK objects
  polyline->Delete();
  extrudeFilter->Delete();
  polyDataToImageStencil->Delete();
  vtkImporter->Delete();
  imageStencilFilter->Delete();
  //vtkExporter->Delete(); // TODO: crashes when outcommented; memory leak??
  delete[] ptIds;

}

void QmitkFiberBundleOperationsView::StdMultiWidgetAvailable (QmitkStdMultiWidget &stdMultiWidget)
{
  m_MultiWidget = &stdMultiWidget;
}


void QmitkFiberBundleOperationsView::StdMultiWidgetNotAvailable()
{
  m_MultiWidget = NULL;
}

/* OnSelectionChanged is registered to SelectionService, therefore no need to
 implement SelectionService Listener explicitly */

void QmitkFiberBundleOperationsView::OnSelectionChanged( std::vector<mitk::DataNode*> nodes )
{

  if ( !this->IsVisible() )
  {
    // do nothing if nobody wants to see me :-(
    return;
  }


  //reset existing Vectors containing FiberBundles and PlanarFigures from a previous selection
  m_SelectedFB.clear();
  m_SelectedPF.clear();
  m_Image = NULL;


  //differ between 2 scenarios...
  // 1) add PF to an existing Spaghetti
  // 2) BOOLEAN OPERATORS HANDLING ... selection of multiple PF, and or multiple Spaghetti


  //scenario 1

  for( std::vector<mitk::DataNode*>::iterator it = nodes.begin();
      it != nodes.end(); ++it )
  {

    mitk::DataNode::Pointer node = *it;
    if ( dynamic_cast<mitk::FiberBundle*>(node->GetData()) )
    {
      //  MITK_INFO << "onselectionchg(): " << node->GetData()->GetNameOfClass();
      m_SelectedFB.push_back(node);
      mitk::FiberBundle::Pointer bundle = dynamic_cast<mitk::FiberBundle*>(node->GetData());
      QString numFibers;
      this->m_Controls->m_NumFibersLabel->setText(numFibers.setNum(bundle->GetNumTracts()));

    } else if (dynamic_cast<mitk::PlanarFigure*>(node->GetData())){

      //   MITK_INFO << "onselectionchg(): " << node->GetData()->GetNameOfClass();
      m_SelectedPF.push_back(node);

    } else if (dynamic_cast<mitk::Image*>(node->GetData())){
      m_Image = dynamic_cast<mitk::Image*>(node->GetData());
    }

  }

  if (m_SelectedPF.size() >= 1 && m_Image.IsNotNull())
    m_Controls->m_GenerateROIImage->setEnabled(true);
  else
    m_Controls->m_GenerateROIImage->setEnabled(false);

  //quick and dirty control structure for extraction ATM
  if (m_SelectedPF.size() == 1) {


    //#############################################
    //### PLANAR FIGURE COMPOSIT SelectionLOGIC ###
    //#############################################

    m_Controls->PFCompoANDButton->setDisabled(true);
    m_Controls->PFCompoORButton->setDisabled(true);
    m_Controls->PFCompoNOTButton->setEnabled(true);
    m_Controls->PFCompoDELButton->setDisabled(true);

    //if planarFigureComposite selected, activate DEL button
    if ( dynamic_cast<mitk::PlanarFigureComposite*>(m_SelectedPF.at(0)->GetData()) )
    {
      m_Controls->PFCompoDELButton->setEnabled(true);
    }


  } else if (m_SelectedPF.size() > 1) {

    //#############################################
    //### PLANAR FIGURE COMPOSIT SelectionLOGIC ###
    //#############################################
    // if 2 PlanarFigure objects (PFCircle, PFPoly, ..., PFComposite) selected,
    //then activate AND OR NOT PF Composite Buttons

    m_Controls->PFCompoANDButton->setEnabled(true);
    m_Controls->PFCompoORButton->setEnabled(true);
    m_Controls->PFCompoNOTButton->setDisabled(true);
    m_Controls->PFCompoDELButton->setDisabled(true);
  }



  if (m_SelectedFB.size() == 1 && m_SelectedPF.size() == 1)
  {

    //#############################################
    //##### EXTRACT FIBERBUNDLE SelectionLOGIC ####
    //#############################################

    m_Controls->doExtractFibersButton->setEnabled(true);



  } else if (nodes.size() > 1 ) {
    //scenario 2


  } else if (nodes.empty()) {

    //#############################################
    //##### EXTRACT FIBERBUNDLE SelectionLOGIC ####
    //#############################################

    m_Controls->doExtractFibersButton->setDisabled(true);


    //#############################################
    //### PLANAR FIGURE COMPOSIT SelectionLOGIC ###
    //#############################################

    m_Controls->PFCompoANDButton->setDisabled(true);
    m_Controls->PFCompoORButton->setDisabled(true);
    m_Controls->PFCompoNOTButton->setDisabled(true);

  }

  if (m_SelectedFB.size() == 2)
  {

    //#############################################
    //##### JOIN/SUBSTRACT FIBERBUNDLES        ####
    //#############################################

    m_Controls->m_JoinBundles->setEnabled(true);
    m_Controls->m_SubstractBundles->setEnabled(true);

  }
  else
  {

    m_Controls->m_JoinBundles->setEnabled(false);
    m_Controls->m_SubstractBundles->setEnabled(false);

  }

}


void QmitkFiberBundleOperationsView::ActionDrawPolygonTriggered()
{
  //  bool checked = m_Controls->m_PolygonButton->isChecked();
  //  if(!this->AssertDrawingIsPossible(checked))
  //    return;

  mitk::PlanarPolygon::Pointer figure = mitk::PlanarPolygon::New();
  figure->ClosedOn();
  this->AddFigureToDataStorage(figure, QString("Polygon%1").arg(++m_PolygonCounter));

  MITK_INFO << "PlanarPolygon created ...";

  mitk::DataStorage::SetOfObjects::ConstPointer _NodeSet = this->GetDefaultDataStorage()->GetAll();
  mitk::DataNode* node = 0;
  mitk::PlanarFigureInteractor::Pointer figureInteractor = 0;
  mitk::PlanarFigure* figureP = 0;

  for(mitk::DataStorage::SetOfObjects::ConstIterator it=_NodeSet->Begin(); it!=_NodeSet->End()
      ; it++)
  {
    node = const_cast<mitk::DataNode*>(it->Value().GetPointer());
    figureP = dynamic_cast<mitk::PlanarFigure*>(node->GetData());

    if(figureP)
    {
      figureInteractor = dynamic_cast<mitk::PlanarFigureInteractor*>(node->GetInteractor());

      if(figureInteractor.IsNull())
        figureInteractor = mitk::PlanarFigureInteractor::New("PlanarFigureInteractor", node);

      mitk::GlobalInteraction::GetInstance()->AddInteractor(figureInteractor);
    }
  }

}


void QmitkFiberBundleOperationsView::ActionDrawEllipseTriggered()
{
  //bool checked = m_Controls->m_CircleButton->isChecked();
  //if(!this->AssertDrawingIsPossible(checked))
  //  return;

  mitk::PlanarCircle::Pointer figure = mitk::PlanarCircle::New();
  this->AddFigureToDataStorage(figure, QString("Circle%1").arg(++m_EllipseCounter));

  this->GetDataStorage()->Modified();
  MITK_INFO << "PlanarCircle created ...";

  //call

  mitk::DataStorage::SetOfObjects::ConstPointer _NodeSet = this->GetDefaultDataStorage()->GetAll();
  mitk::DataNode* node = 0;
  mitk::PlanarFigureInteractor::Pointer figureInteractor = 0;
  mitk::PlanarFigure* figureP = 0;

  for(mitk::DataStorage::SetOfObjects::ConstIterator it=_NodeSet->Begin(); it!=_NodeSet->End()
      ; it++)
  {
    node = const_cast<mitk::DataNode*>(it->Value().GetPointer());
    figureP = dynamic_cast<mitk::PlanarFigure*>(node->GetData());

    if(figureP)
    {
      figureInteractor = dynamic_cast<mitk::PlanarFigureInteractor*>(node->GetInteractor());

      if(figureInteractor.IsNull())
        figureInteractor = mitk::PlanarFigureInteractor::New("PlanarFigureInteractor", node);

      mitk::GlobalInteraction::GetInstance()->AddInteractor(figureInteractor);
    }
  }




}

void QmitkFiberBundleOperationsView::Activated()
{

  MITK_INFO << "FB OPerations ACTIVATED()";
  /*

   mitk::DataStorage::SetOfObjects::ConstPointer _NodeSet = this->GetDefaultDataStorage()->GetAll();
   mitk::DataNode* node = 0;
   mitk::PlanarFigureInteractor::Pointer figureInteractor = 0;
   mitk::PlanarFigure* figure = 0;

   for(mitk::DataStorage::SetOfObjects::ConstIterator it=_NodeSet->Begin(); it!=_NodeSet->End()
   ; it++)
   {
   node = const_cast<mitk::DataNode*>(it->Value().GetPointer());
   figure = dynamic_cast<mitk::PlanarFigure*>(node->GetData());

   if(figure)
   {
   figureInteractor = dynamic_cast<mitk::PlanarFigureInteractor*>(node->GetInteractor());

   if(figureInteractor.IsNull())
   figureInteractor = mitk::PlanarFigureInteractor::New("PlanarFigureInteractor", node);

   mitk::GlobalInteraction::GetInstance()->AddInteractor(figureInteractor);
   }
   }

   */


}

void QmitkFiberBundleOperationsView::AddFigureToDataStorage(mitk::PlanarFigure* figure, const QString& name,
                                                            const char *propertyKey, mitk::BaseProperty *property )
{
  //set desired data to DataNode where Planarfigure is stored
  mitk::DataNode::Pointer newNode = mitk::DataNode::New();
  newNode->SetName(name.toStdString());
  newNode->SetData(figure);

  newNode->AddProperty( "planarfigure.default.line.color", mitk::ColorProperty::New(1.0,0.0,0.0));
  newNode->AddProperty( "planarfigure.line.width", mitk::FloatProperty::New(2.0));
  newNode->AddProperty( "planarfigure.drawshadow", mitk::BoolProperty::New(true));






  newNode->AddProperty( "selected", mitk::BoolProperty::New(true) );
  newNode->AddProperty( "planarfigure.ishovering", mitk::BoolProperty::New(true) );
  newNode->AddProperty( "planarfigure.drawoutline", mitk::BoolProperty::New(true) );
  newNode->AddProperty( "planarfigure.drawquantities", mitk::BoolProperty::New(false) );
  newNode->AddProperty( "planarfigure.drawshadow", mitk::BoolProperty::New(true) );

  newNode->AddProperty( "planarfigure.line.width", mitk::FloatProperty::New(3.0) );
  newNode->AddProperty( "planarfigure.shadow.widthmodifier", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.outline.width", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.helperline.width", mitk::FloatProperty::New(2.0) );

//  PlanarFigureControlPointStyleProperty::Pointer styleProperty =
//    dynamic_cast< PlanarFigureControlPointStyleProperty* >( node->GetProperty( "planarfigure.controlpointshape" ) );
//  if ( styleProperty.IsNotNull() )
//  {
//    m_ControlPointShape = styleProperty->GetShape();
//  }

  newNode->AddProperty( "planarfigure.default.line.color", mitk::ColorProperty::New(1.0,1.0,1.0) );
  newNode->AddProperty( "planarfigure.default.line.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.default.outline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.default.outline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.default.helperline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.default.helperline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.default.markerline.color", mitk::ColorProperty::New(0.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.default.markerline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.default.marker.color", mitk::ColorProperty::New(1.0,1.0,1.0)  );
  newNode->AddProperty( "planarfigure.default.marker.opacity",mitk::FloatProperty::New(2.0) );

  newNode->AddProperty( "planarfigure.hover.line.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.hover.line.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.hover.outline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.hover.outline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.hover.helperline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.hover.helperline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.hover.markerline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.hover.markerline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.hover.marker.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.hover.marker.opacity", mitk::FloatProperty::New(2.0) );

  newNode->AddProperty( "planarfigure.selected.line.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.selected.line.opacity",mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.selected.outline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.selected.outline.opacity", mitk::FloatProperty::New(2.0));
  newNode->AddProperty( "planarfigure.selected.helperline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.selected.helperline.opacity",mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.selected.markerline.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.selected.markerline.opacity", mitk::FloatProperty::New(2.0) );
  newNode->AddProperty( "planarfigure.selected.marker.color", mitk::ColorProperty::New(1.0,0.0,0.0)  );
  newNode->AddProperty( "planarfigure.selected.marker.opacity",mitk::FloatProperty::New(2.0));















  // Add custom property, if available
  //if ( (propertyKey != NULL) && (property != NULL) )
  //{
  //  newNode->AddProperty( propertyKey, property );
  //}

  //get current selected DataNode -which should be a FiberBundle- and set PlanarFigure as child

  //this->GetDataStorage()->GetNodes()



  // mitk::FiberBundle::Pointer selectedFBNode = m_SelectedFBNodes.at(0);

  // figure drawn on the topmost layer / image
  this->GetDataStorage()->Add(newNode );

  std::vector<mitk::DataNode*> selectedNodes = GetDataManagerSelection();
  for(unsigned int i = 0; i < selectedNodes.size(); i++)
  {
    selectedNodes[i]->SetSelected(false);
  }
  //selectedNodes = m_SelectedPlanarFigureNodes->GetNodes();
  /*for(unsigned int i = 0; i < selectedNodes.size(); i++)
   {
   selectedNodes[i]->SetSelected(false);
   }
   */
  newNode->SetSelected(true);

  //Select(newNode);
}

void QmitkFiberBundleOperationsView::DoFiberExtraction()
{
  mitk::FiberBundle::Pointer selFB = dynamic_cast<mitk::FiberBundle*>(m_SelectedFB.at(0)->GetData());
  mitk::PlanarFigure::Pointer selPF = dynamic_cast<mitk::PlanarFigure*> (m_SelectedPF.at(0)->GetData());


  std::vector<int> extFBset = selFB->extractFibersByPF(selPF);


  //MITK_INFO << "returned vector in FBOperationsView: " << extFBset.size();
  //  for(std::vector<int>::iterator dispIt = extFBset.begin(); dispIt != extFBset.end(); dispIt++)
  //  {
  //    MITK_INFO << "vector DTI ID: " << *dispIt;
  //
  //  }


  mitk::FiberBundle::Pointer extFB = selFB->extractFibersById(extFBset);
  MITK_INFO << " Number Of Tracts in sourceFiberBundle: " << selFB->GetNumTracts();
  MITK_INFO << " Number Of Tracts in extractedFiberBundle: " << extFB->GetNumTracts();

  mitk::DataNode::Pointer fbNode;
  fbNode = mitk::DataNode::New();
  fbNode->SetData(extFB);
  fbNode->SetName("extGroupFinberBundle");
  fbNode->SetVisibility(true);
  GetDataStorage()->Add(fbNode);



}

void QmitkFiberBundleOperationsView::generatePFCompo_AND()
{
  mitk::PlanarFigureComposite::Pointer PFCAnd = mitk::PlanarFigureComposite::New();
  PFCAnd->setOperationType(mitk::PFCOMPOSITION_AND_OPERATION);


  for( std::vector<mitk::DataNode::Pointer>::iterator it = m_SelectedPF.begin();
      it != m_SelectedPF.end(); ++it )
  {

    mitk::DataNode::Pointer nodePF = *it;
    mitk::PlanarFigure::Pointer tmpPF =  dynamic_cast<mitk::PlanarFigure*>( nodePF->GetData() );
    PFCAnd->addPlanarFigure( tmpPF );
    PFCAnd->addDataNode( nodePF );
    PFCAnd->setDisplayName("AND_COMPO");
    // MITK_INFO << "PFCAND(): added to AND PF" << nodePF->GetName();

  }

  debugPFComposition(PFCAnd, 0);


  this->addPFCompositionToDataStorage(PFCAnd, NULL /*parent*/);

}


void QmitkFiberBundleOperationsView::debugPFComposition(mitk::PlanarFigureComposite::Pointer pfc, int itLevelStatus)
{
  int myLevel = itLevelStatus;
  if (myLevel == 0)
  {
    MITK_INFO << "############################################## " ;
    MITK_INFO << "#########    DEBUG    START     ############## " ;
    MITK_INFO << "############################################## " ;
  }
  MITK_INFO << "############################################## " ;
  MITK_INFO << "Name: " << pfc->getDisplayName();
  MITK_INFO << "iterationLevel: " << myLevel;
  MITK_INFO << "CompositionType: " << pfc->getOperationType();
  MITK_INFO << "Number of children: " << pfc->getNumberOfChildren();


  //iterate through pfcs children
  for(int i=0; i<pfc->getNumberOfChildren(); ++i)
  {

    mitk::PlanarFigure::Pointer tmpPFchild = pfc->getChildAt(i);
    mitk::DataNode::Pointer savedPFchildNode = pfc->getDataNodeAt(i);

    if (tmpPFchild == savedPFchildNode->GetData())
    {
      MITK_INFO << "[OK] Pointers point to same Data...";

    }else{
      MITK_INFO << "Pointers differ in equation";
    }

    MITK_INFO << "Level: " << myLevel << " ChildNr.: " << i ;

    mitk::PlanarFigureComposite::Pointer pfcompcastNode= dynamic_cast<mitk::PlanarFigureComposite*>(savedPFchildNode->GetData());
    mitk::PlanarFigureComposite::Pointer pfcompcast= dynamic_cast<mitk::PlanarFigureComposite*>(tmpPFchild.GetPointer());
    if( !pfcompcast.IsNull() )
    { // we have a composite as child

      if ( pfcompcastNode.IsNull() )
      {
        MITK_INFO << "************** NODE DIFFER FROM PFC...ERROR! ***************";
      } else {
        MITK_INFO << "[OK]...node contains right type ";
      }



      itLevelStatus++;
      MITK_INFO << "child is PFC...debug this PFC";
      debugPFComposition(pfcompcast, itLevelStatus);

    } else {


      // we have a planarFigure as child
      // figure out which type
      mitk::PlanarCircle::Pointer circleName = mitk::PlanarCircle::New();
      mitk::PlanarRectangle::Pointer rectName = mitk::PlanarRectangle::New();
      mitk::PlanarPolygon::Pointer polyName = mitk::PlanarPolygon::New();


      if (tmpPFchild->GetNameOfClass() == circleName->GetNameOfClass() )
      {
        MITK_INFO << "a circle child of " << pfc->getDisplayName() ;

      } else if (tmpPFchild->GetNameOfClass() == rectName->GetNameOfClass() ){

        MITK_INFO << "a rectangle child of " << pfc->getDisplayName()  ;

      } else if (tmpPFchild->GetNameOfClass() == polyName->GetNameOfClass() ) {

        MITK_INFO << "a polygon child of " << pfc->getDisplayName()  ;
      }

      MITK_INFO << "....................................................... " ;




    }

  } //end for
  if (myLevel == 0)
  {
    MITK_INFO << "############################################## " ;
    MITK_INFO << "#########     DEBUG    END      ############## " ;
    MITK_INFO << "############################################## " ;
  }



}

void QmitkFiberBundleOperationsView::generatePFCompo_OR()
{
  mitk::PlanarFigureComposite::Pointer PFCOr = mitk::PlanarFigureComposite::New();
  PFCOr->setOperationType(mitk::PFCOMPOSITION_OR_OPERATION);


  for( std::vector<mitk::DataNode::Pointer>::iterator it = m_SelectedPF.begin();
      it != m_SelectedPF.end(); ++it )
  {

    mitk::DataNode::Pointer nodePF = *it;
    mitk::PlanarFigure::Pointer tmpPF =  dynamic_cast<mitk::PlanarFigure*>( nodePF->GetData() );
    PFCOr->addPlanarFigure( tmpPF );
    PFCOr->addDataNode( nodePF );
    PFCOr->setDisplayName("OR_COMPO");
    // MITK_INFO << "PFCAND(): added to AND PF" << nodePF->GetName();

  }

  debugPFComposition(PFCOr, 0);


  this->addPFCompositionToDataStorage(PFCOr, NULL /*parent*/);

}

void QmitkFiberBundleOperationsView::generatePFCompo_NOT()
{
  mitk::PlanarFigureComposite::Pointer PFCNot = mitk::PlanarFigureComposite::New();
  PFCNot->setOperationType(mitk::PFCOMPOSITION_NOT_OPERATION);


  for( std::vector<mitk::DataNode::Pointer>::iterator it = m_SelectedPF.begin();
      it != m_SelectedPF.end(); ++it )
  {

    mitk::DataNode::Pointer nodePF = *it;
    mitk::PlanarFigure::Pointer tmpPF =  dynamic_cast<mitk::PlanarFigure*>( nodePF->GetData() );
    PFCNot->addPlanarFigure( tmpPF );
    PFCNot->addDataNode( nodePF );
    PFCNot->setDisplayName("NOT_COMPO");
    // MITK_INFO << "PFCAND(): added to AND PF" << nodePF->GetName();

  }

  debugPFComposition(PFCNot, 0);


  this->addPFCompositionToDataStorage(PFCNot, NULL /*parent*/);

}

void QmitkFiberBundleOperationsView::deletePFCompo()
{

}

void QmitkFiberBundleOperationsView::addPFCompositionToDataStorage(mitk::PlanarFigureComposite::Pointer pfcomp, mitk::DataNode::Pointer parentDataNode )
{

  //a new planarFigureComposition arrived
  //convert it into a dataNode
  mitk::DataNode::Pointer newPFCNode;
  newPFCNode = mitk::DataNode::New();
  newPFCNode->SetName( pfcomp->getDisplayName() );
  //MITK_INFO << "PFComp Name: " << pfcomp->getDisplayName() << " newPFCNodeName: " << newPFCNode->GetName();
  newPFCNode->SetData(pfcomp);
  newPFCNode->SetVisibility(true);

  switch (pfcomp->getOperationType()) {
    case 0:
    {
      // AND PLANARFIGURECOMPOSITE
      // newPFCNode->SetName("AND_PFCombo");

      if (!parentDataNode.IsNull()) {
        MITK_INFO << "adding " << newPFCNode->GetName() << " to " << parentDataNode->GetName() ;
        GetDataStorage()->Add(newPFCNode, parentDataNode);

      } else {
        MITK_INFO << "adding " << newPFCNode->GetName();
        GetDataStorage()->Add(newPFCNode);

      }


      //iterate through its childs

      for(int i=0; i<pfcomp->getNumberOfChildren(); ++i)
      {
        mitk::PlanarFigure::Pointer tmpPFchild = pfcomp->getChildAt(i);
        mitk::DataNode::Pointer savedPFchildNode = pfcomp->getDataNodeAt(i);

        mitk::PlanarFigureComposite::Pointer pfcompcast= dynamic_cast<mitk::PlanarFigureComposite*>(tmpPFchild.GetPointer());
        if ( !pfcompcast.IsNull() )
        { // child is of type planar Figure composite
          // make new node of the child, cuz later the child has to be removed of its old position in datamanager
          // feed new dataNode with information of the savedDataNode, which is gonna be removed soon
          mitk::DataNode::Pointer newChildPFCNode;
          newChildPFCNode = mitk::DataNode::New();
          newChildPFCNode->SetData(tmpPFchild);
          newChildPFCNode->SetName( savedPFchildNode->GetName() );
          pfcompcast->setDisplayName(  savedPFchildNode->GetName()  ); //name might be changed in DataManager by user

          //update inside vector the dataNodePointer
          pfcomp->replaceDataNodeAt(i, newChildPFCNode);

          addPFCompositionToDataStorage(pfcompcast, newPFCNode); //the current PFCNode becomes the childs parent


          // remove savedNode here, cuz otherwise its children will change their position in the dataNodeManager
          // without having its parent anymore
          //GetDataStorage()->Remove(savedPFchildNode);

          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " exists in DS...trying to remove it";

          }else{
            MITK_INFO << "[ERROR] does NOT exist, but can I read its Name? " << savedPFchildNode->GetName();

          }
          // remove old child position in dataStorage
          GetDataStorage()->Remove(savedPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " still exists";
          }


        } else {

          // child is not of type PlanarFigureComposite, so its one of the planarFigures
          // create new dataNode containing the data of the old dataNode, but position in dataManager will be
          // modified cuz we re setting a (new) parent.
          mitk::DataNode::Pointer newPFchildNode = mitk::DataNode::New();
          newPFchildNode->SetName(savedPFchildNode->GetName() );
          newPFchildNode->SetData(tmpPFchild);
          newPFchildNode->SetVisibility(true);

          // replace the dataNode in PFComp DataNodeVector
          pfcomp->replaceDataNodeAt(i, newPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " exists in DS...trying to remove it";

          }else{
            MITK_INFO << "[ERROR] does NOT exist, but can I read its Name? " << savedPFchildNode->GetName();

          }
          // remove old child position in dataStorage
          GetDataStorage()->Remove(savedPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " still exists";
          }

          MITK_INFO << "adding " << newPFchildNode->GetName() << " to " << newPFCNode->GetName();
          //add new child to datamanager with its new position as child of newPFCNode parent
          GetDataStorage()->Add(newPFchildNode, newPFCNode);

        }




      }
      GetDataStorage()->Modified();




      break;
    }
    case 1:
    {
      // AND PLANARFIGURECOMPOSITE
      // newPFCNode->SetName("AND_PFCombo");

      if (!parentDataNode.IsNull()) {
        MITK_INFO << "adding " << newPFCNode->GetName() << " to " << parentDataNode->GetName() ;
        GetDataStorage()->Add(newPFCNode, parentDataNode);

      } else {
        MITK_INFO << "adding " << newPFCNode->GetName();
        GetDataStorage()->Add(newPFCNode);

      }


      //iterate through its childs

      for(int i=0; i<pfcomp->getNumberOfChildren(); ++i)
      {
        mitk::PlanarFigure::Pointer tmpPFchild = pfcomp->getChildAt(i);
        mitk::DataNode::Pointer savedPFchildNode = pfcomp->getDataNodeAt(i);

        mitk::PlanarFigureComposite::Pointer pfcompcast= dynamic_cast<mitk::PlanarFigureComposite*>(tmpPFchild.GetPointer());
        if ( !pfcompcast.IsNull() )
        { // child is of type planar Figure composite
          // make new node of the child, cuz later the child has to be removed of its old position in datamanager
          // feed new dataNode with information of the savedDataNode, which is gonna be removed soon
          mitk::DataNode::Pointer newChildPFCNode;
          newChildPFCNode = mitk::DataNode::New();
          newChildPFCNode->SetData(tmpPFchild);
          newChildPFCNode->SetName( savedPFchildNode->GetName() );
          pfcompcast->setDisplayName(  savedPFchildNode->GetName()  ); //name might be changed in DataManager by user

          //update inside vector the dataNodePointer
          pfcomp->replaceDataNodeAt(i, newChildPFCNode);

          addPFCompositionToDataStorage(pfcompcast, newPFCNode); //the current PFCNode becomes the childs parent


          // remove savedNode here, cuz otherwise its children will change their position in the dataNodeManager
          // without having its parent anymore
          //GetDataStorage()->Remove(savedPFchildNode);

          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " exists in DS...trying to remove it";

          }else{
            MITK_INFO << "[ERROR] does NOT exist, but can I read its Name? " << savedPFchildNode->GetName();

          }
          // remove old child position in dataStorage
          GetDataStorage()->Remove(savedPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " still exists";
          }


        } else {

          // child is not of type PlanarFigureComposite, so its one of the planarFigures
          // create new dataNode containing the data of the old dataNode, but position in dataManager will be
          // modified cuz we re setting a (new) parent.
          mitk::DataNode::Pointer newPFchildNode = mitk::DataNode::New();
          newPFchildNode->SetName(savedPFchildNode->GetName() );
          newPFchildNode->SetData(tmpPFchild);
          newPFchildNode->SetVisibility(true);

          // replace the dataNode in PFComp DataNodeVector
          pfcomp->replaceDataNodeAt(i, newPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " exists in DS...trying to remove it";

          }else{
            MITK_INFO << "[ERROR] does NOT exist, but can I read its Name? " << savedPFchildNode->GetName();

          }
          // remove old child position in dataStorage
          GetDataStorage()->Remove(savedPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " still exists";
          }

          MITK_INFO << "adding " << newPFchildNode->GetName() << " to " << newPFCNode->GetName();
          //add new child to datamanager with its new position as child of newPFCNode parent
          GetDataStorage()->Add(newPFchildNode, newPFCNode);

        }




      }
      GetDataStorage()->Modified();




      break;

    }
    case 2:
    {
      // AND PLANARFIGURECOMPOSITE
      // newPFCNode->SetName("AND_PFCombo");

      if (!parentDataNode.IsNull()) {
        MITK_INFO << "adding " << newPFCNode->GetName() << " to " << parentDataNode->GetName() ;
        GetDataStorage()->Add(newPFCNode, parentDataNode);

      } else {
        MITK_INFO << "adding " << newPFCNode->GetName();
        GetDataStorage()->Add(newPFCNode);

      }


      //iterate through its childs

      for(int i=0; i<pfcomp->getNumberOfChildren(); ++i)
      {
        mitk::PlanarFigure::Pointer tmpPFchild = pfcomp->getChildAt(i);
        mitk::DataNode::Pointer savedPFchildNode = pfcomp->getDataNodeAt(i);

        mitk::PlanarFigureComposite::Pointer pfcompcast= dynamic_cast<mitk::PlanarFigureComposite*>(tmpPFchild.GetPointer());
        if ( !pfcompcast.IsNull() )
        { // child is of type planar Figure composite
          // makeRemoveBundle new node of the child, cuz later the child has to be removed of its old position in datamanager
          // feed new dataNode with information of the savedDataNode, which is gonna be removed soon
          mitk::DataNode::Pointer newChildPFCNode;
          newChildPFCNode = mitk::DataNode::New();
          newChildPFCNode->SetData(tmpPFchild);
          newChildPFCNode->SetName( savedPFchildNode->GetName() );
          pfcompcast->setDisplayName(  savedPFchildNode->GetName()  ); //name might be changed in DataManager by user

          //update inside vector the dataNodePointer
          pfcomp->replaceDataNodeAt(i, newChildPFCNode);

          addPFCompositionToDataStorage(pfcompcast, newPFCNode); //the current PFCNode becomes the childs parent


          // remove savedNode here, cuz otherwise its children will change their position in the dataNodeManager
          // without having its parent anymore
          //GetDataStorage()->Remove(savedPFchildNode);

          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " exists in DS...trying to remove it";

          }else{
            MITK_INFO << "[ERROR] does NOT exist, but can I read its Name? " << savedPFchildNode->GetName();
          }
          // remove old child position in dataStorage
          GetDataStorage()->Remove(savedPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " still exists";
          }


        } else {

          // child is not of type PlanarFigureComposite, so its one of the planarFigures
          // create new dataNode containing the data of the old dataNode, but position in dataManager will be
          // modified cuz we re setting a (new) parent.
          mitk::DataNode::Pointer newPFchildNode = mitk::DataNode::New();
          newPFchildNode->SetName(savedPFchildNode->GetName() );
          newPFchildNode->SetData(tmpPFchild);
          newPFchildNode->SetVisibility(true);

          // replace the dataNode in PFComp DataNodeVector
          pfcomp->replaceDataNodeAt(i, newPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " exists in DS...trying to remove it";

          }else{
            MITK_INFO << "[ERROR] does NOT exist, but can I read its Name? " << savedPFchildNode->GetName();

          }
          // remove old child position in dataStorage
          GetDataStorage()->Remove(savedPFchildNode);


          if ( GetDataStorage()->Exists(savedPFchildNode)) {
            MITK_INFO << savedPFchildNode->GetName() << " still exists";
          }

          MITK_INFO << "adding " << newPFchildNode->GetName() << " to " << newPFCNode->GetName();
          //add new child to datamanager with its new position as child of newPFCNode parent
          GetDataStorage()->Add(newPFchildNode, newPFCNode);

        }




      }
      GetDataStorage()->Modified();




      break;
    }
    default:
      MITK_INFO << "we have an UNDEFINED composition... ERROR" ;
      break;
  }






}


void QmitkFiberBundleOperationsView::JoinBundles()
{
  mitk::FiberBundle::Pointer bundle1 = dynamic_cast<mitk::FiberBundle*>(m_SelectedFB.at(0)->GetData());
  mitk::FiberBundle::Pointer bundle2 = dynamic_cast<mitk::FiberBundle*>(m_SelectedFB.at(1)->GetData());

  mitk::FiberBundle::Pointer newBundle = bundle1->JoinBundle(bundle2);
  mitk::DataNode::Pointer fbNode = mitk::DataNode::New();
  fbNode->SetData(newBundle);
  fbNode->SetName(m_SelectedFB.at(0)->GetName()+"+"+m_SelectedFB.at(1)->GetName());
  fbNode->SetVisibility(true);
  GetDataStorage()->Add(fbNode);
}

void QmitkFiberBundleOperationsView::SubstractBundles()
{
  mitk::FiberBundle::Pointer bundle1 = dynamic_cast<mitk::FiberBundle*>(m_SelectedFB.at(0)->GetData());
  mitk::FiberBundle::Pointer bundle2 = dynamic_cast<mitk::FiberBundle*>(m_SelectedFB.at(1)->GetData());

  mitk::FiberBundle::Pointer newBundle = bundle1->SubstractBundle(bundle2);
  mitk::DataNode::Pointer fbNode = mitk::DataNode::New();
  fbNode->SetData(newBundle);
  fbNode->SetName(m_SelectedFB.at(0)->GetName()+"-"+m_SelectedFB.at(1)->GetName());
  fbNode->SetVisibility(true);
  GetDataStorage()->Add(fbNode);
}

void QmitkFiberBundleOperationsView::GenerationStart()
{
  int generationMethod = m_Controls->m_GenerationBox->currentIndex();

  std::vector<mitk::DataNode*> nodes = GetDataManagerSelection();
  if (nodes.empty()){
    QMessageBox::information( NULL, "Warning", "No data object selected!");
    MITK_WARN("QmitkFiberBundleOperationsView") << "no data object selected";
    return;
  }

  for( std::vector<mitk::DataNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it )
  {
    mitk::DataNode::Pointer node = *it;

    if (node.IsNotNull() && dynamic_cast<mitk::FiberBundle*>(node->GetData()))
    {
      m_FiberBundle = dynamic_cast<mitk::FiberBundle*>(node->GetData());
      m_FiberBundleNode = node;
      switch(generationMethod){
      case 0:
        GenerateGreyscaleHeatmap(true);
        break;
      case 1:
        GenerateGreyscaleHeatmap(false);
        break;
      case 2:
        GenerateColorHeatmap();
        break;
      case 3:
        GenerateFiberEndingsImage();
        break;
      case 4:
        GenerateFiberEndingsPointSet();
        break;
      case 5:
        DWIGenerationStart();
        break;
      }
    }
  }
}

// generate pointset displaying the fiber endings
void QmitkFiberBundleOperationsView::GenerateFiberEndingsPointSet()
{
  if(m_FiberBundle.IsNull()){
    QMessageBox::information( NULL, "Warning", "No fiber bundle selected!");
    MITK_WARN("QmitkGlobalFiberTrackingView") << "no fiber bundle selected";
    return;
  }

  mitk::Geometry3D::Pointer geometry = m_FiberBundle->GetGeometry();

  mitk::PointSet::Pointer pointSet = mitk::PointSet::New();

  int numTracts = m_FiberBundle->GetNumTracts();
  int count = 0;
  for( int i=0; i<numTracts; i++ )
  {
    // get fiber start point
    int numVertices = m_FiberBundle->GetNumPoints(i);
    itk::Point<float, 3> start = m_FiberBundle->GetPoint(i,0);
    itk::Point<float, 3> world1;
    geometry->IndexToWorld(start, world1);
    pointSet->InsertPoint(count, world1);
    count++;
    // get fiber end point
    if(numVertices>1)
    {
      itk::Point<float, 3> end = m_FiberBundle->GetPoint(i,numVertices-1);
      itk::Point<float, 3> world;
      geometry->IndexToWorld(end, world);
      pointSet->InsertPoint(count, world);
      count++;
    }
  }

  mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
  pointSetNode->SetData( pointSet );
  QString name(m_FiberBundleNode->GetName().c_str());
  name += "_fiber_endings";
  pointSetNode->SetName(name.toStdString());
  pointSetNode->SetProperty( "opacity", mitk::FloatProperty::New( 1 ) );
  pointSetNode->SetProperty( "pointsize", mitk::FloatProperty::New( 0.3) );
  pointSetNode->SetColor( 1.0, 1.0, 1.0 );

  GetDefaultDataStorage()->Add(pointSetNode);
}

// generate image displaying the fiber endings
void QmitkFiberBundleOperationsView::GenerateFiberEndingsImage()
{
  if(m_FiberBundle.IsNull()){
    QMessageBox::information( NULL, "Warning", "No fiber bundle selected!");
    MITK_WARN("QmitkGlobalFiberTrackingView") << "no fiber bundle selected";
    return;
  }

  typedef unsigned char OutPixType;

  // run generator
  typedef itk::TractsToFiberEndingsImageFilter<FloatImageType,
      OutPixType> ImageGeneratorType;
  ImageGeneratorType::Pointer generator = ImageGeneratorType::New();
  generator->SetFiberBundle(m_FiberBundle);

  generator->SetUpsamplingFactor(m_Controls->m_UpsamplingSpinBox->value());
  generator->Update();

  // get result
  typedef itk::Image<OutPixType,3> OutType;
  OutType::Pointer outImg = generator->GetOutput();

  mitk::Image::Pointer img = mitk::Image::New();
  img->InitializeByItk(outImg.GetPointer());
  img->SetVolume(outImg->GetBufferPointer());

  // to datastorage
  mitk::DataNode::Pointer node = mitk::DataNode::New();
  node->SetData(img);
  QString name(m_FiberBundleNode->GetName().c_str());
  name += "_fiber_endings";
  node->SetName(name.toStdString());
  node->SetVisibility(true);

  GetDataStorage()->Add(node);
}

// generate rgba heatmap from fiber bundle
void QmitkFiberBundleOperationsView::GenerateColorHeatmap()
{
  if(m_FiberBundle.IsNull() || m_FiberBundleNode.IsNull())
  {
    QMessageBox::information( NULL, "Warning", "No fiber bundle selected!");
    MITK_WARN("QmitkGlobalFiberTrackingView") << "no fiber bundle selected";
    return;
  }

  typedef itk::RGBAPixel<unsigned char> OutPixType;

  // run generator
  typedef itk::TractsToProbabilityImageFilter<FloatImageType, OutPixType>
      ImageGeneratorType;
  ImageGeneratorType::Pointer generator = ImageGeneratorType::New();
  //generator->SetInput(NULL);
  generator->SetFiberBundle(m_FiberBundle);

  generator->SetUpsamplingFactor(m_Controls->m_UpsamplingSpinBox->value());
  generator->Update();

  // get result
  typedef itk::Image<OutPixType,3> OutType;
  OutType::Pointer outImg = generator->GetOutput();

  mitk::Image::Pointer img = mitk::Image::New();
  img->InitializeByItk(outImg.GetPointer());
  img->SetVolume(outImg->GetBufferPointer());

  // to datastorage
  mitk::DataNode::Pointer node = mitk::DataNode::New();
  node->SetData(img);
  QString name(m_FiberBundleNode->GetName().c_str());
  name += "_rgba_heatmap";
  node->SetName(name.toStdString());
  node->SetVisibility(true);

  mitk::LevelWindow opaclevwin;
  opaclevwin.SetRangeMinMax(0,255);
  opaclevwin.SetWindowBounds(0,0);
  mitk::LevelWindowProperty::Pointer prop =
      mitk::LevelWindowProperty::New(opaclevwin);
  node->AddProperty( "opaclevelwindow", prop );

  GetDataStorage()->Add(node);
}

// generate greyscale heatmap from fiber bundle
void QmitkFiberBundleOperationsView::GenerateGreyscaleHeatmap(bool binary)
{
  if(m_FiberBundle.IsNull() || m_FiberBundleNode.IsNull())
  {
    QMessageBox::information( NULL, "Warning", "No fiber bundle selected!");
    MITK_WARN("QmitkGlobalFiberTrackingView") << "no fiber bundle selected";
    return;
  }

  typedef unsigned char OutPixType;

  // run generator
  typedef itk::TractsToProbabilityImageFilter<FloatImageType, OutPixType> ImageGeneratorType;
  ImageGeneratorType::Pointer generator = ImageGeneratorType::New();
  generator->SetFiberBundle(m_FiberBundle);
  generator->SetInvertImage(m_Controls->m_InvertCheckbox->isChecked());
  generator->SetUpsamplingFactor(m_Controls->m_UpsamplingSpinBox->value());
  if (binary)
    generator->SetBinaryEnvelope(true);
  else
    generator->SetBinaryEnvelope(false);
  generator->Update();

  // get result
  typedef itk::Image<OutPixType,3> OutType;
  OutType::Pointer outImg = generator->GetOutput();

  mitk::Image::Pointer img = mitk::Image::New();
  img->InitializeByItk(outImg.GetPointer());
  img->SetVolume(outImg->GetBufferPointer());

  // to datastorage
  mitk::DataNode::Pointer node = mitk::DataNode::New();
  node->SetData(img);
  QString name(m_FiberBundleNode->GetName().c_str());
  if(binary)
    name += "_enveloppe";
  else
    name += "_heatmap";
  node->SetName(name.toStdString());
  node->SetVisibility(true);

  mitk::LevelWindow opaclevwin2;
  opaclevwin2.SetRangeMinMax(0,255);
  opaclevwin2.SetWindowBounds(0,0);
  mitk::LevelWindowProperty::Pointer prop2 =
      mitk::LevelWindowProperty::New(opaclevwin2);
  node->AddProperty( "opaclevelwindow", prop2 );

  GetDataStorage()->Add(node);
}

// generate dwi from fiber bundle (experimental)
void QmitkFiberBundleOperationsView::DWIGenerationStart()
{
  // get fiber bundle and dwi image from data manager
  typedef mitk::DiffusionImage<short> DiffVolumesType;
  std::vector<mitk::DataNode*> nodes = GetDataManagerSelection();
  DiffVolumesType::Pointer originalDWI = NULL;
  mitk::FiberBundle::Pointer fiberBundle = NULL;
  for( std::vector<mitk::DataNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it )
  {
    mitk::DataNode::Pointer node = *it;

    if (node.IsNotNull() &&
        dynamic_cast<DiffVolumesType*>(node->GetData()))
    {
      originalDWI = dynamic_cast<DiffVolumesType*>(node->GetData());
      continue;
    }
    if (node.IsNotNull() &&
        dynamic_cast<mitk::FiberBundle*>(node->GetData()))
    {
      fiberBundle = dynamic_cast<mitk::FiberBundle*>(node->GetData());
    }
  }
  if(fiberBundle.IsNull() || originalDWI.IsNull()){
    QMessageBox::information( NULL, "Warning", "Please load and select a dwi image and a fiber bundle.");
    MITK_WARN("QmitkGlobalFiberTrackingView") << "please select a fiber bundle and a diffusion image";
    return;
  }

  // CONSTRUCT CONTAINER WITH DIRECTIONS
  typedef vnl_vector_fixed< double, 3 > GradientDirectionType;
  typedef itk::VectorContainer< unsigned int,  GradientDirectionType > GradientDirectionContainerType;
  GradientDirectionContainerType::Pointer directions = originalDWI->GetDirections();

  float bVal = originalDWI->GetB_Value();

  typedef itk::VectorImage< short, 3 > DWIImageType;
  DWIImageType::Pointer vectorImage = DWIImageType::New();
  itk::TractsToDWIImageFilter::Pointer filter = itk::TractsToDWIImageFilter::New();

  filter->SetInput(originalDWI->GetVectorImage());
  filter->SetFiberBundle(fiberBundle);
  filter->SetbD(m_Controls->m_UpsamplingSpinBox->value());
  filter->SetGradientDirections(directions);
  filter->SetParticleWidth(0.2);
  filter->GenerateData();
  vectorImage = filter->GetOutput();

  DiffVolumesType::Pointer diffImage = DiffVolumesType::New();
  diffImage->SetDirections(directions);
  diffImage->SetOriginalDirections(directions);
  diffImage->SetVectorImage(vectorImage);
  diffImage->SetB_Value(bVal);
  diffImage->InitializeFromVectorImage();

  mitk::DataNode::Pointer node = mitk::DataNode::New();
  node->SetData( diffImage );
  QString name(m_FiberBundleNode->GetName().c_str());
  name += "_dwi";
  GetDefaultDataStorage()->Add(node);
}

