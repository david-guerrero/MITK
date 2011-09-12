/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef QMITK_POINTLIST_VIEW_H_INCLUDED
#define QMITK_POINTLIST_VIEW_H_INCLUDED

#include <QListView>
#include <QLabel>
#include "QmitkExtExports.h"

#include "QmitkPointListModel.h"

class QmitkStdMultiWidget;

/*!
* \brief GUI widget for handling mitk::PointSet
*
* Displays all the points in a mitk::PointSet graphically. 
* Reacts automatically to changes in the PointSet's selection status.
* Updates PointSet's selection status when this list's selection changes.
*
* If a QmitkStdMultiWidget is assigned via SetMultiWidget(), the
* crosshair of the QmitkStdMultiWidget is moved to the currently selected
* point.
*
*/
class QmitkExt_EXPORT QmitkPointListView : public QListView
{
  Q_OBJECT

public:

  QmitkPointListView( QWidget* parent = 0 );
  ~QmitkPointListView();

  /// assign a point set for observation
  void SetPointSetNode( mitk::DataNode* pointSetNode );

  /// which point set to work on
  const mitk::PointSet* GetPointSet() const;

  void SetMultiWidget( QmitkStdMultiWidget* multiWidget ); ///< assign a QmitkStdMultiWidget for updating render window crosshair

  QmitkStdMultiWidget* GetMultiWidget() const;  ///< return the QmitkStdMultiWidget that is used for updating render window crosshair

  void SetTimesStep(int i); ///< which time step to display/model

signals:

  void SignalPointSelectionChanged();  ///< this signal is emmitted, if the selection of a point in the pointset is changed


protected slots:

  /// Filtering double click event for editing point coordinates via a dialog
  void OnPointDoubleClicked(const QModelIndex & index);

  /// called when the point set data structure changes
  void OnPointSetSelectionChanged();

  /// called when the selection of the view widget changes
  void OnListViewSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

   /// fade the shown timestep out
  void fadeTimeStepOut();

  /// open ContextMenu
  void ctxMenu(const QPoint &pos);

  /// Turn TimeStep Fading On/Off
  void SetFading(bool onOff);

  /// Delete all points in the list
  void ClearPointList();

  /// delete all points in the list in the current timestep
  void ClearPointListTS();

protected:

  void keyPressEvent( QKeyEvent * e ); ///< react to F2, F3 and DEL keys
  void wheelEvent( QWheelEvent* event); ///< change timestep of the current pointset by mouse wheel
  void fadeTimeStepIn(); ///< fade a label with the currently shown timestep in

protected:

  QmitkPointListModel*    m_PointListModel;

  bool                    m_SelfCall;

  bool                    m_showFading;

  /// used to position the planes on a selected point
  QmitkStdMultiWidget*    m_MultiWidget;

  QLabel*                m_TimeStepFaderLabel;

};
#endif
