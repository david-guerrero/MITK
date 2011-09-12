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

#include <Poco/ClassLibrary.h>


#include <berryIBundleActivator.h>
#include "src/internal/mitkPeripheralConfigActivator.h"
#include "src/internal/QmitkMITKPeripheralConfigView.h"

POCO_BEGIN_NAMED_MANIFEST(berryIViewPart, berry::IViewPart)
  POCO_EXPORT_CLASS(::QmitkMITKPeripheralConfigView)
POCO_END_MANIFEST

POCO_BEGIN_MANIFEST(berry::IBundleActivator)
  POCO_EXPORT_CLASS(mitk::PeripheralConfigActivator)
POCO_END_MANIFEST