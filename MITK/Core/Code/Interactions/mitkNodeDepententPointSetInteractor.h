/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-10-04 14:49:09 +0200 (Mo, 04 Okt 2010) $
Version:   $Revision: 26576 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef MITKNodeDepententPointSetInteractor_H_HEADER_INCLUDED
#define MITKNodeDepententPointSetInteractor_H_HEADER_INCLUDED

#include "mitkCommon.h"
#include "mitkVector.h"
#include <mitkInteractor.h>
#include <mitkDataNode.h>
#include <mitkPointSetInteractor.h>


namespace mitk
{

  /**
   * \brief PointSetInteraction that is dependent on the visibility property of a data node.
   *
   * The interactor checks if the renderwindow specific property "visible" of a different node (e.g. image) 
   * specified by @param dependentDataNode is true. The specific renderwindow is specified by the sender of the event.
   * If the property is true the the object behaves as described by PointSetInteractor. 
   * If not, interaction is blocked.
   *
   * This class shows how to write an interactor, that is dependent on a special property of the associated node. 
   * See bug #6047 for further information and a patch to test this class embedded in tutorial Step 5.
   * \ingroup Interaction
   */
  class MITK_CORE_EXPORT NodeDepententPointSetInteractor : public PointSetInteractor
  {
  public:
    mitkClassMacro(NodeDepententPointSetInteractor, PointSetInteractor);
    mitkNewMacro4Param(Self, const char*, DataNode*, DataNode*, int);
    mitkNewMacro3Param(Self, const char*, DataNode*, DataNode*);

    /**
     * \brief Checks visibility of the specified node (e.g. image), 
     * returns 0 if node is not visible in sending render window
     * If Sender within stateEvent is NULL a value of 0 is returned. 
     */
    virtual float CanHandleEvent(StateEvent const* stateEvent) const;

  protected:
    /**
     * \brief Constructor with Param n for limited Set of Points
     *
     * If no n is set, then the number of points is unlimited
     * n=0 is not supported. In this case, n is set to 1.
     */
    NodeDepententPointSetInteractor(const char * type, DataNode* dataNode, DataNode* dependentDataNode, int n = -1);

    /**
     * \brief Default Destructor
     **/
    virtual ~NodeDepententPointSetInteractor();
    
  public:
  
    mitk::DataNode::Pointer m_DependentDataNode;

  };
}
#endif /* MITKNodeDepententPointSetInteractor_H_HEADER_INCLUDED */
