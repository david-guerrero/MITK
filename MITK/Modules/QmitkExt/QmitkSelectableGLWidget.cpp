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

#include "QmitkSelectableGLWidget.h"
#include "QmitkRenderWindow.h"

#include <QWheelEvent>

QmitkSelectableGLWidget::QmitkSelectableGLWidget(QWidget* parent)
  : QWidget(parent)
{
  setupUi(this);
  QLayout *hlayout;
  hlayout=layout();
  hlayout->setMargin(3);
  //hlayout->setAutoAdd(true);

  QString rendererName("Renderer::");
  rendererName += objectName();

  // create Renderer
  m_Renderer= mitk::VtkPropRenderer::New( qPrintable(rendererName), NULL, mitk::RenderingManager::GetInstance() ); 

  // create widget
  QString composedName("QSGLWt::");
  if(!objectName().isEmpty())
      composedName+=objectName();
  else
      composedName+="QmitkGLWidget";
  /*
  * here is the place to define QT-Flags to enable and disable certain OpenGL elements, like StencilBuffer, RGBA and so on.
  * See QGLFormat for futher informations
  * QGL::AlphaChannel: Enable Alpha in Framebuffer
  * QGL::Rgba enable use of rgba rather than color_index
  * QGL::StencilBuffer for use of stencilbuffer in OpenGL
  */
  m_RenderWindow = new QmitkRenderWindow(this, composedName,m_Renderer);

  hlayout->addWidget(m_RenderWindow);
  
}


mitk::VtkPropRenderer* QmitkSelectableGLWidget::GetRenderer()
{
  return m_Renderer.GetPointer();
}


QmitkRenderWindow* QmitkSelectableGLWidget::GetRenderWindow() const
{
  return m_RenderWindow;
}


void QmitkSelectableGLWidget::wheelEvent( QWheelEvent * e )
{
  if ( m_RenderWindow->GetSliceNavigationController()->GetSliceLocked() )
    return;
  
  mitk::Stepper* stepper = m_RenderWindow
    ->GetSliceNavigationController()->GetSlice();
  
  if (stepper->GetSteps() <= 1)
  {
    stepper = m_RenderWindow->GetSliceNavigationController()->GetTime();
  }

  if (e->orientation() * e->delta()  > 0) 
  {
    stepper->Next();
  }
  else
  {
    stepper->Previous();
  }
}


mitk::SliceNavigationController*
QmitkSelectableGLWidget
::GetSliceNavigationController() const
{
  return m_RenderWindow->GetSliceNavigationController();
}


mitk::CameraRotationController*
QmitkSelectableGLWidget
::GetCameraRotationController() const
{
  return m_RenderWindow->GetCameraRotationController();
}


mitk::BaseController*
QmitkSelectableGLWidget
::GetController() const
{
  return m_RenderWindow->GetController();
}
