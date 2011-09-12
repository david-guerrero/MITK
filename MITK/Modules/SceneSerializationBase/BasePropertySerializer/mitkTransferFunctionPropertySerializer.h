/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-10-23 11:12:48 +0200 (Fr, 23 Okt 2009) $
Version:   $Revision: 1.12 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#include "mitkTransferFunctionProperty.h"
#include "mitkBasePropertySerializer.h"

#include "SceneSerializationBaseExports.h"

namespace mitk
{
  class SceneSerializationBase_EXPORT TransferFunctionPropertySerializer : public BasePropertySerializer
  {
    public:
      
      mitkClassMacro( TransferFunctionPropertySerializer, BasePropertySerializer );
      itkNewMacro(Self);

      virtual TiXmlElement* Serialize();
      virtual BaseProperty::Pointer Deserialize(TiXmlElement* element);

      static bool SerializeTransferFunction( const char * filename, TransferFunction::Pointer tf );
      static TransferFunction::Pointer DeserializeTransferFunction( const char *filePath );
    
    protected:
    
      TransferFunctionPropertySerializer();
      virtual ~TransferFunctionPropertySerializer();
  };
} // namespace

