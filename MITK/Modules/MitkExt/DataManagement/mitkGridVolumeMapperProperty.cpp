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
#include "mitkGridVolumeMapperProperty.h"

mitk::GridVolumeMapperProperty::GridVolumeMapperProperty( )
{
  AddRepresentationTypes();
  SetValue( PT );
}


mitk::GridVolumeMapperProperty::GridVolumeMapperProperty( const IdType& value )
{
  AddRepresentationTypes();
  if ( IsValidEnumerationValue( value ) )
  {
    SetValue( value );
  }
  else
  {
    SetValue( PT );
  }
}


mitk::GridVolumeMapperProperty::GridVolumeMapperProperty( const std::string& value )
{
  AddRepresentationTypes();
  if ( IsValidEnumerationValue( value ) )
  {
    SetValue( value );
  }
  else
  {
    SetValue( PT );
  }
}


void mitk::GridVolumeMapperProperty::SetVolumeMapperToPT()
{
  SetValue( PT );
}


void mitk::GridVolumeMapperProperty::SetVolumeMapperToZSweep()
{
  SetValue( ZSWEEP );
}


void mitk::GridVolumeMapperProperty::SetVolumeMapperToRayCast()
{
  SetValue( RAYCAST );
}


void mitk::GridVolumeMapperProperty::AddRepresentationTypes()
{
  AddEnum( "Ray Cast", RAYCAST );
  AddEnum( "Projected Tetrahedra", PT );
  AddEnum( "ZSweep", ZSWEEP );
}


bool mitk::GridVolumeMapperProperty::AddEnum( const std::string& name, const IdType& id )
{
  return Superclass::AddEnum( name, id );  
}
