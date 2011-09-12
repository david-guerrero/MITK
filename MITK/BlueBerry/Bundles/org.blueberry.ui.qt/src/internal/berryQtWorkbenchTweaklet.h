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

#ifndef BERRYQTWORKBENCHTWEAKLET_H_
#define BERRYQTWORKBENCHTWEAKLET_H_

#include <berryWorkbenchTweaklet.h>

#include <org_blueberry_ui_qt_Export.h>

namespace berry {

class BERRY_UI_QT QtWorkbenchTweaklet : public QObject, public WorkbenchTweaklet
{
  Q_OBJECT
  Q_INTERFACES(berry::WorkbenchTweaklet)

public:

  berryObjectMacro(QtWorkbenchTweaklet);

  QtWorkbenchTweaklet();
  QtWorkbenchTweaklet(const QtWorkbenchTweaklet& other);

  Display* CreateDisplay();

  bool IsRunning();

  SmartPointer<WorkbenchWindow> CreateWorkbenchWindow(int number);

  void* CreatePageComposite(void* parent);

  IDialog::Pointer CreateStandardDialog(const std::string& id);

};

} // namespace berry

#endif /*BERRYQTWORKBENCHTWEAKLET_H_*/
