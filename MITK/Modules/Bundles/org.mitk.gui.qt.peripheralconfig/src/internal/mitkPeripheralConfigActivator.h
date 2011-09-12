/*=========================================================================

 Program:   MITK Platform
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


#ifndef MBIPERIPHERALCONFIGACTIVATOR_H_
#define MBIPERIPHERALCONFIGACTIVATOR_H_

#include <berryPlugin.h>

#include <PeripheralconfigDll.h>

namespace mitk
{

	/**
	 * \ingroup org_mitk_core_sandbox_internal
	 *
	 * \brief The plug-in activator for the sandbox
	 *
	 * When the plug-in is started by the framework, it initialzes sandbox
	 * specific things.
	 */
	class PERIPHERALCONFIG_EXPORT PeripheralConfigActivator : public berry::Plugin
	{

	public:

			
		static std::string m_PersistentDir;
		/**
		 * Registers sandbox core object factories.
		 */
		void Start(berry::IBundleContext::Pointer context);

	};

}

#endif /* MBIPERIPHERALCONFIGACTIVATOR_H_ */
