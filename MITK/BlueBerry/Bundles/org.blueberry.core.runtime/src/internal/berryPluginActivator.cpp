/*=========================================================================

 Program:   BlueBerry Platform
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


#include "berryPluginActivator.h"

#include <QtPlugin>

namespace berry {

org_blueberry_core_runtime_Activator::org_blueberry_core_runtime_Activator()
{
}

void org_blueberry_core_runtime_Activator::start(ctkPluginContext* context)
{
  m_PreferencesService = new PreferencesService(context->getDataFile("").absolutePath().toStdString());
  m_PrefServiceReg = context->registerService<IPreferencesService>(m_PreferencesService.GetPointer());
}

void org_blueberry_core_runtime_Activator::stop(ctkPluginContext* context)
{
  m_PrefServiceReg.unregister();
  m_PreferencesService->ShutDown();
  m_PreferencesService = 0;
  m_PrefServiceReg = 0;
}

}

Q_EXPORT_PLUGIN2(org_blueberry_core_runtime, berry::org_blueberry_core_runtime_Activator)
