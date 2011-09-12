/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 11215 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef UNSTRUCTURED_GRID_WRITERFACTORY_H_HEADER_INCLUDED
#define UNSTRUCTURED_GRID_WRITERFACTORY_H_HEADER_INCLUDED

#include "itkObjectFactoryBase.h"
#include "MitkExtExports.h"
#include "mitkBaseData.h"

namespace mitk
{

class MitkExt_EXPORT UnstructuredGridVtkWriterFactory : public itk::ObjectFactoryBase
{
public:

  mitkClassMacro( mitk::UnstructuredGridVtkWriterFactory, itk::ObjectFactoryBase )

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    static bool IsRegistered = false;
    if ( !IsRegistered )
    {
      UnstructuredGridVtkWriterFactory::Pointer ugVtkWriterFactory = UnstructuredGridVtkWriterFactory::New();
      ObjectFactoryBase::RegisterFactory( ugVtkWriterFactory );
      IsRegistered = true;
    }
  }

protected:
  UnstructuredGridVtkWriterFactory();
  ~UnstructuredGridVtkWriterFactory();

private:
  UnstructuredGridVtkWriterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace mitk

#endif // UNSTRUCTURED_GRID_WRITERFACTORY_H_HEADER_INCLUDED



