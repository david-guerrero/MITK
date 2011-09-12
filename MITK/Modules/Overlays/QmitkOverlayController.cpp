/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-05-12 19:56:03 +0200 (Di, 12 Mai 2009) $
Version:   $Revision: 17179 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "QmitkOverlayController.h"

#include "QmitkRenderWindow.h"
#include "QmitkOverlay.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <algorithm>


QmitkOverlayController::QmitkOverlayController( QmitkRenderWindow* rw, mitk::PropertyList* pl )
: QObject(), m_RenderWindow( rw ), m_PropertyList( pl )
{
  if ( m_RenderWindow == NULL )
  {
    MITK_ERROR << "invalid QmitkRenderWindow";
    return;
  }

  connect( rw, SIGNAL( moved() ), this, SLOT( AdjustOverlayPosition() ) );

  this->InitializeOverlayLayout();
  this->AdjustOverlayPosition();
  this->SetOverlayVisibility( true );

  if ( m_PropertyList.IsNull() )
    m_PropertyList = mitk::PropertyList::New();

}

QmitkOverlayController::~QmitkOverlayController()
{
}

void QmitkOverlayController::InitializeOverlayLayout()
{
  // setup widget for each position
  this->InitializeWidget( QmitkOverlay::top_Left );
  this->InitializeWidget( QmitkOverlay::top_Center );
  this->InitializeWidget( QmitkOverlay::top_Right );
  this->InitializeWidget( QmitkOverlay::middle_Left );
  this->InitializeWidget( QmitkOverlay::middle_Right );
  this->InitializeWidget( QmitkOverlay::bottom_Left );
  this->InitializeWidget( QmitkOverlay::bottom_Center );
  this->InitializeWidget( QmitkOverlay::bottom_Right );

}

void QmitkOverlayController::InitializeWidget( QmitkOverlay::DisplayPosition pos )
{
  // create a new QWidget as Tool & FramelessWindowHint
  m_PositionedOverlays[ pos ] = new QWidget( m_RenderWindow, Qt::Tool | Qt::FramelessWindowHint );
  
  // autoFillBackGround(false) and WA_TranslucentBackground = true are needed to have a translucent background 
  // transparency does NOT work under Win-XP 32-Bit --> paint black background

#if !defined(_WIN32) || defined(_WIN64)
  m_PositionedOverlays[ pos ]->setAttribute( Qt::WA_TranslucentBackground, true );
#else
  m_PositionedOverlays[ pos ]->setStyleSheet( "QWidget { background: black }" );
  m_PositionedOverlays[ pos ]->setAttribute( Qt::WA_TranslucentBackground, false );
#endif

  // X11 specific attributes
  m_PositionedOverlays[ pos ]->setAttribute( Qt::WA_X11NetWmWindowTypeUtility, true );

  // mac-specific attributes:
  // making sure overlays are even visible if RenderWindow does not have the focus (not default for Qt::Tool on mac)
  m_PositionedOverlays[ pos ]->setAttribute( Qt::WA_MacAlwaysShowToolWindow, true );
  // testing something
  m_PositionedOverlays[ pos ]->setAttribute( Qt::WA_MacShowFocusRect, false );

  // overlays should not get the focus
  m_PositionedOverlays[ pos ]->setFocusPolicy( Qt::NoFocus );

  // setting the color of the background to transparent - not sure it's needed after the attributes have been set above
  QPalette p = QPalette();
  p.setColor( QPalette::Window, Qt::transparent );
  m_PositionedOverlays[ pos ]->setPalette( p );

  // setting position-specific properties
  switch ( pos )
  {
  case QmitkOverlay::top_Left : 
    {
      // adding left-aligned top-to-bottom layout
      QVBoxLayout* layout = new QVBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::TopToBottom );
      layout->setAlignment( Qt::AlignLeft );
      m_PositionedOverlays[ pos ]->layout()->setSpacing( 0 );
      break;
    }
  case QmitkOverlay::top_Center : 
    {
      // adding center-aligned top-to-bottom layout
      QVBoxLayout* layout = new QVBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::TopToBottom );
      layout->setAlignment( Qt::AlignCenter );
      layout->setAlignment( Qt::AlignLeft );
      m_PositionedOverlays[ pos ]->layout()->setSpacing( 0 );
      break;
    }
  case QmitkOverlay::top_Right :
    {
      // adding right-aligned top-to-bottom layout
      QVBoxLayout* layout = new QVBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::TopToBottom );
      layout->setAlignment( Qt::AlignRight );
      m_PositionedOverlays[ pos ]->layout()->setSpacing( 0 );
      break;
    }
  case QmitkOverlay::middle_Left : 
    {
      // adding left-aligned left-to-right layout
      QHBoxLayout* layout = new QHBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::LeftToRight );
      layout->setAlignment( Qt::AlignLeft );
      layout->setSpacing( 3 );
      break;

    }
  case QmitkOverlay::middle_Right : 
    {
      // adding right-aligned right-to-left layout
      QHBoxLayout* layout = new QHBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::RightToLeft );
      layout->setAlignment( Qt::AlignRight );
      layout->setSpacing( 3 );
      break;
    }
  case QmitkOverlay::bottom_Left : 
    {
      // adding left-aligned bottom-to-top layout
      QVBoxLayout* layout = new QVBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::BottomToTop );
      layout->setAlignment( Qt::AlignLeft );
      m_PositionedOverlays[ pos ]->layout()->setSpacing( 0 );
      break;
    }
  case QmitkOverlay::bottom_Center : 
    {
      QVBoxLayout* layout = new QVBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::BottomToTop );
      layout->setAlignment( Qt::AlignCenter );
      m_PositionedOverlays[ pos ]->layout()->setSpacing( 0 );
      break;
    }
  case QmitkOverlay::bottom_Right :
    {
      QVBoxLayout* layout = new QVBoxLayout( m_PositionedOverlays[ pos ] );
      layout->setDirection( QBoxLayout::BottomToTop );
      layout->setAlignment( Qt::AlignRight );
      m_PositionedOverlays[ pos ]->layout()->setSpacing( 0 );
      break;
    }
  }
}

void QmitkOverlayController::AdjustOverlayPosition()
{
  // setting position of top-left overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::top_Left ]->isVisible() )
  {
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint(0,0) );
    m_PositionedOverlays[ QmitkOverlay::top_Left ]->move( pos.x(), pos.y() );
  }

  // setting position of top-center overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::top_Center ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::top_Center ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint( m_RenderWindow->size().width()/2, 0 ) ) ;
    widget->move( pos.x() - widget->size().width()/2, pos.y() );
  }

  // setting position of top-right overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::top_Right ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::top_Right ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint( m_RenderWindow->size().width(), 0 ) ) ;
    widget->move( pos.x() - widget->size().width(), pos.y() );
  }

  // setting position of middle-left overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::middle_Left ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::middle_Left ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint( 0, m_RenderWindow->size().height()/2 ) ) ;
    widget->move( pos.x(), pos.y() - widget->size().height()/2 );
  }

  // setting position of middle-right overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::middle_Right ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::middle_Right ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint( m_RenderWindow->size().width(), m_RenderWindow->size().height()/2 ) ) ;
    widget->move( pos.x() - widget->size().width(), pos.y() - widget->size().height()/2 );
  }

  // setting position of bottom-left overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::bottom_Left ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::bottom_Left ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint( 0, m_RenderWindow->size().height() ) ) ;
    widget->move( pos.x(), pos.y() - widget->size().height() );
  }

  // setting position of bottom-center overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::bottom_Center ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::bottom_Center ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint(  m_RenderWindow->size().width()/2, m_RenderWindow->size().height() ) ) ;
    widget->move( pos.x() - widget->size().width()/2, pos.y() - widget->size().height() );
  }

  // setting position of bottom-right overlay-container
  if ( m_PositionedOverlays[ QmitkOverlay::bottom_Right ]->isVisible() )
  {
    QWidget* widget = m_PositionedOverlays[ QmitkOverlay::bottom_Right ];
    QPoint pos = m_RenderWindow->mapToGlobal( QPoint(  m_RenderWindow->size().width(), m_RenderWindow->size().height() ) ) ;
    widget->move( pos.x() - widget->size().width(), pos.y() - widget->size().height() );
  }

} 


void QmitkOverlayController::SetOverlayVisibility( bool visible )
{
  // setting visibility of all registered overlays to 'visible'
  foreach( QmitkOverlay* overlay, m_AllOverlays )
  {
    if ( visible )
    {
      overlay->GetWidget()->show();
    }
    else
    {
      overlay->GetWidget()->hide();
    }
  }
}


void QmitkOverlayController::AddOverlay( QmitkOverlay* overlay )
{
  // if no renderwindow has been set, it's not possible to add overlays...
  if ( m_RenderWindow == NULL )
  {
    MITK_ERROR << "invalid QmitkRenderWindow";
    return;
  }

  if ( overlay != NULL )
  {
    // get desired position and layer of the overlay
    QmitkOverlay::DisplayPosition pos = overlay->GetPosition();

    // concatenate local propertyList and propertyList of the RenderingManager
    // local properties have priority as they are not overwritten if preset in both 
    m_PropertyList->ConcatenatePropertyList( m_RenderWindow->GetRenderer()->GetRenderingManager()->GetPropertyList(), false );

    // add the overlay to the OverlayContainer in the RenderWindow ...
    overlay->GetWidget()->setParent( m_PositionedOverlays[ pos ] );

    // ... and set it up with the correct properties
    this->UpdateOverlayData( overlay );
    
    // add overlay to list of all overlays and correctly put it into the layering
    m_AllOverlays.push_back( overlay );
    this->RestackOverlays( pos );

    // make sure the widget containing the added overlay is shown ...
    m_PositionedOverlays[ pos ]->show();
    // ... and reset the position of the widgets
    this->AdjustOverlayPosition();
  }
}

void QmitkOverlayController::UpdateOverlayData( QmitkOverlay* overlay )
{
  overlay->GenerateData( m_PropertyList );
}

void QmitkOverlayController::RemoveOverlay( QmitkOverlay* overlay )
{
  if ( overlay != NULL )
  {
    // get desired position and layer of the overlay
    QmitkOverlay::DisplayPosition pos = overlay->GetPosition();

    OverlayVector::iterator iter = std::find( m_AllOverlays.begin(), m_AllOverlays.end(), overlay );
    
    if ( iter != m_AllOverlays.end() )
    {
      m_AllOverlays.erase( iter );
      overlay->GetWidget()->setParent( NULL );
      overlay->GetWidget()->hide();

      if ( m_PositionedOverlays[ pos ]->layout()->isEmpty() )
      {
        m_PositionedOverlays[ pos ]->hide();
      }
      else
      {
        this->RestackOverlays( pos );
        // reset the position of the widgets
        this->AdjustOverlayPosition();
      }
    }

    overlay->deleteLater();
  }
}



void QmitkOverlayController::AlignOverlays()
{
  //OverlayVector::iterator overlayIter;

  //for ( overlayIter=m_AllOverlays.begin(); overlayIter!=m_AllOverlays.end(); overlayIter++ )
  //{
  //  int stackLayer = dynamic_cast<QBoxLayout*>( m_PositionedOverlays[ (*overlayIter)->GetPosition() ]->layout() )->isEmpty() ? 0 : layer;
  //  dynamic_cast<QBoxLayout*>( m_PositionedOverlays[ (*overlayIter)->GetPosition() ]->layout() )->addWidget( (*overlayIter)->GetWidget(), stackLayer, Qt::AlignLeft );
  //}
}

void QmitkOverlayController::RestackOverlays( QmitkOverlay::DisplayPosition pos )
{
  OverlayVector::iterator overlayIter;
  QBoxLayout* layout = dynamic_cast<QBoxLayout*>( m_PositionedOverlays[ pos ]->layout() );

  std::sort( m_AllOverlays.begin(), m_AllOverlays.end() );

  for ( overlayIter=m_AllOverlays.begin(); overlayIter!=m_AllOverlays.end(); overlayIter++ )
  {
    // do nothing if the overlay is not in the right position
    if ( (*overlayIter)->GetPosition() != pos )
    {
      continue;
    }

    // determine the desired stacking layer
    // if the overlay-container is empty, simply append the overlay to the list
    // if it's not empty, use the layer of the overlay
    unsigned int layer = (*overlayIter)->GetLayer();
    int stackLayer = 0;
    if ( !layout->isEmpty() )
    {
      stackLayer = layer;
    }
    
    switch ( pos )
    {
      // same alignment for all lefts, ...
    case QmitkOverlay::top_Left : {}
    case QmitkOverlay::middle_Left : {}
    case QmitkOverlay::bottom_Left : 
      {
        layout->insertWidget( stackLayer, (*overlayIter)->GetWidget(), 0, Qt::AlignLeft );    
        break;
      }
      // ... for all centers, ...
    case QmitkOverlay::top_Center : {}
    case QmitkOverlay::bottom_Center :
      {
        layout->insertWidget( stackLayer, (*overlayIter)->GetWidget(), 0, Qt::AlignCenter );
        break;
      }
      // ... and for all rights
    case QmitkOverlay::top_Right : {}
    case QmitkOverlay::middle_Right : {}
    case QmitkOverlay::bottom_Right : 
      {
        layout->insertWidget( stackLayer, (*overlayIter)->GetWidget(), 0, Qt::AlignRight );    
        break;
      }
    }
  }

}

void QmitkOverlayController::UpdateAllOverlays()
{
  foreach( QmitkOverlay* overlay, m_AllOverlays )
  {
    this->UpdateOverlayData( overlay );
  }
}