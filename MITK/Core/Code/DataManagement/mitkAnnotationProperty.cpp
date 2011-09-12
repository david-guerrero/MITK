/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2008-04-14 19:45:53 +0200 (Mo, 14 Apr 2008) $
Version:   $Revision: 14081 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "mitkAnnotationProperty.h"

mitk::AnnotationProperty::AnnotationProperty()
{
}


mitk::AnnotationProperty::AnnotationProperty( 
  const char *label, const Point3D &position ) 
: m_Label( "" ),
  m_Position( position )
{
  if ( label != NULL )
  {
    m_Label = label;
  }
}


mitk::AnnotationProperty::AnnotationProperty( 
  const std::string &label, const Point3D &position )
: m_Label( label ),
  m_Position( position )
{
}


mitk::AnnotationProperty::AnnotationProperty( 
  const char *label, ScalarType x, ScalarType y, ScalarType z ) 
: m_Label( "" )
{
  if ( label != NULL )
  {
    m_Label = label;
  }

  m_Position[0] = x;
  m_Position[1] = y;
  m_Position[2] = z;
}


mitk::AnnotationProperty::AnnotationProperty( 
  const std::string &label, ScalarType x, ScalarType y, ScalarType z )
: m_Label( label )
{
  m_Position[0] = x;
  m_Position[1] = y;
  m_Position[2] = z;
}


const mitk::Point3D &mitk::AnnotationProperty::GetPosition() const
{
  return m_Position;
}


void mitk::AnnotationProperty::SetPosition( const mitk::Point3D &position )
{
  m_Position = position;
}


bool mitk::AnnotationProperty::operator==( const BaseProperty &property ) const 
{  
  const Self *other = dynamic_cast< const Self * >( &property );

  if ( other == NULL ) return false;

  return ( (other->m_Label == m_Label )
    && (other->m_Position == m_Position ) );
}


std::string mitk::AnnotationProperty::GetValueAsString() const 
{
  std::stringstream myStr;

  myStr << this->GetLabel() << this->GetPosition();
  return myStr.str(); 
}
