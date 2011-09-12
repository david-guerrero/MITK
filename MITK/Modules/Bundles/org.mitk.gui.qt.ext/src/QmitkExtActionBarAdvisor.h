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

#ifndef QMITKEXTACTIONBARADVISOR_H_
#define QMITKEXTACTIONBARADVISOR_H_

#include <berryActionBarAdvisor.h>

#include <org_mitk_gui_qt_ext_Export.h>

class MITK_QT_COMMON_EXT_EXPORT QmitkExtActionBarAdvisor : public berry::ActionBarAdvisor
{
public:

  QmitkExtActionBarAdvisor(berry::IActionBarConfigurer::Pointer configurer);

protected:

  void FillMenuBar(void* menuBar);
};

#endif /*QMITKEXTACTIONBARADVISOR_H_*/
