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

#include "QmitkWorkbenchWindowAdvisor.h"
#include "QmitkActionBarAdvisor.h"


#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QStatusBar>

#include <berryPlatform.h>
#include <berryPlatformUI.h>
#include <berryIWorkbenchWindow.h>
#include <berryIPreferencesService.h>

#include <internal/berryQtShowViewAction.h>

#include <QmitkFileOpenAction.h>
#include <QmitkFileExitAction.h>
#include <QmitkStatusBar.h>
#include <QmitkProgressBar.h>
#include <QmitkMemoryUsageIndicatorView.h>

#include <QToolBar>

QmitkWorkbenchWindowAdvisor::QmitkWorkbenchWindowAdvisor(berry::IWorkbenchWindowConfigurer::Pointer configurer)
: berry::WorkbenchWindowAdvisor(configurer)
{

}

berry::ActionBarAdvisor::Pointer
QmitkWorkbenchWindowAdvisor::CreateActionBarAdvisor(
  berry::IActionBarConfigurer::Pointer configurer)
{
  berry::ActionBarAdvisor::Pointer actionBarAdvisor(new QmitkActionBarAdvisor(configurer));
  return actionBarAdvisor;
}

void QmitkWorkbenchWindowAdvisor::PostWindowCreate()
{
  // very bad hack...
  berry::IWorkbenchWindow::Pointer window = this->GetWindowConfigurer()->GetWindow();
  QMainWindow* mainWindow = static_cast<QMainWindow*>(window->GetShell()->GetControl());

  QMenuBar* menuBar = mainWindow->menuBar();

  QMenu* fileMenu = menuBar->addMenu("&File");

  fileMenu->addAction(new QmitkFileOpenAction(window));
  fileMenu->addSeparator();
  fileMenu->addAction(new QmitkFileExitAction(window));

  berry::IViewRegistry* viewRegistry = berry::PlatformUI::GetWorkbench()->GetViewRegistry();
  const std::vector<berry::IViewDescriptor::Pointer>& viewDescriptors = viewRegistry->GetViews();

  QMenu* viewMenu = menuBar->addMenu("Show &View");

  // sort elements (converting vector to map...)
  std::vector<berry::IViewDescriptor::Pointer>::const_iterator iter;
  std::map<std::string, berry::IViewDescriptor::Pointer> VDMap;

  for (iter = viewDescriptors.begin(); iter != viewDescriptors.end(); ++iter)
  {
    if ((*iter)->GetId() == "org.blueberry.ui.internal.introview")
      continue;
    std::pair<std::string, berry::IViewDescriptor::Pointer> p((*iter)->GetLabel(), (*iter)); 
    VDMap.insert(p);
  }

  QToolBar* qToolbar = new QToolBar;
  
  std::map<std::string, berry::IViewDescriptor::Pointer>::const_iterator MapIter;
  for (MapIter = VDMap.begin(); MapIter != VDMap.end(); ++MapIter)
  {
    berry::QtShowViewAction* viewAction = new berry::QtShowViewAction(window, (*MapIter).second);
    //m_ViewActions.push_back(viewAction);
    viewMenu->addAction(viewAction);
    qToolbar->addAction(viewAction);
  }
  
  mainWindow->addToolBar(qToolbar);

  QStatusBar* qStatusBar = new QStatusBar();

  //creating a QmitkStatusBar for Output on the QStatusBar and connecting it with the MainStatusBar
  QmitkStatusBar *statusBar = new QmitkStatusBar(qStatusBar);
  //disabling the SizeGrip in the lower right corner
  statusBar->SetSizeGripEnabled(false);

  QmitkProgressBar *progBar = new QmitkProgressBar();
  qStatusBar->addPermanentWidget(progBar, 0);
  progBar->hide();
  mainWindow->setStatusBar(qStatusBar);

  QmitkMemoryUsageIndicatorView* memoryIndicator = new QmitkMemoryUsageIndicatorView();
  qStatusBar->addPermanentWidget(memoryIndicator, 0);
}
