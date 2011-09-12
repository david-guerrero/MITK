/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
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


#include "mitkStepper.h"
#include "mitkTestingMacros.h"

int mitkStepperTest(int /*argc*/, char* /*argv*/[])
{
  MITK_TEST_BEGIN(StepperTest)

  mitk::Stepper::Pointer stepperA;
  stepperA = mitk::Stepper::New();
  MITK_TEST_CONDITION_REQUIRED(stepperA.IsNotNull(),"Stepper instantiation.")

  // number of steps defaults to zero, SetPos should have no effect, GetPos should return zero
  stepperA->SetPos(10);
  MITK_TEST_CONDITION_REQUIRED(stepperA->GetPos() == 0,"Pos remains zero if m_Steps is zero.")

  mitk::Stepper::Pointer stepperB = mitk::Stepper::New();
  stepperA->SetSteps(4);
  //stepperA->PingPongOn();
  stepperB->SetSteps(6);
  // stepperB->PingPongOn();
  /* for (int i=0 ; i<10; i++) {
    std::cout << i << ": A: " << stepperA->GetPos() << " B:" << stepperB->GetPos() << std::endl; 
    stepperA->Next();
    stepperB->Next();
  }*/
  
  MITK_TEST_END()
}
