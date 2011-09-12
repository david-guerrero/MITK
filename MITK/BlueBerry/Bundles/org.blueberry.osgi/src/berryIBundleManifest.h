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

#ifndef BERRYIBUNDLEMANIFEST_H_
#define BERRYIBUNDLEMANIFEST_H_

#include "berryMacros.h"
#include "berryObject.h"

#include <vector>

namespace berry {


struct BERRY_OSGI IBundleManifest : public Object
{
  berryInterfaceMacro(IBundleManifest, berry);

  static const std::string BUNDLE_ACTIVATOR;
  static const std::string BUNDLE_COPYRIGHT;
  static const std::string BUNDLE_ACTIVATION_POLICY;
  static const std::string BUNDLE_NAME;
  static const std::string BUNDLE_SYMBOLICNAME;
  static const std::string BUNDLE_VENDOR;
  static const std::string BUNDLE_VERSION;
  static const std::string MANIFEST_VERSION;
  static const std::string REQUIRE_BUNDLE;
  static const std::string SYSTEM_BUNDLE;
  static const std::string VERSION;

  enum ActivationPolicy { LAZY, EAGER };



  struct Dependency {
    std::string symbolicName;
    // VersionRange versions;
  };

  typedef std::vector<Dependency> Dependencies;

  virtual ~IBundleManifest() {};

  virtual const std::string& GetActivatorClass() const = 0;
  virtual const std::string& GetActivatorLibrary() const = 0;
  virtual const std::string& GetCopyright() const = 0;
  virtual ActivationPolicy GetActivationPolicy() const = 0;
  virtual bool IsSystemBundle() const = 0;
  virtual const std::string& GetName() const = 0;
  virtual const Dependencies& GetRequiredBundles() const = 0;
  virtual const std::string& GetSymbolicName() const = 0;
  virtual const std::string& GetVendor() const = 0;
  //const Version& GetVersion() const;

};

}  // namespace berry

#endif /*BERRYIBUNDLEMANIFEST_H_*/
