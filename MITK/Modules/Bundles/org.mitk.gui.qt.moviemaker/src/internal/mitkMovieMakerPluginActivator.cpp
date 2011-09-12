#include "mitkMovieMakerPluginActivator.h"

#include "QmitkMovieMaker.h"

#include <QtPlugin>

namespace mitk {

  void MovieMakerPluginActivator::start(ctkPluginContext* context)
  {
    BERRY_REGISTER_EXTENSION_CLASS(QmitkMovieMaker, context)
  }

  void MovieMakerPluginActivator::stop(ctkPluginContext* context)
  {
    Q_UNUSED(context)
  }

}

Q_EXPORT_PLUGIN2(org_mitk_gui_qt_moviemaker, mitk::MovieMakerPluginActivator)