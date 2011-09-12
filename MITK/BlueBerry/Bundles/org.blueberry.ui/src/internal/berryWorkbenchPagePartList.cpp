/*=========================================================================

 Program:   BlueBerry Platform
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

#include "berryWorkbenchPagePartList.h"
#include "berryPartPane.h"
#include "../berryImageDescriptor.h"

namespace berry
{

void WorkbenchPagePartList::FirePartOpened(IWorkbenchPartReference::Pointer part)
{
  partService.FirePartOpened(part);
}

void WorkbenchPagePartList::FirePartClosed(IWorkbenchPartReference::Pointer part)
{
  partService.FirePartClosed(part);
}

void WorkbenchPagePartList::FirePartAdded(IWorkbenchPartReference::Pointer  /*part*/)
{
  // TODO: There is no listener for workbench page additions yet
}

void WorkbenchPagePartList::FirePartRemoved(
    IWorkbenchPartReference::Pointer  /*part*/)
{
  // TODO: There is no listener for workbench page removals yet
}

void WorkbenchPagePartList::FireActiveEditorChanged(
    IWorkbenchPartReference::Pointer ref)
{
  if (ref.IsNotNull())
  {
    this->FirePartBroughtToTop(ref);
  }
}

void WorkbenchPagePartList::FireActivePartChanged(
    IWorkbenchPartReference::Pointer /*oldRef*/,
    IWorkbenchPartReference::Pointer newRef)
{
  partService.SetActivePart(newRef);

  IWorkbenchPart::Pointer realPart = newRef == 0 ? IWorkbenchPart::Pointer(0) : newRef->GetPart(false);
  selectionService->SetActivePart(realPart);
}

void WorkbenchPagePartList::FirePartHidden(IWorkbenchPartReference::Pointer ref)
{
  partService.FirePartHidden(ref);
}

void WorkbenchPagePartList::FirePartVisible(IWorkbenchPartReference::Pointer ref)
{
  partService.FirePartVisible(ref);
}

void WorkbenchPagePartList::FirePartInputChanged(
    IWorkbenchPartReference::Pointer ref)
{
  partService.FirePartInputChanged(ref);
}

WorkbenchPagePartList::WorkbenchPagePartList(PageSelectionService* selService) :
  selectionService(selService), partService("", "")
{

}

IPartService* WorkbenchPagePartList::GetPartService()
{
  return &partService;
}

void WorkbenchPagePartList::FirePartBroughtToTop(
    IWorkbenchPartReference::Pointer ref)
{
  partService.FirePartBroughtToTop(ref);
}

}
