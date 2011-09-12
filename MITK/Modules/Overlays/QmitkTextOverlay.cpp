/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-01-14 14:20:26 +0100 (Thu, 14 Jan 2010) $
Version:   $Revision: 21047 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "QmitkTextOverlay.h"

#include "mitkProperties.h"
#include "mitkColorProperty.h"
#include "mitkPropertyList.h"

#include <itkCommand.h>

#include <QLayout>


QmitkTextOverlay::QmitkTextOverlay( const char* id ): 
QmitkOverlay(id), m_Widget( NULL ), m_ObserverTag(0)
{
  m_Widget = new QLabel();
  m_Widget->setStyleSheet("");
}

QmitkTextOverlay::~QmitkTextOverlay()
{
  m_Widget->deleteLater();
  m_Widget = NULL;

  m_PropertyList->GetProperty( m_Id )->RemoveObserver(m_ObserverTag);
}

void QmitkTextOverlay::GenerateData( mitk::PropertyList::Pointer pl )
{
  if ( pl.IsNull() )
    return;

  m_PropertyList = pl;

  if ( m_PropertyList.IsNotNull() )
  {
    this->SetupCallback( m_PropertyList->GetProperty( m_Id ) );

    this->GetTextProperties( pl );
    this->SetText();
  }
  else
  {
    MITK_ERROR << "invalid propList";
  }

}

void QmitkTextOverlay::SetText()
{
  std::string text = "";
  if ( m_PropertyList.IsNull() || !m_PropertyList->GetStringProperty( m_Id, text ) )
  {
    MITK_DEBUG << "Property " << m_Id << " could not be found";
  }
  m_Widget->setText( text.c_str() );
  m_Widget->repaint();
}


void QmitkTextOverlay::GetTextProperties( mitk::PropertyList::Pointer pl )
{
  if ( pl.IsNull() )
    return;

  mitk::PropertyList::Pointer propertyList = pl;
  QPalette palette = QPalette();
  QFont font = QFont();

  // get the desired color of the textOverlays
  mitk::ColorProperty::Pointer colorProp = 
    dynamic_cast<mitk::ColorProperty*>( propertyList->GetProperty( "overlay.color" ) );

  if ( colorProp.IsNull() )
  {
    colorProp = mitk::ColorProperty::New( 127.0, 196.0, 232.0 );
  }

  mitk::Color color = colorProp->GetColor();
  palette.setColor( QPalette::Foreground, QColor( color[0],color[1],color[2],255 ) );
  palette.setColor( QPalette::Window, Qt::transparent);
  m_Widget->setPalette( palette );

  // get the desired opacity of the overlays
  //mitk::FloatProperty::Pointer opacityProperty =
  //  dynamic_cast<mitk::FloatProperty*>( propertyList->GetProperty( "overlay.opacity" ) );

  //if ( opacityProperty.IsNull() )
  //{
  //  m_Widget->setWindowOpacity( 1 );
  //} 
  //else
  //{
  //  m_Widget->setWindowOpacity( opacityProperty->GetValue() );
  //}
  
   //set the desired font-size of the overlays
  int fontSize = 0;
  if ( !propertyList->GetIntProperty( "overlay.fontSize", fontSize ) )
  {
    fontSize = 9.5;
  } 
  font.setPointSize( fontSize );

  bool useKerning = false;
  if ( !propertyList->GetBoolProperty( "overlay.kerning", useKerning ) )
  {
    useKerning = true;
  }
  font.setKerning( useKerning );

  std::string fontFamily = "";
  if ( !propertyList->GetStringProperty( "overlay.fontFamily", fontFamily ) )
  {
    fontFamily = "Verdana";
  }
  font.setFamily(  QString(fontFamily.c_str()) );

  m_Widget->setFont( font );
  
}

QLabel* QmitkTextOverlay::GetWidget()
{
  return m_Widget;
}


void QmitkTextOverlay::SetupCallback( mitk::BaseProperty::Pointer prop )
{
  if ( prop.IsNotNull() )
  {
    typedef itk::SimpleMemberCommand< QmitkTextOverlay > MemberCommandType;
    MemberCommandType::Pointer propModifiedCommand;
    propModifiedCommand = MemberCommandType::New();
    propModifiedCommand->SetCallbackFunction( this, &QmitkTextOverlay::SetText );
    m_ObserverTag = prop->AddObserver( itk::ModifiedEvent(), propModifiedCommand );
  }
  else
  {
    MITK_DEBUG << "invalid property";
  }
}

