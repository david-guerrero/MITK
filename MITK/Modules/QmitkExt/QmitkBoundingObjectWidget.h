/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2009-05-28 17:19:30 +0200 (Do, 28 Mai 2009) $
Version:   $Revision: 17495 $ 

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _QMITKBOUNDINGOBJECTWIDGET_H_INCLUDED
#define _QMITKBOUNDINGOBJECTWIDGET_H_INCLUDED

//includes
#include <qwidget.h>
#include "QmitkExtExports.h"
#include <QComboBox>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkBoundingObject.h>
#include <mitkBoundingObjectGroup.h>
#include <QTreeWidget>
#include <QPushButton>

class QmitkExt_EXPORT QmitkBoundingObjectWidget : public QWidget
{
  Q_OBJECT

public:
  QmitkBoundingObjectWidget (QWidget* parent = 0, Qt::WindowFlags f = 0 );
  ~QmitkBoundingObjectWidget ();

  void SetDataStorage(mitk::DataStorage* dataStorage);
  mitk::DataStorage* GetDataStorage();
  mitk::BoundingObject::Pointer GetSelectedBoundingObject();
  mitk::DataNode::Pointer GetSelectedBoundingObjectNode();

  mitk::DataNode::Pointer GetAllBoundingObjects();

  void setEnabled(bool flag);

  void OnBoundingObjectModified( const itk::EventObject& e);

  void RemoveAllItems();

signals:
  //signal when bo has changed
  void BoundingObjectsChanged();

protected slots:
    void CreateBoundingObject(int type);
    void OnDelButtonClicked();
 /*   void OnLoadButtonClicked();
    void OnSaveButtonClicked();*/
    void SelectionChanged();
    void OnItemDoubleClicked(QTreeWidgetItem* item, int col);
    void OnItemDataChanged(QTreeWidgetItem* item, int col);
    
protected:

  void AddItem(mitk::DataNode* node);
  void RemoveItem();

  mitk::DataStorage* m_DataStorage;
  QTreeWidget* m_TreeWidget;
  QComboBox* m_addComboBox;
  QPushButton* m_DelButton;
  QPushButton* m_SaveButton;
  QPushButton* m_LoadButton;
  QTreeWidgetItem* m_lastSelectedItem;
  unsigned long m_lastAffineObserver;

  typedef std::map< QTreeWidgetItem*, mitk::DataNode* > ItemNodeMapType;
  ItemNodeMapType m_ItemNodeMap;

  unsigned int m_BoundingObjectCounter;

  enum BoundingObjectType{
    CUBOID,
    CONE,
    ELLIPSOID,
    CYLINDER,
  };

};
#endif 
