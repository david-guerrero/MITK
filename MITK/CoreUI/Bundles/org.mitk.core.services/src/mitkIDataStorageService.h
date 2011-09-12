/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef MITKIDATASTORAGESERVICE_H_
#define MITKIDATASTORAGESERVICE_H_

#include <berryService.h>

#include <org_mitk_core_services_Export.h>
#include "mitkIDataStorageReference.h"

#include <string>

#include <QtPlugin>

namespace mitk
{

struct MITK_CORE_SERVICES_PLUGIN IDataStorageService : public berry::Service
{
  berryInterfaceMacro(IDataStorageService, berry);

  static const std::string ID;

  virtual IDataStorageReference::Pointer CreateDataStorage(const std::string& label) = 0;
  virtual std::vector<IDataStorageReference::Pointer> GetDataStorageReferences() const = 0;

  virtual IDataStorageReference::Pointer GetDefaultDataStorage() const = 0;

  virtual IDataStorageReference::Pointer GetActiveDataStorage() const = 0;
  virtual void SetActiveDataStorage(IDataStorageReference::Pointer dataStorageRef) = 0;
};

}

Q_DECLARE_INTERFACE(mitk::IDataStorageService, "org.mitk.service.IDataStorageService")

#endif /*MITKIDATASTORAGESERVICE_H_*/
