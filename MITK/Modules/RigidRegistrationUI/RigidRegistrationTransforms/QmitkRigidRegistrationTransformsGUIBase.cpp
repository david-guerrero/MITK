/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: -1 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "QmitkRigidRegistrationTransformsGUIBase.h"

QmitkRigidRegistrationTransformsGUIBase::QmitkRigidRegistrationTransformsGUIBase(QWidget* parent, Qt::WindowFlags f) : QWidget( parent, f ), 
  m_FixedImage(NULL), m_MovingImage(NULL)
{
}

QmitkRigidRegistrationTransformsGUIBase::~QmitkRigidRegistrationTransformsGUIBase()
{
}

void QmitkRigidRegistrationTransformsGUIBase::SetFixedImage(mitk::Image::Pointer fixedImage)
{
  m_FixedImage = fixedImage;
}

void QmitkRigidRegistrationTransformsGUIBase::SetMovingImage(mitk::Image::Pointer movingImage)
{
  m_MovingImage = movingImage;
}
