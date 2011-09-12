/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2007-12-11 14:46:19 +0100 (Di, 11 Dez 2007) $
Version:   $Revision: 13129 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#include "mitkResliceMethodProperty.h"

mitk::ResliceMethodProperty::ResliceMethodProperty( )
{
  AddThickSlicesTypes();
  SetValue( (IdType)0 );
}

mitk::ResliceMethodProperty::ResliceMethodProperty( const IdType& value )
{
  AddThickSlicesTypes();
  if ( IsValidEnumerationValue( value ) )
    SetValue( value );
 }

mitk::ResliceMethodProperty::ResliceMethodProperty( const std::string& value )
{
  AddThickSlicesTypes();
  if ( IsValidEnumerationValue( value ) )
    SetValue( value );
}

void mitk::ResliceMethodProperty::AddThickSlicesTypes()
{  
  AddEnum( "disabled", (IdType) 0 );
  AddEnum( "mip", (IdType) 1 );
  AddEnum( "sum", (IdType) 2 );
  AddEnum( "weighted", (IdType) 3 );
}

