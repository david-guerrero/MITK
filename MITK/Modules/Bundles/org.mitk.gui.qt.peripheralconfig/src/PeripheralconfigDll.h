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

#ifndef _PERIPHERALCONFIG_EXPORT_DLL_H_
#define _PERIPHERALCONFIG_EXPORT_DLL_H_


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the org_mitk_gui_qt_peripheralconfig_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// org_mitk_gui_qt_peripheralconfig_EXPORTS functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && !defined(MITK_STATIC)
  #if defined(org_mitk_gui_qt_peripheralconfig_EXPORTS)
    #define PERIPHERALCONFIG_EXPORT __declspec(dllexport)
  #else
    #define PERIPHERALCONFIG_EXPORT __declspec(dllimport)
  #endif
#endif


#if !defined(PERIPHERALCONFIG_EXPORT)
  #define PERIPHERALCONFIG_EXPORT
#endif

#endif /*_PERIPHERALCONFIG_EXPORT_DLL_H_*/
