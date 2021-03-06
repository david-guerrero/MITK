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

#ifndef mitkColorPropertySerializer_h_included
#define mitkColorPropertySerializer_h_included

#include "mitkBasePropertySerializer.h"

#include "mitkColorProperty.h"

#include "SceneSerializationBaseExports.h"

namespace mitk
{

class SceneSerializationBase_EXPORT ColorPropertySerializer : public BasePropertySerializer
{
  public:
    
    mitkClassMacro( ColorPropertySerializer, BasePropertySerializer );
    itkNewMacro(Self);

    virtual TiXmlElement* Serialize()
    {
      if (const ColorProperty* prop = dynamic_cast<const ColorProperty*>(m_Property.GetPointer()))
      {
        TiXmlElement* element = new TiXmlElement("color");
        Color color = prop->GetValue();
        element->SetDoubleAttribute("r", color[0]);
        element->SetDoubleAttribute("g", color[1]);
        element->SetDoubleAttribute("b", color[2]);
        return element;
      }
      else return NULL;
    }

    virtual BaseProperty::Pointer Deserialize(TiXmlElement* element)
    {
      if (!element) return NULL;

      Color c;
      if ( element->QueryFloatAttribute( "r", &c[0] ) != TIXML_SUCCESS ) return NULL;
      if ( element->QueryFloatAttribute( "g", &c[1] ) != TIXML_SUCCESS ) return NULL;
      if ( element->QueryFloatAttribute( "b", &c[2] ) != TIXML_SUCCESS ) return NULL;

      return ColorProperty::New( c ).GetPointer();
    }

  protected:

    ColorPropertySerializer() {}
    virtual ~ColorPropertySerializer() {}
};

} // namespace

// important to put this into the GLOBAL namespace (because it starts with 'namespace mitk')
MITK_REGISTER_SERIALIZER(ColorPropertySerializer);

#endif

