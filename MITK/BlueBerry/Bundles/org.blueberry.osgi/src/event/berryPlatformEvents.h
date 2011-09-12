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

#ifndef BERRYPLATFORMEVENTS_H_
#define BERRYPLATFORMEVENTS_H_

#include <org_blueberry_osgi_Export.h>

#include "../berryMessage.h"

#include "berryPlatformEvent.h"

namespace berry {

struct BERRY_OSGI PlatformEvents
{
  typedef Message1<const PlatformEvent&> EventType;

  EventType platformStarted;
  EventType platformError;
  EventType platformWarning;

  EventType logged;
};

}

#endif /*BERRYPLATFORMEVENTS_H_*/
