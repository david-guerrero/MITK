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

#ifndef BERRYINTERNALPLATFORM_H_
#define BERRYINTERNALPLATFORM_H_

#include <Poco/Path.h>
#include <Poco/Mutex.h>
#include <Poco/AutoPtr.h>
#include <Poco/Logger.h>
#include <Poco/Util/Application.h>

#include "../event/berryPlatformEvents.h"
#include "../service/berryServiceRegistry.h"
#include "berryExtensionPointService.h"

#include <map>

class ctkPluginFrameworkFactory;
class ctkPluginContext;

namespace berry {

struct IBundle;
class CodeCache;
class BundleLoader;
class PlatformLogChannel;
class SystemBundle;

class BERRY_OSGI InternalPlatform : private Poco::Util::Application
{
private:

  static Poco::Mutex m_Mutex;

  bool m_Initialized;
  bool m_Running;

  bool m_ConsoleLog;

  ServiceRegistry* m_ServiceRegistry;

  Poco::Path m_BaseStatePath;
  Poco::Path m_InstallPath;
  Poco::Path m_InstancePath;
  Poco::Path m_UserPath;
  Poco::Path m_ConfigPath;

  std::vector<std::string> m_FilteredArgs;

  CodeCache* m_CodeCache;
  BundleLoader* m_BundleLoader;

  SystemBundle* m_SystemBundle;

  Poco::AutoPtr<PlatformLogChannel> m_PlatformLogChannel;
  Poco::Logger* m_PlatformLogger;

  ctkPluginFrameworkFactory* m_ctkPluginFrameworkFactory;
  QList<long> m_CTKPluginsToStart;

  PlatformEvents m_Events;
  PlatformEvent m_EventStarted;

  int* m_Argc;
  char** m_Argv;

  //std::map<std::string, std::string> m_ArgMap;

  InternalPlatform();
  //InternalPlatform(const InternalPlatform&) : m_EventStarted(PlatformEvent::EV_PLATFORM_STARTED) {};

  void AssertInitialized();

  // Poco::Application method overrides
  void defineOptions(Poco::Util::OptionSet& options);
  int main(const std::vector<std::string>& args);

  void uninstallPugin(const QUrl& pluginUrl, ctkPluginContext* pfwContext);

public:
  virtual ~InternalPlatform();

  void PrintHelp(const std::string& name, const std::string& value);

  static InternalPlatform* GetInstance();

  void Initialize(int& argc, char** argv, Poco::Util::AbstractConfiguration* config = 0);
  void Launch();
  void Shutdown();

  ctkPluginContext* GetCTKPluginFrameworkContext() const;

  /// Returns a ServiceRegistry object for registering
  /// and accessing services from different plugins
  ServiceRegistry& GetServiceRegistry();

  /// Convenience method to quickly get the extension
  /// point service, which is automatically started
  /// by the platform
  IExtensionPointService::Pointer GetExtensionPointService();

  bool ConsoleLog() const;

  const Poco::Path& GetConfigurationPath();

  const Poco::Path& GetInstallPath();

  const Poco::Path& GetInstancePath();

  bool GetStatePath(Poco::Path& statePath, SmartPointer<IBundle> bundle, bool create = true);

  const Poco::Path& GetUserPath();

  PlatformEvents& GetEvents();

  bool IsRunning() const;

  Poco::Util::LayeredConfiguration& GetConfiguration() const;

  std::vector<std::string> GetApplicationArgs() const;

  int& GetRawApplicationArgs(char**& argv);

  IBundle::Pointer GetBundle(const std::string& id);

  std::vector<IBundle::Pointer> GetBundles() const;

  Poco::Logger* GetLogger();
};

}  // namespace berry

#endif /*BERRYINTERNALPLATFORM_H_*/
