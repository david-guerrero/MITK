/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef QmitkNormalizedMutualInformationHistogramMetricViewWidgetHIncluded
#define QmitkNormalizedMutualInformationHistogramMetricViewWidgetHIncluded

#include "ui_QmitkNormalizedMutualInformationHistogramMetricControls.h"
#include "MitkRigidRegistrationUIExports.h"
#include "QmitkRigidRegistrationMetricsGUIBase.h"
#include <itkArray.h>
#include <itkObject.h>
#include <itkImage.h>

/*!
* \brief Widget for rigid registration
*
* Displays options for rigid registration.
*/
class MITK_RIGIDREGISTRATION_UI_EXPORT QmitkNormalizedMutualInformationHistogramMetricView : public QmitkRigidRegistrationMetricsGUIBase
{

public:

  QmitkNormalizedMutualInformationHistogramMetricView( QWidget* parent = 0, Qt::WindowFlags f = 0 );
  ~QmitkNormalizedMutualInformationHistogramMetricView();

  virtual mitk::MetricParameters::MetricType GetMetricType();

  virtual itk::Object::Pointer GetMetric();

  virtual itk::Array<double> GetMetricParameters();

  virtual void SetMetricParameters(itk::Array<double> metricValues);

  virtual QString GetName();

  virtual void SetupUI(QWidget* parent);

  virtual bool Maximize();


private:

  template < class TPixelType, unsigned int VImageDimension >
  itk::Object::Pointer GetMetric2(itk::Image<TPixelType, VImageDimension>* itkImage1);

protected:

  Ui::QmitkNormalizedMutualInformationHistogramMetricControls m_Controls;

  itk::Object::Pointer m_MetricObject;

};

#endif
