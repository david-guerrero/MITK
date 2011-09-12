/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-07-22 16:45:18 +0200 (Do, 04 Mai 2006) $
Version:   $Revision: 6790 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef VTKMITKEVENTPROVIDER_H_HEADER_INCLUDED_C1C53723
#define VTKMITKEVENTPROVIDER_H_HEADER_INCLUDED_C1C53723

#include "mitkCommon.h"
#include "mitkRenderWindow.h"

#include "vtkInteractorObserver.h"

namespace mitk
{


/**
* \brief Integrates into the VTK event mechanism to generate MITK specific events. 
* This class is NON-QT dependent pandon to the current MITK event handling code in QmitkRenderWindow.
* \ingroup Renderer
*/
class MITK_CORE_EXPORT vtkEventProvider : public vtkInteractorObserver
{
  public:
    static vtkEventProvider *New();
    vtkTypeRevisionMacro(vtkEventProvider,vtkInteractorObserver);

    // Satisfy the superclass API. Enable/disable listening for events.
    virtual void SetEnabled(int);
    virtual void SetInteractor(vtkRenderWindowInteractor* iren);
    
    // Interface to MITK
    virtual void SetMitkRenderWindow(mitk::RenderWindow* renWin);
    mitk::RenderWindow* GetRenderWindow();

  protected:
    vtkEventProvider();
    ~vtkEventProvider();

    //methods for processing events - callback for the observer/command pattern of vtkCommand
    static void ProcessEvents(vtkObject* object, unsigned long event,
                            void* clientdata, void* calldata);

    mitk::RenderWindow* m_RenderWindow;

    // adds the MITK interaction event types to the VTK observer/command pattern
    void AddInteractionEvent(unsigned long ievent);
    // removes the MITK interaction event types
    void RemoveInteractionEvent(unsigned long ievent);
    typedef std::vector<unsigned long> InteractionEventsVectorType;
    InteractionEventsVectorType m_InteractionEventsVector;


  private:
    vtkEventProvider(const vtkEventProvider&);  // Not implemented.
    void operator=(const vtkEventProvider&);        // Not implemented.
   
   
};

}
#endif /* VTKMITKEVENTPROVIDER_H_HEADER_INCLUDED_C1C53723 */


