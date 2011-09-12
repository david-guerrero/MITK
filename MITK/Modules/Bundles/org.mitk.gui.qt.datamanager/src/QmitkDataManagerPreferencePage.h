/*=========================================================================

 Program:   Medical Imaging & Interaction Toolkit
 Language:  C++
 Date:      $Date: 2009-02-10 14:14:32 +0100 (Di, 10 Feb 2009) $
 Version:   $Revision: 16224 $

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/


#ifndef QMITKDATAMANAGERPREFERENCEPAGE_H_
#define QMITKDATAMANAGERPREFERENCEPAGE_H_

#include "berryIQtPreferencePage.h"
#include <org_mitk_gui_qt_datamanager_Export.h>
#include <berryIPreferences.h>

class QWidget;
class QCheckBox;

struct MITK_QT_DATAMANAGER QmitkDataManagerPreferencePage : public QObject, public berry::IQtPreferencePage
{
  Q_OBJECT
  Q_INTERFACES(berry::IPreferencePage)

public:
  QmitkDataManagerPreferencePage();
  QmitkDataManagerPreferencePage(const QmitkDataManagerPreferencePage& other);

  void Init(berry::IWorkbench::Pointer workbench);

  void CreateQtControl(QWidget* widget);

  QWidget* GetQtControl() const;

  ///
  /// \see IPreferencePage::PerformOk()
  ///
  virtual bool PerformOk();

  ///
  /// \see IPreferencePage::PerformCancel()
  ///
  virtual void PerformCancel();

  ///
  /// \see IPreferencePage::Update()
  ///
  virtual void Update();

protected:
  QWidget* m_MainControl;  
  QCheckBox* m_EnableSingleEditing;
  QCheckBox* m_PlaceNewNodesOnTop;
  QCheckBox* m_ShowHelperObjects;
  QCheckBox* m_ShowNodesContainingNoData;
  QCheckBox* m_UseSurfaceDecimation;
  berry::IPreferences::Pointer m_DataManagerPreferencesNode;
};

#endif /* QMITKDATAMANAGERPREFERENCEPAGE_H_ */
