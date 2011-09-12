/*=========================================================================

 Program:   MITK Platform
 Language:  C++
 Date:      $Date$
 Version:   $Revision: 17020 $

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/


#ifndef QMITKCOMMONACTIVATOR_H_
#define QMITKCOMMONACTIVATOR_H_

#include <ctkPluginActivator.h>

/**
 * \ingroup org_mitk_core_ext_internal
 *
 * \brief The plug-in activator for the StateMachine
 *
 * When the plug-in is started by the framework, it initializes StateMachine
 * specific things.
 */
class QmitkCommonActivator : public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

public:

  /**
   * Sets default StateMachine to EventMapper.
   */
  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

};

#endif /* QMITKCOMMONACTIVATOR_H_ */

