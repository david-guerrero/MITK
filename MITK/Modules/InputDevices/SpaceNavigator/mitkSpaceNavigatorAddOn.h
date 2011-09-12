/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-03-31 17:34:48 +0200 (Mi, 31 Mrz 2010) $
Version:   $Revision: 21985 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef MITKSPACENAVIGATORADDON_H_HEADER_INCLUDED
#define MITKSPACENAVIGATORADDON_H_HEADER_INCLUDED

#include <mitkVector.h>
#include <mitkCommon.h>

#include <mitkEventMapperAddOn.h>
#include <mitkSpaceNavigatorExports.h>

namespace mitk {

  /**
  * Documentation in the interface.
  *
  * @brief EventMapper addon for a 3DConnexion Space Navigator
  * @ingroup Interaction
  */
  class mitkSpaceNavigator_EXPORT SpaceNavigatorAddOn : public EventMapperAddOn
  {

  public:

    // Singleton
    static SpaceNavigatorAddOn* GetInstance();

    /**
    * @noimplement not needed
    */
    void DeviceChange (long device, long keys, long programmableKeys);

    /**
    * If a button is pressed down an event is fired.
    *
    * @param keyCode the id to the key, that is pressed
    */
    void KeyDown (int keyCode);

    /**
    * @noimplement not needed
    */
    void KeyUp (int keyCode);

    /**
    * Reacts on any movement of the mouse and fires events accordingly.
    *
    * @param translation 
    *           the translation of the mouse as an 3D vector
    * @param rotation 
    *           the rotation of the mouse as an 3D vector
    * @param angle 
    *           the angle from the mouse as an scalar unit
    */
    void SensorInput(mitk::Vector3D translation, mitk::Vector3D rotation, mitk::ScalarType angle);

  protected:

  private:

  }; // end class SpaceNavigatorAddOn
} // end namespace mitk

#endif // MITKSPACENAVIGATORADDON_H_HEADER_INCLUDED
