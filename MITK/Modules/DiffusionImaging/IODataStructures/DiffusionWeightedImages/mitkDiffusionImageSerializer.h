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

#ifndef mitkDiffusionImageSerializer_h_included
#define mitkDiffusionImageSerializer_h_included

#include "MitkDiffusionImagingExports.h"
#include "mitkBaseDataSerializer.h"

namespace mitk
{
/**
  \brief Serializes mitk::Surface for mitk::SceneIO
*/
class MitkDiffusionImaging_EXPORT DiffusionImageSerializer : public BaseDataSerializer
{
  public:
    mitkClassMacro( DiffusionImageSerializer, BaseDataSerializer );
    itkNewMacro(Self);
    virtual std::string Serialize();
  protected:
    DiffusionImageSerializer();
    virtual ~DiffusionImageSerializer();
};
} // namespace
#endif
