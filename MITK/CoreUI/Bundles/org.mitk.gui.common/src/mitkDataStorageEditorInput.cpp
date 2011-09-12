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

#include "mitkDataStorageEditorInput.h"

#include <berryPlatform.h>
#include <mitkIDataStorageService.h>

namespace mitk
{

DataStorageEditorInput::DataStorageEditorInput()
{
}

DataStorageEditorInput::DataStorageEditorInput(IDataStorageReference::Pointer ref)
{
  m_DataStorageRef = ref;
}

bool DataStorageEditorInput::Exists() const
{
  return true;
}

std::string DataStorageEditorInput::GetName() const
{
  return "DataStorage Scene";
}

std::string DataStorageEditorInput::GetToolTipText() const
{
  return "";
}

bool DataStorageEditorInput::operator==(const berry::Object* o) const
{
  if (const DataStorageEditorInput* input = dynamic_cast<const DataStorageEditorInput*>(o))
    return this->GetName() == input->GetName();

  return false;
}

IDataStorageReference::Pointer
DataStorageEditorInput::GetDataStorageReference()
{
  if (m_DataStorageRef.IsNull())
  {
    berry::ServiceRegistry& serviceRegistry = berry::Platform::GetServiceRegistry();
    IDataStorageService::Pointer dataService = serviceRegistry.GetServiceById<IDataStorageService>(IDataStorageService::ID);
    if (!dataService) return IDataStorageReference::Pointer(0);
    m_DataStorageRef = dataService->GetDefaultDataStorage();
  }

  return m_DataStorageRef;
}

}
