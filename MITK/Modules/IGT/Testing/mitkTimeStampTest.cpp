/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-02-25 18:43:22 +0100 (Mi, 25 Feb 2009) $
Version:   $Revision: 16010 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mitkTimeStamp.h"
#include "mitkRealTimeClock.h"

#include "mitkTestingMacros.h"
#include "itkObject.h"

//#include <time.h>



/**
 *  test for the class "mitkTimeStamp"
 */
int mitkTimeStampTest(int /* argc */, char* /*argv*/[])
{
  MITK_TEST_BEGIN("TimeStamp")

  // let's create an object of our class  
  
  
  // first test: did this work?
  // using MITK_TEST_CONDITION_REQUIRED makes the test stop after failure, since
  // it makes no sense to continue without an object.
  MITK_TEST_CONDITION_REQUIRED(mitk::TimeStamp::GetInstance() != (0), "Testing GetInstance() of Singleton" );

  itk::Object::Pointer tester = itk::Object::New();
  itk::Object::Pointer tester2 = itk::Object::New();

  //start-tracking sets the reference-time, timestamps are relative to this time
  mitk::TimeStamp::GetInstance()->Start(tester);
  std::cout << "first device has started tracking\n";

  //sleeps for 20 ms
  #if defined (WIN32) || defined (_WIN32)
  Sleep(20);
  #else
  usleep(20000);
  #endif

  std::cout << "supposed to have waited 20ms \n";

  double time_elapsed, relative_time_elapsed;

  //gets time elapsed since start
  time_elapsed = mitk::TimeStamp::GetInstance()->GetElapsed();

  std::cout << "have actually waited : " << time_elapsed << "ms \n";

  //elapsed time is not allowed to be too far from 20 ms (+-10ms)-> debugging "destroys" the correct value of course
  MITK_TEST_CONDITION_REQUIRED((time_elapsed-20) > -10 , "Testing if elapsed time is correct (-10)");
  //MITK_TEST_CONDITION_REQUIRED((time_elapsed-20) < 10 , "Testing if elapsed time is correct (+10)");

  //second "device" starts tracking
  mitk::TimeStamp::GetInstance()->Start(tester2);
  std::cout << "second device has started\n";
  //first device stops
  mitk::TimeStamp::GetInstance()->Stop(tester);
  std::cout << "first device has stopped tracking\n";

  time_elapsed = mitk::TimeStamp::GetInstance()->GetElapsed();
  relative_time_elapsed = mitk::TimeStamp::GetInstance()->GetElapsed(tester2);

  std::cout << "time elapsed supposed to be greater than 20 ms\n";
  std::cout << time_elapsed << " actually elapsed\n";

  std::cout << "relative time elapsed supposed to be smaller than absolute time elapsed : \n";
  std::cout << relative_time_elapsed << " actually elapsed\n";

  //relative timespan must be smaller than absolute timespan
  MITK_TEST_CONDITION_REQUIRED( time_elapsed > relative_time_elapsed , " testing if relative timespan is shorter than absolute timespan");

  //timestamp still has to be valid (tester2 still tracking), and has to be larger than 20ms
  //MITK_TEST_CONDITION_REQUIRED( time_elapsed > 15 , "testing if second device is still keeping the TimeStamp \"alive\"");

  mitk::TimeStamp::GetInstance()->Stop(tester2);
  std::cout << " second device has stopped tracking\n";
  time_elapsed = mitk::TimeStamp::GetInstance()->GetElapsed();

  //when all devices have stopped, -1 has to be returned
  MITK_TEST_CONDITION_REQUIRED( time_elapsed == -1 , "testing if -1 is returned after all devices have stopped");

  // always end with this!
  MITK_TEST_END();

}


