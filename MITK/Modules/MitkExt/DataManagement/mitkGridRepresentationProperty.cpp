/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision:  $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <vtkProperty.h>
#include "mitkGridRepresentationProperty.h"


mitk::GridRepresentationProperty::GridRepresentationProperty( )
{
  AddRepresentationTypes();
  SetValue( WIREFRAME );
}


mitk::GridRepresentationProperty::GridRepresentationProperty( const IdType& value )
{
  AddRepresentationTypes();
  if ( IsValidEnumerationValue( value ) )
  {
    SetValue( value );
  }
  else
  {
    SetValue( WIREFRAME );
  }
}


mitk::GridRepresentationProperty::GridRepresentationProperty( const std::string& value )
{
  AddRepresentationTypes();
  if ( IsValidEnumerationValue( value ) )
  {
    SetValue( value );
  }
  else
  {
    SetValue( WIREFRAME );
  }
}


void mitk::GridRepresentationProperty::SetRepresentationToPoints()
{
  SetValue( POINTS );
}


void mitk::GridRepresentationProperty::SetRepresentationToWireframe()
{
  SetValue( WIREFRAME );
}


void mitk::GridRepresentationProperty::SetRepresentationToSurface()
{
  SetValue( SURFACE );
}

void mitk::GridRepresentationProperty::SetRepresentationToWireframeSurface()
{
  //SetValue( WIREFRAME_SURFACE );
}

void mitk::GridRepresentationProperty::AddRepresentationTypes()
{
  AddEnum( "Points", POINTS );
  AddEnum( "Wireframe", WIREFRAME );
  AddEnum( "Surface", SURFACE );
  //AddEnum( "WireframeSurface", WIREFRAME_SURFACE );
}


bool mitk::GridRepresentationProperty::AddEnum( const std::string& name, const IdType& id )
{
  return Superclass::AddEnum( name, id );  
}
