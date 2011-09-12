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

#ifndef mitkBoolPropertySerializer_h_included
#define mitkBoolPropertySerializer_h_included

#include "mitkBasePropertySerializer.h"

#include "mitkProperties.h"

#include "SceneSerializationBaseExports.h"

namespace mitk
{

class SceneSerializationBase_EXPORT BoolPropertySerializer : public BasePropertySerializer
{
  public:
    
    mitkClassMacro( BoolPropertySerializer, BasePropertySerializer );
    itkNewMacro(Self);

    virtual TiXmlElement* Serialize()
    {
      if (const BoolProperty* prop = dynamic_cast<const BoolProperty*>(m_Property.GetPointer()))
      {
        TiXmlElement* element = new TiXmlElement("bool");
        if (prop->GetValue() == true)
        {
          element->SetAttribute("value", "true");
        }
        else
        {
          element->SetAttribute("value", "false");
        }
        return element;
      }
      else return NULL;
    }

    virtual BaseProperty::Pointer Deserialize(TiXmlElement* element)
    {
      if (!element) return NULL;
      return BoolProperty::New( std::string(element->Attribute("value")) == "true" ).GetPointer();
    }

  protected:

    BoolPropertySerializer() {}
    virtual ~BoolPropertySerializer() {}
};

} // namespace

// important to put this into the GLOBAL namespace (because it starts with 'namespace mitk')
MITK_REGISTER_SERIALIZER(BoolPropertySerializer);

#endif

