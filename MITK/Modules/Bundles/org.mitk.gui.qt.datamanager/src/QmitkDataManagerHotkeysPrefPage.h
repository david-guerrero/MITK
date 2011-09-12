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


#ifndef QMITKDATAMANAGERHOTKEYSPREFPAGE_H_
#define QMITKDATAMANAGERHOTKEYSPREFPAGE_H_

#include "berryIQtPreferencePage.h"
#include <org_mitk_gui_qt_datamanager_Export.h>

#include <map>
#include <QWidget>

class QmitkHotkeyLineEdit;

struct MITK_QT_DATAMANAGER QmitkDataManagerHotkeysPrefPage : public QObject, public berry::IQtPreferencePage
{
  Q_OBJECT
  Q_INTERFACES(berry::IPreferencePage)

public:
  QmitkDataManagerHotkeysPrefPage();
  QmitkDataManagerHotkeysPrefPage(const QmitkDataManagerHotkeysPrefPage& other);

  void Init(berry::IWorkbench::Pointer workbench);

  void CreateQtControl(QWidget* parent);

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
  ///
  /// The node from which the properties are taken (will be catched from the preferences service in ctor)
  ///
  berry::IPreferences::WeakPtr m_DataManagerHotkeysPreferencesNode;

  ///
  /// Maps a label to hotkey lineedit, e.g. "Toggle Visibility of selected nodes" => QmitkHotkeyLineEdit
  ///
  std::map<QString, QmitkHotkeyLineEdit*> m_HotkeyEditors;

  QWidget* m_MainControl;
};

#endif /* QMITKDATAMANAGERHOTKEYSPREFPAGE_H_ */
