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

#ifndef mitkPointSetSerializer_h_included
#define mitkPointSetSerializer_h_included

#include "SceneSerializationExports.h"
#include "mitkBaseDataSerializer.h"

namespace mitk
{
/**
  \brief Serializes mitk::Surface for mitk::SceneIO
*/
class SceneSerialization_EXPORT PointSetSerializer : public BaseDataSerializer
{
  public:
    mitkClassMacro( PointSetSerializer, BaseDataSerializer );
    itkNewMacro(Self);
    virtual std::string Serialize();
  protected:
    PointSetSerializer();
    virtual ~PointSetSerializer();
};
} // namespace
#endif
