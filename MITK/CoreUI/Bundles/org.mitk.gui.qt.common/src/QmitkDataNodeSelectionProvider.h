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


#ifndef QMITKDATATREENODESELECTIONPROVIDER_H_
#define QMITKDATATREENODESELECTIONPROVIDER_H_

#include <org_mitk_gui_qt_common_Export.h>

#include <berryQtSelectionProvider.h>
#include <mitkDataNodeSelection.h>

class MITK_QT_COMMON QmitkDataNodeSelectionProvider : public berry::QtSelectionProvider
{
public:

  berryObjectMacro(QmitkDataNodeSelectionProvider)

  QmitkDataNodeSelectionProvider();

  berry::ISelection::ConstPointer GetSelection() const;

protected:

  mitk::DataNodeSelection::ConstPointer GetDataNodeSelection() const;

  virtual void FireSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
};

#endif /* QMITKDATATREENODESELECTIONPROVIDER_H_ */
