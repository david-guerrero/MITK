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

#include "berryQtPluginActivator.h"

#include "berryQtStyleManager.h"

#include "berryQtDnDTweaklet.h"
#include "berryQtImageTweaklet.h"
#include "berryQtMessageDialogTweaklet.h"
#include "berryQtWorkbenchTweaklet.h"
#include "berryQtWorkbenchPageTweaklet.h"
#include "berryQtWidgetsTweaklet.h"
#include "berryQtStylePreferencePage.h"
#include "defaultpresentation/berryQtWorkbenchPresentationFactory.h"

namespace berry {

QtPluginActivator::QtPluginActivator()
{

}

QtPluginActivator::~QtPluginActivator()
{

}

void
QtPluginActivator::start(ctkPluginContext* context)
{
  AbstractUICTKPlugin::start(context);

  BERRY_REGISTER_EXTENSION_CLASS(QtDnDTweaklet, context)
  BERRY_REGISTER_EXTENSION_CLASS(QtImageTweaklet, context);
  BERRY_REGISTER_EXTENSION_CLASS(QtMessageDialogTweaklet, context);
  BERRY_REGISTER_EXTENSION_CLASS(QtWidgetsTweaklet, context)
  BERRY_REGISTER_EXTENSION_CLASS(QtWorkbenchTweaklet, context)
  BERRY_REGISTER_EXTENSION_CLASS(QtWorkbenchPageTweaklet, context)
  BERRY_REGISTER_EXTENSION_CLASS(QtWorkbenchPresentationFactory, context)

  BERRY_REGISTER_EXTENSION_CLASS(QtStylePreferencePage, context)

  QtStyleManager* manager = new QtStyleManager();
  styleManager = IQtStyleManager::Pointer(manager);
  context->registerService<berry::IQtStyleManager>(manager);

}

void QtPluginActivator::stop(ctkPluginContext* context)
{
  styleManager = 0;

  AbstractUICTKPlugin::stop(context);
}

}

Q_EXPORT_PLUGIN2(org_blueberry_ui_qt, berry::QtPluginActivator)
