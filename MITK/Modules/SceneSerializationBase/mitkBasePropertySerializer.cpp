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

#include "mitkBasePropertySerializer.h"

mitk::BasePropertySerializer::BasePropertySerializer()
{
}

mitk::BasePropertySerializer::~BasePropertySerializer()
{
}

TiXmlElement* mitk::BasePropertySerializer::Serialize()
{
  MITK_INFO << this->GetNameOfClass() 
           << " is asked to serialize an object " << (const void*) this->m_Property;

  return NULL; 
}

mitk::BaseProperty::Pointer mitk::BasePropertySerializer::Deserialize(TiXmlElement*)
{
  MITK_ERROR << this->GetNameOfClass() << " is asked to deserialize an object but has no implementation. This is bad.";
  return NULL; 
}
