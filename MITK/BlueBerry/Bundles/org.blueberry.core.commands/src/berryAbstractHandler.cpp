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

#include "berryAbstractHandler.h"

namespace berry
{

AbstractHandler::AbstractHandler() :
  baseEnabled(true)
{

}

bool AbstractHandler::IsEnabled()
{
  return baseEnabled;
}

bool AbstractHandler::IsHandled()
{
  return true;
}

void AbstractHandler::SetBaseEnabled(bool state)
{
  if (baseEnabled == state)
  {
    return;
  }
  baseEnabled = state;
  //fireHandlerChanged(new HandlerEvent(this, true, false));
}

}
