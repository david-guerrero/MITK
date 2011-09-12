/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2007-12-11 14:46:19 +0100 (Di, 11 Dez 2007) $
Version:   $Revision: 6607 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkNrrdDiffusionImageIOFactory.h"
#include "mitkIOAdapter.h"
#include "mitkNrrdDiffusionImageReader.h"

#include "itkVersion.h"

  
namespace mitk
{

NrrdDiffusionImageIOFactory::NrrdDiffusionImageIOFactory()
{
  typedef short DiffusionPixelType;
  typedef itk::VectorImage< DiffusionPixelType, 3 >   DiffusionImageType;
  typedef NrrdDiffusionImageReader<DiffusionPixelType> NrrdDiffVolReaderType;
  this->RegisterOverride("mitkIOAdapter",
                         "mitkNrrdDiffusionImageReader",
                         "mitk Diffusion Image IO",
                         1,
                         itk::CreateObjectFunction<IOAdapter<NrrdDiffVolReaderType> >::New());
}
  
NrrdDiffusionImageIOFactory::~NrrdDiffusionImageIOFactory()
{
}

const char* NrrdDiffusionImageIOFactory::GetITKSourceVersion() const
{
  return ITK_SOURCE_VERSION;
}

const char* NrrdDiffusionImageIOFactory::GetDescription() const
{
  return "NrrdDiffusionImage IO Factory, allows the loading of NRRD DWI data";
}

} // end namespace mitk
