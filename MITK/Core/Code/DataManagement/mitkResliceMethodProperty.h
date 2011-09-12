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

#ifndef __MITKRESLICEMETHODENUMPROPERTY_H
#define __MITKRESLICEMETHODENUMPROPERTY_H

#include "mitkEnumerationProperty.h"

namespace mitk
{

/**
 * Encapsulates the thick slices method enumeration
 */
class MITK_CORE_EXPORT ResliceMethodProperty : public EnumerationProperty
{
public:

  mitkClassMacro( ResliceMethodProperty, EnumerationProperty );
  itkNewMacro(ResliceMethodProperty);
  mitkNewMacro1Param(ResliceMethodProperty, const IdType&); 
  mitkNewMacro1Param(ResliceMethodProperty, const std::string&);  

  virtual BaseProperty& operator=(const BaseProperty& other) { return Superclass::operator=(other); }
  
protected:
  
  ResliceMethodProperty( );  
  ResliceMethodProperty( const IdType& value );  
  ResliceMethodProperty( const std::string& value );
  
  void AddThickSlicesTypes();
};

} // end of namespace mitk

#endif //_MITK_VTK_SCALARMODE_PROPERTY__H_


