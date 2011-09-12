/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-07-14 19:11:20 +0200 (Tue, 14 Jul 2009) $
Version:   $Revision: 18127 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <QmitkDnDFrameWidget.h>
#include <QtGui>


// berry Includes
#include <berryPlatform.h>

#include <mitkDataNodeFactory.h>
#include "mitkIDataStorageService.h"
#include "mitkDataStorageEditorInput.h"
#include "mitkRenderingManager.h"

#include "mitkProperties.h"
#include "mitkNodePredicateData.h"
#include "mitkNodePredicateNot.h"
#include "mitkNodePredicateProperty.h"

#include "mitkProgressBar.h"

QmitkDnDFrameWidget::QmitkDnDFrameWidget(QWidget *parent)
: QWidget(parent)
{
  setAcceptDrops(true);
}

void QmitkDnDFrameWidget::dragEnterEvent( QDragEnterEvent *event )
{   // accept drags
  event->accept();
}
void QmitkDnDFrameWidget::dropEvent( QDropEvent * event )
{ //open dragged files

  mitk::IDataStorageService::Pointer service =
    berry::Platform::GetServiceRegistry().GetServiceById<mitk::IDataStorageService>(mitk::IDataStorageService::ID);

  mitk::DataStorage::Pointer ds;
  if (service.IsNotNull())
    ds = service->GetActiveDataStorage()->GetDataStorage();
  else
    return;

  QList<QUrl> fileNames = event->mimeData()->urls();

  bool dsmodified = false;
  for (QList<QUrl>::Iterator fileName = fileNames.begin();
    fileName != fileNames.end(); ++fileName)
  {

    mitk::DataNodeFactory::Pointer nodeReader = mitk::DataNodeFactory::New();
    try
    {
      nodeReader->SetFileName(fileName->toLocalFile().toLatin1().data());
      if(event->dropAction()==Qt::LinkAction)
        nodeReader->SetImageSerie(true);
      nodeReader->Update();
      for ( unsigned int i = 0 ; i < nodeReader->GetNumberOfOutputs( ); ++i )
      {
        mitk::DataNode::Pointer node;
        node = nodeReader->GetOutput(i);
        if ( node->GetData() != NULL )
        {
          ds->Add(node);
          dsmodified = true;
        }
      }
    }
    catch(...)
    {

    }

  }

  if(dsmodified)
  {
    // get all nodes that have not set "includeInBoundingBox" to false
    mitk::NodePredicateNot::Pointer pred
      = mitk::NodePredicateNot::New(mitk::NodePredicateProperty::New("includeInBoundingBox"
      , mitk::BoolProperty::New(false)));

    mitk::DataStorage::SetOfObjects::ConstPointer rs = ds->GetSubset(pred);
    // calculate bounding geometry of these nodes
    mitk::TimeSlicedGeometry::Pointer bounds = ds->ComputeBoundingGeometry3D(rs);
    // initialize the views to the bounding geometry
    mitk::RenderingManager::GetInstance()->InitializeViews(bounds);
  }
}