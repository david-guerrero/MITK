/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-02-10 18:08:54 +0100 (Di, 10 Feb 2009) $
Version:   $Revision: 16228 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkRealTimeClock.h"

// check if MITK runs on a Windows-System
#ifdef _WIN32
  #include "mitkWindowsRealTimeClock.h"
#else // should be Linux or Mac OSX
  #include "mitkLinuxRealTimeClock.h"
#endif

mitk::RealTimeClock::Pointer mitk::RealTimeClock::New()
{
  mitk::RealTimeClock::Pointer smartPtr;

#ifdef _WIN32
  smartPtr = mitk::WindowsRealTimeClock::New();
#else
  smartPtr = mitk::LinuxRealTimeClock::New();
#endif
  
  return smartPtr;
}