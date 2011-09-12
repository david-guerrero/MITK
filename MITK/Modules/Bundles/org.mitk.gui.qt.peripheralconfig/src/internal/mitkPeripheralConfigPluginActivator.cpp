#include "mitkPeripheralConfigPluginActivator.h"

#include <QtPlugin>

namespace mitk {

void PeripheralConfiPlugingActivator::start(ctkPluginContext* context)
{
  BERRY_REGISTER_EXTENSION_CLASS(QmitkMITKPeripheralConfigViewImageCropperView, context)
}

void PeripheralConfigPluginActivator::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

}

Q_EXPORT_PLUGIN2(org_mitk_gui_qt_peripheralconfig, mitk::PeripheralConfigPluginActivator)