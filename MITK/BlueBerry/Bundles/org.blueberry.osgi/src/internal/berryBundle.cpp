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

#include "berryLog.h"

#include "Poco/Exception.h"

#include "berryBundle.h"
#include "berryInternalPlatform.h"
#include "../berryBundleLoader.h"
#include "berryBundleManifest.h"
#include "../berryIBundleActivator.h"
#include "../berryIBundleContext.h"
#include "../berryPlatformException.h"

#include <ctkPluginContext.h>

#include <iostream>

namespace berry {

Bundle::Bundle(BundleLoader& loader, IBundleStorage::Pointer storage, bool initialize) :
  m_BundleLoader(loader)
{
  {
    Poco::Mutex::ScopedLock lock(m_Mutex);
    m_Storage = storage;
  }

  if (initialize)
  {
    init();
  }
}

void Bundle::init()
{
  Poco::Mutex::ScopedLock lock(m_Mutex);
  try
  {
    this->LoadManifest();
    m_State = BUNDLE_INSTALLED;
  }
  catch (Poco::FileException& exc)
  {
    BERRY_ERROR << "Exception: " << exc.displayText() << std::endl;
    m_State = BUNDLE_UNINSTALLED;
  }
}

Bundle::~Bundle()
{

}

bool Bundle::operator==(const Object* o) const
{
  if (const IBundle* bundle = dynamic_cast<const IBundle*>(o))
    return this->GetSymbolicName() == bundle->GetSymbolicName();

  return false;
}

IBundleActivator*
Bundle::GetActivator() const
{
  Poco::Mutex::ScopedLock lock(m_Mutex);
  if (m_State == BUNDLE_ACTIVE)
    return m_Activator;

  return 0;
}

const std::string&
Bundle::GetActivatorClass() const
{
  return m_Manifest->GetActivatorClass();
}

const std::string&
Bundle::GetActivatorLibrary() const
{
  return m_Manifest->GetActivatorLibrary();
}

const std::string&
Bundle::GetCopyright() const
{
  return m_Manifest->GetCopyright();
}

const std::string&
Bundle::GetVendor() const
{
  return m_Manifest->GetVendor();
}

IBundleManifest::ActivationPolicy
Bundle::GetActivationPolicy() const
{
  return m_Manifest->GetActivationPolicy();
}

std::istream*
Bundle::GetLocalizedResource(const std::string& name) const
{
  return m_Storage->GetResource(name);
}

std::istream*
Bundle::GetResource(const std::string& name) const
{
  return m_Storage->GetResource(name);
}

bool
Bundle::IsActive() const
{
  return m_State == BUNDLE_ACTIVE;
}

bool
Bundle::IsResolved() const
{
  return m_State == BUNDLE_RESOLVED || m_State == BUNDLE_STARTING ||
          m_State == BUNDLE_ACTIVE || m_State == BUNDLE_STOPPING;
}

bool
Bundle::IsStarted() const
{
  return m_State == BUNDLE_STARTING || m_State == BUNDLE_ACTIVE ||
          m_State == BUNDLE_STOPPING;
}

bool Bundle::IsSystemBundle() const
{
  return m_Manifest->IsSystemBundle();
}

const IBundleManifest&
Bundle::GetManifest() const
{
  return *m_Manifest;
}

const std::string&
Bundle::GetName() const
{
  return m_Manifest->GetName();
}

const Poco::Path
Bundle::GetPath() const
{
  return m_Storage->GetPath();
}

IBundleStorage&
Bundle::GetStorage()
{
  return *m_Storage;
}

// const Version& GetVersion() const;

const IBundleManifest::Dependencies&
Bundle::GetRequiredBundles() const
{
  return m_Manifest->GetRequiredBundles();
}

void
Bundle::Resolve()
{
  if (m_State == BUNDLE_INSTALLED)
  {
    ctkPluginContext* context = InternalPlatform::GetInstance()->GetCTKPluginFrameworkContext();

    //const BundleManifest::Dependencies& dependencies =;
    IBundleManifest::Dependencies::const_iterator iter;
    for (iter =  this->GetRequiredBundles().begin(); iter !=  this->GetRequiredBundles().end(); ++iter)
    {
      //BERRY_INFO << "Checking dependency:" << iter->symbolicName << ";\n";
      IBundle::Pointer bundle = m_BundleLoader.FindBundle(iter->symbolicName);
      if (bundle.IsNull())
      {
        // Check if we have a CTK Plugin dependency
        bool resolved = false;
        if (context)
        {
          QString symbolicName = QString::fromStdString(iter->symbolicName);
          foreach (QSharedPointer<ctkPlugin> plugin, context->getPlugins())
          {
            if (plugin->getSymbolicName() == symbolicName)
            {
              resolved = true;
              break;
            }
          }
        }

        if (!resolved)
        {
          throw BundleResolveException("The bundle " + this->GetSymbolicName() + " depends on missing bundle:", iter->symbolicName);
        }
      }
      else if (!bundle->IsResolved())
      {
        bundle->Resolve();
      }
    }
    m_State = BUNDLE_RESOLVED;
  }
}

void
Bundle::Start()
{
  if (m_State == BUNDLE_RESOLVED)
  {
    Poco::Mutex::ScopedLock lock(m_Mutex);

    m_State = BUNDLE_STARTING;
//    BundleEvent starting(this, BundleEvent::EV_BUNDLE_STARTING);
//    this->GetEvents().bundleStarting(this, starting);
    BERRY_INFO(InternalPlatform::GetInstance()->ConsoleLog()) << "Bundle " << this->GetSymbolicName() << " is starting";
    m_Activator->Start(m_BundleLoader.GetContextForBundle(IBundle::Pointer(this)));

    m_State = BUNDLE_ACTIVE;
//    BundleEvent started(this, BundleEvent::EV_BUNDLE_STARTED);
//    this->GetEvents().bundleStarted(this, started);
//    BERRY_INFO << "Bundle " << this->GetSymbolicName() << " is active";
  }
  else
  {
    throw BundleStateException("Bundle " + this->GetSymbolicName() +
        " could not be started, because it is not in state RESOLVED.");
  }
}

void
Bundle::Stop()
{
  throw Poco::NotImplementedException("Bundle::Stop() not implemented yet");
}

Bundle::State
Bundle::GetState() const
{
  return m_State;
}

std::string
Bundle::GetStateString() const
{
  switch (this->GetState())
  {
  case BUNDLE_INSTALLED:
  return "Installed";
  case BUNDLE_UNINSTALLED:
    return "Uninstalled";
  case BUNDLE_RESOLVED:
    return "Resolved";
  case BUNDLE_STARTING:
    return "Starting";
  case BUNDLE_ACTIVE:
    return "Active";
  case BUNDLE_STOPPING:
    return "Stopping";
  default: throw BundleStateException("The bundle is not in a valid state");
  }
}

BundleEvents&
Bundle::GetEvents()
{
  return m_BundleLoader.GetEvents();
}

const std::string&
Bundle::GetSymbolicName() const
{
  return m_Manifest->GetSymbolicName();
}

void
Bundle::LoadManifest()
{
  std::istream* istr = m_Storage->GetResource("META-INF/MANIFEST.MF");
  if (!istr)
    throw Poco::FileNotFoundException("Could not load META-INF/MANIFEST.MF from " + m_Storage->GetPath().toString());

  m_Manifest = new BundleManifest(istr);
  delete istr;
}

void
Bundle::SetActivator(IBundleActivator* activator)
{
  Poco::Mutex::ScopedLock lock(m_Mutex);
  m_Activator = activator;
}

}
