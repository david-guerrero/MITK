/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 10185 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "QmitkBasicImageProcessingView.h"

// QT includes (GUI)
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qmessagebox.h>

// Berry includes (selection service)
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// MITK includes (GUI)
#include "QmitkStdMultiWidget.h"
#include "QmitkDataNodeSelectionProvider.h"
#include "mitkDataNodeObject.h"

// MITK includes (general)
#include "mitkNodePredicateDataType.h"
#include "mitkNodePredicateDimension.h"
#include "mitkNodePredicateAnd.h"
#include "mitkImageTimeSelector.h"
#include "mitkVectorImageMapper2D.h"
#include "mitkProperties.h"

// Includes for image casting between ITK and MITK
#include "mitkImageCast.h"
#include "mitkITKImageImport.h"

// ITK includes (general)
#include <itkVectorImage.h>

// Morphological Operations
#include <itkBinaryBallStructuringElement.h>
#include <itkGrayscaleDilateImageFilter.h>
#include <itkGrayscaleErodeImageFilter.h>
#include <itkGrayscaleMorphologicalOpeningImageFilter.h>
#include <itkGrayscaleMorphologicalClosingImageFilter.h>

// Smoothing
#include <itkMedianImageFilter.h>
#include <itkDiscreteGaussianImageFilter.h> 
#include <itkTotalVariationDenoisingImageFilter.h>

// Threshold
#include <itkBinaryThresholdImageFilter.h>

// Inversion
#include <itkInvertIntensityImageFilter.h>

// Derivatives
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkLaplacianImageFilter.h>
#include <itkSobelEdgeDetectionImageFilter.h>

// Resampling
#include <itkResampleImageFilter.h>
#include <itkNearestNeighborInterpolateImageFunction.h>

// Image Arithmetics
#include <itkAddImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkDivideImageFilter.h>

// Boolean operations
#include <itkOrImageFilter.h>
#include <itkAndImageFilter.h>
#include <itkXorImageFilter.h>


// Convenient Definitions
typedef itk::Image<short, 3>                                                            ImageType;
typedef itk::Image<double, 3>                                                           FloatImageType;
typedef itk::Image<itk::Vector<float,3>, 3>                                             VectorImageType;

typedef itk::BinaryBallStructuringElement<ImageType::PixelType, 3>                      BallType;
typedef itk::GrayscaleDilateImageFilter<ImageType, ImageType, BallType>                 DilationFilterType;
typedef itk::GrayscaleErodeImageFilter<ImageType, ImageType, BallType>                  ErosionFilterType; 
typedef itk::GrayscaleMorphologicalOpeningImageFilter<ImageType, ImageType, BallType>   OpeningFilterType;
typedef itk::GrayscaleMorphologicalClosingImageFilter<ImageType, ImageType, BallType>   ClosingFilterType;

typedef itk::MedianImageFilter< ImageType, ImageType >                                  MedianFilterType;
typedef itk::DiscreteGaussianImageFilter< ImageType, ImageType>                         GaussianFilterType;
typedef itk::TotalVariationDenoisingImageFilter<FloatImageType, FloatImageType>         TotalVariationFilterType;
typedef itk::TotalVariationDenoisingImageFilter<VectorImageType, VectorImageType>       VectorTotalVariationFilterType;

typedef itk::BinaryThresholdImageFilter< ImageType, ImageType >                         ThresholdFilterType;
typedef itk::InvertIntensityImageFilter< ImageType, ImageType >                         InversionFilterType;

typedef itk::GradientMagnitudeRecursiveGaussianImageFilter< ImageType, ImageType >      GradientFilterType;
typedef itk::LaplacianImageFilter< FloatImageType, FloatImageType >                     LaplacianFilterType;
typedef itk::SobelEdgeDetectionImageFilter< FloatImageType, FloatImageType >            SobelFilterType;

typedef itk::ResampleImageFilter< ImageType, ImageType >                                ResampleImageFilterType;

typedef itk::AddImageFilter< ImageType, ImageType, ImageType >                          AddFilterType;
typedef itk::SubtractImageFilter< ImageType, ImageType, ImageType >                     SubtractFilterType;
typedef itk::MultiplyImageFilter< ImageType, ImageType, ImageType >                     MultiplyFilterType;
typedef itk::DivideImageFilter< ImageType, ImageType, FloatImageType >                  DivideFilterType;

typedef itk::OrImageFilter< ImageType, ImageType >                                      OrImageFilterType;
typedef itk::AndImageFilter< ImageType, ImageType >                                     AndImageFilterType;
typedef itk::XorImageFilter< ImageType, ImageType >                                     XorImageFilterType;


QmitkBasicImageProcessing::QmitkBasicImageProcessing()
: QmitkFunctionality(),
  m_Controls(NULL),
  m_SelectedImageNode(NULL),
  m_TimeStepperAdapter(NULL),
  m_SelectionListener(NULL)
{
}

QmitkBasicImageProcessing::~QmitkBasicImageProcessing()
{
  //berry::ISelectionService* s = GetSite()->GetWorkbenchWindow()->GetSelectionService();
  //if(s)
  //  s->RemoveSelectionListener(m_SelectionListener);
}

void QmitkBasicImageProcessing::CreateQtPartControl(QWidget *parent)
{
  if (m_Controls == NULL)
  {
    m_Controls = new Ui::QmitkBasicImageProcessingViewControls;
    m_Controls->setupUi(parent);
    this->CreateConnections();


    //setup predictaes for combobox

    mitk::NodePredicateDimension::Pointer dimensionPredicate = mitk::NodePredicateDimension::New(3);
    mitk::NodePredicateDataType::Pointer imagePredicate = mitk::NodePredicateDataType::New("Image");
    m_Controls->m_ImageSelector2->SetDataStorage(this->GetDefaultDataStorage());
    m_Controls->m_ImageSelector2->SetPredicate(mitk::NodePredicateAnd::New(dimensionPredicate, imagePredicate));
  }
  m_Controls->gbTwoImageOps->hide();

  m_SelectedImageNode = mitk::DataStorageSelection::New(this->GetDefaultDataStorage(), false);
}

void QmitkBasicImageProcessing::CreateConnections()
{
  if ( m_Controls )
  {
    connect( (QObject*)(m_Controls->cbWhat1), SIGNAL( activated(int) ), this, SLOT( SelectAction(int) ) );
    connect( (QObject*)(m_Controls->btnDoIt), SIGNAL(clicked()),(QObject*) this, SLOT(StartButtonClicked()));

    connect( (QObject*)(m_Controls->cbWhat2), SIGNAL( activated(int) ), this, SLOT( SelectAction2(int) ) );
    connect( (QObject*)(m_Controls->btnDoIt2), SIGNAL(clicked()),(QObject*) this, SLOT(StartButton2Clicked()));

    connect( (QObject*)(m_Controls->rBOneImOp), SIGNAL( clicked() ), this, SLOT( ChangeGUI() ) );
    connect( (QObject*)(m_Controls->rBTwoImOp), SIGNAL( clicked() ), this, SLOT( ChangeGUI() ) );
  }

  m_TimeStepperAdapter = new QmitkStepperAdapter((QObject*) m_Controls->sliceNavigatorTime, 
    GetActiveStdMultiWidget()->GetTimeNavigationController()->GetTime(), "sliceNavigatorTimeFromBIP");
}

void QmitkBasicImageProcessing::Activated()
{
  QmitkFunctionality::Activated();

  this->m_Controls->cbWhat1->clear();
  this->m_Controls->cbWhat1->insertItem( NOACTIONSELECTED, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Please select operation", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( CATEGORY_DENOISING, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "--- Denoising ---", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( GAUSSIAN, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Gaussian", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( MEDIAN, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Median", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( TOTALVARIATION, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Total Variation", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( CATEGORY_MORPHOLOGICAL, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "--- Morphological ---", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( DILATION, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Dilation", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( EROSION, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Erosion", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( OPENING, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Opening", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( CLOSING, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Closing", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( CATEGORY_EDGE_DETECTION, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "--- Edge Detection ---", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( GRADIENT, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Gradient", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( LAPLACIAN, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Laplacian (2nd Derivative)", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( SOBEL, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Sobel Operator", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( CATEGORY_MISC, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "--- Misc ---", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( THRESHOLD, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Threshold", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( INVERSION, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Image Inversion", 0, QApplication::UnicodeUTF8) ));
  this->m_Controls->cbWhat1->insertItem( DOWNSAMPLING, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Downsampling", 0, QApplication::UnicodeUTF8) ));

  this->m_Controls->cbWhat2->clear();
  this->m_Controls->cbWhat2->insertItem( TWOIMAGESNOACTIONSELECTED, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Please select on operation", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( CATEGORY_ARITHMETIC, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "--- Arithmetric operations ---", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( ADD, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Add to Image 1:", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( SUBTRACT, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Subtract from Image 1:", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( MULTIPLY, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Multiply with Image 1:", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( DIVIDE, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "Divide Image 1 by:", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( CATEGORY_BOOLEAN, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "--- Boolean operations ---", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( AND, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "AND", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( OR, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "OR", 0, QApplication::UnicodeUTF8) ) );
  this->m_Controls->cbWhat2->insertItem( XOR, QString( QApplication::translate("QmitkBasicImageProcessingViewControls", "XOR", 0, QApplication::UnicodeUTF8) ) );

}

//datamanager selection changed
void QmitkBasicImageProcessing::OnSelectionChanged(std::vector<mitk::DataNode*> nodes)
{
  //any nodes there?
  if (!nodes.empty())
  {
  // reset GUI
  this->ResetOneImageOpPanel();
  m_Controls->sliceNavigatorTime->setEnabled(false);
  m_Controls->leImage1->setText("Select an Image in Data Manager");
  m_Controls->tlWhat1->setEnabled(false);
  m_Controls->cbWhat1->setEnabled(false);
  m_Controls->tlWhat2->setEnabled(false);
  m_Controls->cbWhat2->setEnabled(false);

  m_SelectedImageNode->RemoveAllNodes();
  //get the selected Node
  mitk::DataNode* _DataNode = nodes.front();
  *m_SelectedImageNode = _DataNode;
  //try to cast to image
  mitk::Image::Pointer tempImage = dynamic_cast<mitk::Image*>(m_SelectedImageNode->GetNode()->GetData());

    //no image
    if( tempImage.IsNull() || (tempImage->IsInitialized() == false) ) 
    {
      m_Controls->leImage1->setText("Not an image.");
      return;
    }

    //2D image
    if( tempImage->GetDimension() < 3)
    {
      m_Controls->leImage1->setText("2D images are not supported.");
      return;
    }

    //image
    m_Controls->leImage1->setText(QString(m_SelectedImageNode->GetNode()->GetName().c_str()));

    // button coding
    if ( tempImage->GetDimension() > 3 ) 
    {
      m_Controls->sliceNavigatorTime->setEnabled(true);
      m_Controls->tlTime->setEnabled(true);
    }
    m_Controls->tlWhat1->setEnabled(true);
    m_Controls->cbWhat1->setEnabled(true);
    m_Controls->tlWhat2->setEnabled(true);
    m_Controls->cbWhat2->setEnabled(true);
  }
}

void QmitkBasicImageProcessing::ChangeGUI()
{
  if(m_Controls->rBOneImOp->isChecked())
  {
    m_Controls->gbTwoImageOps->hide();
    m_Controls->gbOneImageOps->show();
  }
  else if(m_Controls->rBTwoImOp->isChecked())
  {
    m_Controls->gbOneImageOps->hide();
    m_Controls->gbTwoImageOps->show();
  }
}

void QmitkBasicImageProcessing::ResetOneImageOpPanel()
{
  m_Controls->tlParam1->setText("Param1");
  m_Controls->tlParam2->setText("Param2");

  m_Controls->cbWhat1->setCurrentIndex(0);

  m_Controls->tlTime->setEnabled(false);

  this->ResetParameterPanel();

  m_Controls->btnDoIt->setEnabled(false);
  m_Controls->cbHideOrig->setEnabled(false);
}

void QmitkBasicImageProcessing::ResetParameterPanel()
{
  m_Controls->tlParam->setEnabled(false);
  m_Controls->tlParam1->setEnabled(false);
  m_Controls->tlParam2->setEnabled(false);

  m_Controls->sbParam1->setEnabled(false);
  m_Controls->sbParam2->setEnabled(false);
  m_Controls->sbParam1->setValue(0);
  m_Controls->sbParam2->setValue(0);
}

void QmitkBasicImageProcessing::ResetTwoImageOpPanel()
{
  m_Controls->cbWhat2->setCurrentIndex(0);

  m_Controls->tlImage2->setEnabled(false);
  m_Controls->m_ImageSelector2->setEnabled(false);

  m_Controls->btnDoIt2->setEnabled(false);
}

void QmitkBasicImageProcessing::SelectAction(int action)
{
  if ( ! m_SelectedImageNode->GetNode() ) return;

  // Prepare GUI
  this->ResetParameterPanel();
  m_Controls->btnDoIt->setEnabled(false);
  m_Controls->cbHideOrig->setEnabled(false);

  QString text1 = "No Parameters";
  QString text2 = "No Parameters";

  // check which operation the user has selected and set parameters and GUI accordingly
  switch (action)
  {
  case 2: 
    {
      m_SelectedAction = GAUSSIAN;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "&Variance:";

      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 2 );
      break;
    }

  case 3:
    {
      m_SelectedAction = MEDIAN;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "&Radius:";
      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 3 );
      break;
    }

  case 4:
    {
      m_SelectedAction = TOTALVARIATION;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      m_Controls->tlParam2->setEnabled(true);
      m_Controls->sbParam2->setEnabled(true);
      text1 = "Number Iterations:";
      text2 = "Regularization\n(Lambda/1000):";
      m_Controls->sbParam1->setMinimum( 1 );
      m_Controls->sbParam1->setMaximum( 1000 );
      m_Controls->sbParam1->setValue( 40 );
      m_Controls->sbParam2->setMinimum( 0 );
      m_Controls->sbParam2->setMaximum( 100000 );
      m_Controls->sbParam2->setValue( 1 );
      break;
    }

  case 6: 
    {
      m_SelectedAction = DILATION;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "&Radius:";
      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 3 );
      break;
    }

  case 7: 
    {
      m_SelectedAction = EROSION;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "&Radius:";
      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 3 );
      break;
    }

  case 8: 
    {
      m_SelectedAction = OPENING;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "&Radius:";
      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 3 );
      break;
    }

  case 9:
    {
      m_SelectedAction = CLOSING;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "&Radius:";
      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 3 );
      break;
    }

  case 11:
    {
      m_SelectedAction = GRADIENT;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "Sigma of Gaussian Kernel:\n(in Image Spacing Units)";
      m_Controls->sbParam1->setMinimum( 0 );
      m_Controls->sbParam1->setMaximum( 200 );
      m_Controls->sbParam1->setValue( 2 );
      break; 
    }

  case 12:
    {
      m_SelectedAction = LAPLACIAN;
      break;
    }

  case 13:
    {
      m_SelectedAction = SOBEL;
      break;
    }

  case 15: 
    {
      m_SelectedAction = THRESHOLD;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      m_Controls->tlParam2->setEnabled(true);
      m_Controls->sbParam2->setEnabled(true);
      text1 = "Lower threshold:";
      text2 = "Upper threshold:";
      m_Controls->sbParam1->setMinimum( -100000 );
      m_Controls->sbParam1->setMaximum( 100000 );
      m_Controls->sbParam1->setValue( 0 );
      m_Controls->sbParam2->setMinimum( -100000 );
      m_Controls->sbParam2->setMaximum( 100000 );
      m_Controls->sbParam2->setValue( 300 );
      break;
    }

  case 16: 
    {
      m_SelectedAction = INVERSION;
      break;
    }

  case 17:
    {
      m_SelectedAction = DOWNSAMPLING;
      m_Controls->tlParam1->setEnabled(true);
      m_Controls->sbParam1->setEnabled(true);
      text1 = "Downsampling by Factor:";
      m_Controls->sbParam1->setMinimum( 1 );
      m_Controls->sbParam1->setMaximum( 100 );
      m_Controls->sbParam1->setValue( 2 );
      break;
    }

  default: return;
  }

  m_Controls->tlParam->setEnabled(true);
  m_Controls->tlParam1->setText(text1);
  m_Controls->tlParam2->setText(text2);

  m_Controls->btnDoIt->setEnabled(true);
  m_Controls->cbHideOrig->setEnabled(true);
}

void QmitkBasicImageProcessing::StartButtonClicked() 
{
  if(!m_SelectedImageNode->GetNode()) return;

  this->BusyCursorOn();

  mitk::Image::Pointer newImage;
  
  try
  {
    newImage = dynamic_cast<mitk::Image*>(m_SelectedImageNode->GetNode()->GetData());
  }
  catch ( std::exception &e )
  {
  QString exceptionString = "An error occured during image loading:\n";
  exceptionString.append( e.what() );
    QMessageBox::warning( NULL, "Basic Image Processing", exceptionString , QMessageBox::Ok, QMessageBox::NoButton );
    this->BusyCursorOff();
    return;
  }
  
  // check if input image is valid, casting does not throw exception when casting from 'NULL-Object'
  if ( (! newImage) || (newImage->IsInitialized() == false) ) 
  {
    this->BusyCursorOff();
   
    QMessageBox::warning( NULL, "Basic Image Processing", "Input image is broken or not initialized. Returning.", QMessageBox::Ok, QMessageBox::NoButton );
    return;
  }
  
  // check if operation is done on 4D a image time step
  if(newImage->GetDimension() > 3)
  {
    mitk::ImageTimeSelector::Pointer timeSelector = mitk::ImageTimeSelector::New();
    timeSelector->SetInput(newImage);
    timeSelector->SetTimeNr( ((QmitkSliderNavigatorWidget*)m_Controls->sliceNavigatorTime)->GetPos() );
    timeSelector->Update();
    newImage = timeSelector->GetOutput();
  }



  // check if image or vector image
  ImageType::Pointer itkImage = ImageType::New();
  VectorImageType::Pointer itkVecImage = VectorImageType::New();

  int isVectorImage = newImage->GetPixelType().GetNumberOfComponents();

  if(isVectorImage > 1)
  {
    CastToItkImage( newImage, itkVecImage );
  }
  else
  {
    CastToItkImage( newImage, itkImage );
  }

  std::stringstream nameAddition("");

  int param1 = m_Controls->sbParam1->value();
  int param2 = m_Controls->sbParam2->value();

  try{

  switch (m_SelectedAction)
  {

  case GAUSSIAN:
    {
      GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
      gaussianFilter->SetInput( itkImage );
      gaussianFilter->SetVariance( param1 );
      gaussianFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(gaussianFilter->GetOutput());
      nameAddition << "_Gaussian_var_" << param1;
      std::cout << "Gaussian filtering successful." << std::endl;
      break;
    }

  case MEDIAN:
    {
      MedianFilterType::Pointer medianFilter = MedianFilterType::New();
      MedianFilterType::InputSizeType size;
      size.Fill(param1);
      medianFilter->SetRadius( size );
      medianFilter->SetInput(itkImage);
      medianFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(medianFilter->GetOutput());
      nameAddition << "_Median_radius_" << param1;
      std::cout << "Median Filtering successful." << std::endl;
      break;
    }

  case TOTALVARIATION:
    {
      if(isVectorImage > 1)
      {
        VectorTotalVariationFilterType::Pointer TVFilter
          = VectorTotalVariationFilterType::New();
        TVFilter->SetInput( itkVecImage.GetPointer() );
        TVFilter->SetNumberIterations(param1);
        TVFilter->SetLambda(double(param2)/1000.);
        TVFilter->UpdateLargestPossibleRegion();

        newImage = mitk::ImportItkImage(TVFilter->GetOutput());
      }
      else
      {
        FloatImageType::Pointer fImage = FloatImageType::New();
        CastToItkImage( newImage, fImage );
        TotalVariationFilterType::Pointer TVFilter
          = TotalVariationFilterType::New();
        TVFilter->SetInput( fImage.GetPointer() );
        TVFilter->SetNumberIterations(param1);
        TVFilter->SetLambda(double(param2)/1000.);
        TVFilter->UpdateLargestPossibleRegion();

        newImage = mitk::ImportItkImage(TVFilter->GetOutput());
      }

      nameAddition << "_TV_Iter_" << param1 << "_L_" << param2;
      std::cout << "Total Variation Filtering successful." << std::endl;
      break;
    }

  case DILATION:
    {
      BallType binaryBall;
      binaryBall.SetRadius( param1 );
      binaryBall.CreateStructuringElement();

      DilationFilterType::Pointer dilationFilter = DilationFilterType::New();
      dilationFilter->SetInput( itkImage );
      dilationFilter->SetKernel( binaryBall );
      dilationFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(dilationFilter->GetOutput());
      nameAddition << "_Dilated_by_" << param1;
      std::cout << "Dilation successful." << std::endl;
      break;
    }

  case EROSION:
    {
      BallType binaryBall;
      binaryBall.SetRadius( param1 );
      binaryBall.CreateStructuringElement();

      ErosionFilterType::Pointer erosionFilter = ErosionFilterType::New();
      erosionFilter->SetInput( itkImage );
      erosionFilter->SetKernel( binaryBall );
      erosionFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(erosionFilter->GetOutput());
      nameAddition << "_Eroded_by_" << param1;
      std::cout << "Erosion successful." << std::endl;
      break;
    }

  case OPENING:
    {
      BallType binaryBall;
      binaryBall.SetRadius( param1 );
      binaryBall.CreateStructuringElement();

      OpeningFilterType::Pointer openFilter = OpeningFilterType::New();
      openFilter->SetInput( itkImage );
      openFilter->SetKernel( binaryBall );
      openFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(openFilter->GetOutput());
      nameAddition << "_Opened_by_" << param1;
      std::cout << "Opening successful." << std::endl;
      break;
    }

  case CLOSING:
    {
      BallType binaryBall;
      binaryBall.SetRadius( param1 );
      binaryBall.CreateStructuringElement();

      ClosingFilterType::Pointer closeFilter = ClosingFilterType::New();
      closeFilter->SetInput( itkImage );
      closeFilter->SetKernel( binaryBall );
      closeFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(closeFilter->GetOutput());
      nameAddition << "_Closed_by_" << param1;
      std::cout << "Closing successful." << std::endl;
      break;
    }

  case GRADIENT:
    {
      GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
      gradientFilter->SetInput( itkImage );
      gradientFilter->SetSigma( param1 );
      gradientFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(gradientFilter->GetOutput());
      nameAddition << "_Gradient_sigma_" << param1;
      std::cout << "Gradient calculation successful." << std::endl;
      break;
    }

  case LAPLACIAN:
    {
      FloatImageType::Pointer fImage = FloatImageType::New();
      CastToItkImage( newImage, fImage );
      LaplacianFilterType::Pointer laplacianFilter = LaplacianFilterType::New();
      laplacianFilter->SetInput( fImage );
      laplacianFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(laplacianFilter->GetOutput());
      nameAddition << "_Second_Derivative";
      std::cout << "Laplacian filtering successful." << std::endl;
      break;
    }

  case SOBEL:
    {
      FloatImageType::Pointer fImage = FloatImageType::New();
      CastToItkImage( newImage, fImage );
      SobelFilterType::Pointer sobelFilter = SobelFilterType::New();
      sobelFilter->SetInput( fImage );
      sobelFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(sobelFilter->GetOutput());
      nameAddition << "_Sobel";
      std::cout << "Edge Detection successful." << std::endl;
      break;
    }

  case THRESHOLD:
    {
      ThresholdFilterType::Pointer thFilter = ThresholdFilterType::New();
      thFilter->SetLowerThreshold(param1 < param2 ? param1 : param2);
      thFilter->SetUpperThreshold(param2 > param1 ? param2 : param1);
      thFilter->SetInsideValue(1);
      thFilter->SetOutsideValue(0);
      thFilter->SetInput(itkImage);
      thFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(thFilter->GetOutput());
      nameAddition << "_Threshold";
      std::cout << "Thresholding successful." << std::endl;
      break;
    }

  case INVERSION:
    {
      InversionFilterType::Pointer invFilter = InversionFilterType::New();
      mitk::ScalarType min = newImage->GetScalarValueMin();
      mitk::ScalarType max = newImage->GetScalarValueMax();
      invFilter->SetMaximum( max + min );
      invFilter->SetInput(itkImage);
      invFilter->UpdateLargestPossibleRegion();
      newImage = mitk::ImportItkImage(invFilter->GetOutput());
      nameAddition << "_Inverted";
      std::cout << "Image inversion successful." << std::endl;
      break;
    }

  case DOWNSAMPLING:
    {
      ResampleImageFilterType::Pointer downsampler = ResampleImageFilterType::New();
      downsampler->SetInput( itkImage );

      typedef itk::NearestNeighborInterpolateImageFunction< ImageType, double > InterpolatorType;
      InterpolatorType::Pointer interpolator = InterpolatorType::New();
      downsampler->SetInterpolator( interpolator );

      downsampler->SetDefaultPixelValue( 0 );

      ResampleImageFilterType::SpacingType spacing = itkImage->GetSpacing();
      spacing *= (double) param1;
      downsampler->SetOutputSpacing( spacing );

      downsampler->SetOutputOrigin( itkImage->GetOrigin() );
      downsampler->SetOutputDirection( itkImage->GetDirection() );

      ResampleImageFilterType::SizeType size = itkImage->GetLargestPossibleRegion().GetSize();
      for ( int i = 0; i < 3; ++i )
      {
        size[i] /= param1;
      }
      downsampler->SetSize( size );
      downsampler->UpdateLargestPossibleRegion();

      newImage = mitk::ImportItkImage(downsampler->GetOutput());
      nameAddition << "_Downsampled_by_" << param1;
      std::cout << "Downsampling successful." << std::endl;
      break;
    }

  default:
    this->BusyCursorOff();
    return;
  }
  }
  catch (...)
  {
    this->BusyCursorOff();
    QMessageBox::warning(NULL, "Warning", "Problem when applying filter operation. Check your input...");
    return;
  }

  newImage->DisconnectPipeline();

  // adjust level/window to new image 
  mitk::LevelWindow levelwindow;
  levelwindow.SetAuto( newImage );
  mitk::LevelWindowProperty::Pointer levWinProp = mitk::LevelWindowProperty::New();
  levWinProp->SetLevelWindow( levelwindow );

  // compose new image name 
  std::string name = m_SelectedImageNode->GetNode()->GetName();
  if (name.find(".pic.gz") == name.size() -7 )
  {
    name = name.substr(0,name.size() -7);
  }
  name.append( nameAddition.str() );

  // create final result MITK data storage node
  mitk::DataNode::Pointer result = mitk::DataNode::New();
  result->SetProperty( "levelwindow", levWinProp );
  result->SetProperty( "name", mitk::StringProperty::New( name.c_str() ) );
  result->SetData( newImage );

  // for vector images, a different mapper is needed
  if(isVectorImage > 1)
  {
    mitk::VectorImageMapper2D::Pointer mapper = 
      mitk::VectorImageMapper2D::New();
    result->SetMapper(1,mapper);
  }

  // reset GUI to ease further processing
  this->ResetOneImageOpPanel();

  // add new image to data storage and set as active to ease further processing
  GetDefaultDataStorage()->Add( result, m_SelectedImageNode->GetNode() );
  if ( m_Controls->cbHideOrig->isChecked() == true )
    m_SelectedImageNode->GetNode()->SetProperty( "visible", mitk::BoolProperty::New(false) );
  // TODO!! m_Controls->m_ImageSelector1->SetSelectedNode(result);

  // show the results
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  this->BusyCursorOff();
}

void QmitkBasicImageProcessing::SelectAction2(int operation)
{
  // check which operation the user has selected and set parameters and GUI accordingly
  switch (operation)
  {
  case 2:
    m_SelectedOperation = ADD;
    break;
  case 3:
    m_SelectedOperation = SUBTRACT;
    break;
  case 4:
    m_SelectedOperation = MULTIPLY;
    break;
  case 5: 
    m_SelectedOperation = DIVIDE;
    break;
  case 7: 
    m_SelectedOperation = AND;
    break;
  case 8: 
    m_SelectedOperation = OR;
    break;
  case 9: 
    m_SelectedOperation = XOR;
    break;
  default: 
    this->ResetTwoImageOpPanel();
    return;
  }
  m_Controls->tlImage2->setEnabled(true);
  m_Controls->m_ImageSelector2->setEnabled(true);
  m_Controls->btnDoIt2->setEnabled(true);
}

void QmitkBasicImageProcessing::StartButton2Clicked() 
{
  mitk::Image::Pointer newImage1 = dynamic_cast<mitk::Image*>
    (m_SelectedImageNode->GetNode()->GetData());
  mitk::Image::Pointer newImage2 = dynamic_cast<mitk::Image*>
    (m_Controls->m_ImageSelector2->GetSelectedNode()->GetData());

  // check if images are valid
  if( (!newImage1) || (!newImage2) || (newImage1->IsInitialized() == false) || (newImage2->IsInitialized() == false) ) 
  {
    itkGenericExceptionMacro(<< "At least one of the input images are broken or not initialized. Returning");
    return;
  }

  this->BusyCursorOn();
  this->ResetTwoImageOpPanel();

  // check if 4D image and use filter on correct time step
  int time = ((QmitkSliderNavigatorWidget*)m_Controls->sliceNavigatorTime)->GetPos();
  if(time>=0)
  {
    mitk::ImageTimeSelector::Pointer timeSelector = mitk::ImageTimeSelector::New();

    timeSelector->SetInput(newImage1);
    timeSelector->SetTimeNr( time );
    timeSelector->UpdateLargestPossibleRegion();
    newImage1 = timeSelector->GetOutput();
    newImage1->DisconnectPipeline();

    timeSelector->SetInput(newImage2);
    timeSelector->SetTimeNr( time );
    timeSelector->UpdateLargestPossibleRegion();
    newImage2 = timeSelector->GetOutput();
    newImage2->DisconnectPipeline();
  }

  // reset GUI for better usability
  this->ResetTwoImageOpPanel();

  ImageType::Pointer itkImage1 = ImageType::New();
  ImageType::Pointer itkImage2 = ImageType::New();

  CastToItkImage( newImage1, itkImage1 );
  CastToItkImage( newImage2, itkImage2 );

  // Remove temp image
  newImage2 = NULL;

  std::string nameAddition = "";

  try
  {
  switch (m_SelectedOperation)
  {
  case ADD:
    {
      AddFilterType::Pointer addFilter = AddFilterType::New();
      addFilter->SetInput1( itkImage1 );
      addFilter->SetInput2( itkImage2 );
      addFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage(addFilter->GetOutput());
      nameAddition = "_Added";
    }
    break;

  case SUBTRACT:
    {
      SubtractFilterType::Pointer subFilter = SubtractFilterType::New();
      subFilter->SetInput1( itkImage1 );
      subFilter->SetInput2( itkImage2 );
      subFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage(subFilter->GetOutput());
      nameAddition = "_Subtracted";
    }
    break;

  case MULTIPLY:
    {
      MultiplyFilterType::Pointer multFilter = MultiplyFilterType::New();
      multFilter->SetInput1( itkImage1 );
      multFilter->SetInput2( itkImage2 );
      multFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage(multFilter->GetOutput());
      nameAddition = "_Multiplied";
    }
    break;

  case DIVIDE:
    {
      DivideFilterType::Pointer divFilter = DivideFilterType::New();
      divFilter->SetInput1( itkImage1 );
      divFilter->SetInput2( itkImage2 );
      divFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage<FloatImageType>(divFilter->GetOutput());
      nameAddition = "_Divided";
    }
    break;

  case AND:
    {
      AndImageFilterType::Pointer andFilter = AndImageFilterType::New();
      andFilter->SetInput1( itkImage1 );
      andFilter->SetInput2( itkImage2 );
      andFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage(andFilter->GetOutput());
      nameAddition = "_AND";
      break;
    }

  case OR:
    {
      OrImageFilterType::Pointer orFilter = OrImageFilterType::New();
      orFilter->SetInput1( itkImage1 );
      orFilter->SetInput2( itkImage2 );
      orFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage(orFilter->GetOutput());
      nameAddition = "_OR";
      break;
    }

  case XOR:
    {
      XorImageFilterType::Pointer xorFilter = XorImageFilterType::New();
      xorFilter->SetInput1( itkImage1 );
      xorFilter->SetInput2( itkImage2 );
      xorFilter->UpdateLargestPossibleRegion();
      newImage1 = mitk::ImportItkImage(xorFilter->GetOutput());
      nameAddition = "_XOR";
      break;
    }

  default: 
    std::cout << "Something went wrong..." << std::endl;
    this->BusyCursorOff();
    return;
  }
  }
  catch (...)
  {
    this->BusyCursorOff();
    QMessageBox::warning(NULL, "Warning", "Problem when applying arithmetic operation to two images. Check dimensions of input images.");
    return;
  }

  // disconnect pipeline; images will not be reused
  newImage1->DisconnectPipeline();
  itkImage1 = NULL;
  itkImage2 = NULL;

  // adjust level/window to new image and compose new image name
  mitk::LevelWindow levelwindow;
  levelwindow.SetAuto( newImage1 );
  mitk::LevelWindowProperty::Pointer levWinProp = mitk::LevelWindowProperty::New();
  levWinProp->SetLevelWindow( levelwindow );
  std::string name = m_SelectedImageNode->GetNode()->GetName();
  if (name.find(".pic.gz") == name.size() -7 )
  {
    name = name.substr(0,name.size() -7);
  }

  // create final result MITK data storage node
  mitk::DataNode::Pointer result = mitk::DataNode::New();  
  result->SetProperty( "levelwindow", levWinProp );
  result->SetProperty( "name", mitk::StringProperty::New( (name + nameAddition ).c_str() ));
  result->SetData( newImage1 );
  GetDefaultDataStorage()->Add( result, m_SelectedImageNode->GetNode() );

  // show only the newly created image
  m_SelectedImageNode->GetNode()->SetProperty( "visible", mitk::BoolProperty::New(false) );
  m_Controls->m_ImageSelector2->GetSelectedNode()->SetProperty( "visible", mitk::BoolProperty::New(false) );

  // show the newly created image
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  this->BusyCursorOff();
}

