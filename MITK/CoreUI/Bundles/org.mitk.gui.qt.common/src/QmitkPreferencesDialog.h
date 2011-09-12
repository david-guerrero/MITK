/*=========================================================================

 Program:   BlueBerry Platform
 Language:  C++
 Date:      $Date: 2009-02-22 15:57:29 +0100 (So, 22 Feb 2009) $
 Version:   $Revision: 16375 $

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/


#ifndef BERRYQMITKPREFERENCESDIALOG_H_
#define BERRYQMITKPREFERENCESDIALOG_H_

#include <berryIBerryPreferencesService.h>
#include <berryIQtPreferencePage.h>
#include <berryIConfigurationElement.h>
#include <org_mitk_gui_qt_common_Export.h>

#include <QDialog>

class QVBoxLayout;
class QGridLayout;
class QHBoxLayout;
class QLineEdit;
class QTreeWidget;
class QLabel;
class QPushButton;
class QSplitter;
class QTreeWidgetItem;
class QStackedWidget;

#include <map>
#include <vector>

class MITK_QT_COMMON QmitkPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    QmitkPreferencesDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~QmitkPreferencesDialog();    
    void SetSelectedPage(const std::string& id);

protected slots:
  void OnImportButtonClicked(bool triggered=false);
  void OnExportButtonClicked(bool triggered=false);
  void OnApplyButtonClicked(bool triggered=false);
  void OnCloseButtonClicked(bool triggered=false);
  void OnKeywordTextChanged(const QString & s);
  void OnKeywordEditingFinished();
  void OnPreferencesTreeItemSelectionChanged();
  
protected:
  ///
  /// Just a stub class for holding information on prefpages (metadata)
  ///
  struct PrefPage
  {
    PrefPage(std::string _id, std::string _name, std::string _category
      , std::string _className, std::string _keywords, berry::IConfigurationElement::Pointer _confElem);
    bool operator==(const PrefPage& other);
    bool operator<(const PrefPage& other);

    std::string id;
    std::string name;
    std::string category;
    std::string className;
    std::string keywords;
    berry::IQtPreferencePage* prefPage;
    berry::IConfigurationElement::Pointer confElem;
    QTreeWidgetItem* treeWidgetItem;
  };

  bool eventFilter(QObject *obj, QEvent *event);
  void UpdateTree();

  ///
  /// Saves all preferencepages.
  ///
  void SavePreferences();

  ///
  /// The Preferences Service to retrieve and store preferences.
  ///
  berry::IPreferencesService::WeakPtr m_PreferencesService;

  ///
  /// Saves all treewidgetitems in a map, the key is the id of the preferencepage.
  ///
  std::vector<PrefPage> m_PrefPages;
  std::size_t m_CurrentPage;

  //# QWidgets
  QGridLayout* m_Layout;
  QSplitter* m_Splitter;

  QWidget* m_LeftPanel;
  QGridLayout* m_LeftLayout;
  QLineEdit* m_Keyword;
  QTreeWidget* m_PreferencesTree;

  QWidget* m_RightPanel;
  QGridLayout* m_RightLayout;
  QLabel* m_Headline;
  QStackedWidget* m_PreferencesPanel;

  QPushButton* m_ImportButton;
  QPushButton* m_ExportButton;
  QPushButton* m_ApplyButton;
  QPushButton* m_CloseButton;

};

#endif /* BERRYQMITKPREFERENCESDIALOG_H_ */
