#ifndef MITKPERIPHERALCONFIGPLUGINACTIVATOR_H
#define MITKPERIPHERALCONFIGPLUGINACTIVATOR_H

#include <ctkPluginActivator.h>

namespace mitk {

class PeripheralConfigPluginActivator :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

public:

	static std::string m_PersistentDir;
  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

}; // PeripheralConfig Activator

}

#endif // MITKPERIPHERALCONFIGPLUGINACTIVATOR_H