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

#ifndef BERRYIACTIONBARCONFIGURER_H_
#define BERRYIACTIONBARCONFIGURER_H_

#include <berryMacros.h>

#include <org_blueberry_ui_Export.h>

#include "berryIWorkbenchWindowConfigurer.h"

namespace berry
{

/**
 * Interface providing special access for configuring the action bars
 * of a workbench window.
 * <p>
 * Note that these objects are only available to the main application
 * (the plug-in that creates and owns the workbench).
 * </p>
 * <p>
 * This interface is not intended to be implemented by clients.
 * </p>
 *
 * @see org.blueberry.ui.application.WorkbenchAdvisor#fillActionBars
 * @since 3.0
 * @noimplement This interface is not intended to be implemented by clients.
 */
struct BERRY_UI IActionBarConfigurer : public Object {

  berryInterfaceMacro(IActionBarConfigurer, berry);

    /**
     * Returns the workbench window configurer for the window
     * containing this configurer's action bars.
     *
     * @return the workbench window configurer
     * @since 3.1
     */
    virtual IWorkbenchWindowConfigurer::Pointer GetWindowConfigurer() = 0;


    /**
     * Returns the menu manager for the main menu bar of a workbench window.
     *
     * @return the menu manager
     */
    virtual void* GetMenuManager() = 0;

    /**
     * Returns the status line manager of a workbench window.
     *
     * @return the status line manager
     */
    //virtual IStatusLineManager GetStatusLineManager() = 0;

    /**
     * Returns the cool bar manager of the workbench window.
     *
     * @return the cool bar manager
     */
    //virtual ICoolBarManager GetCoolBarManager() = 0;

    /**
     * Register the action as a global action with a workbench
     * window.
     * <p>
     * For a workbench retarget action
     * ({@link org.blueberry.ui.actions.RetargetAction RetargetAction})
     * to work, it must be registered.
     * You should also register actions that will participate
     * in custom key bindings.
     * </p>
     *
     * @param action the global action
     * @see org.blueberry.ui.actions.RetargetAction
     */
    //virtual void RegisterGlobalAction(IAction action) = 0;

};

}

#endif /*BERRYIACTIONBARCONFIGURER_H_*/
