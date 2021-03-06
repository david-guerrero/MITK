#include "mitkPluginActivator.h"

#include <QtPlugin>

#include "QmitkSimpleExampleView.h"

namespace mitk {

void PluginActivator::start(ctkPluginContext* context)
{
  BERRY_REGISTER_EXTENSION_CLASS(QmitkSimpleExampleView, context)
}

void PluginActivator::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

}

Q_EXPORT_PLUGIN2(org_mitk_gui_qt_simpleexample, mitk::PluginActivator)
