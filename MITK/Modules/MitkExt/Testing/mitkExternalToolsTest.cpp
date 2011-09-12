/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.12 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#include <iostream>
#include <string>

#include <stdlib.h> // for system() call

#include <itksys/SystemTools.hxx> // for chdir et al.

int mitkExternalToolsTest(int argc, char* argv[])
{
  return EXIT_SUCCESS; // we'll go back to a CMake solution... 


  std::cout << "Got " << argc << " parameters" << std::endl;
  if ( argc == 5 )
  {
    // "parse" commandline 
    // quote spaces in commandline parameters (all paths/files)
    //std::string cmakeBinary         = itksys::SystemTools::EscapeChars( argv[1], " " );
    std::string cmakeBinary         = argv[1];
    std::string cmakeGenerator      = argv[2];
    std::string mitkBinaryDirectory = argv[3];
    std::string sourceDirectory     = argv[4];
    
    // try to configure MITK external project
    std::cout << "Calling CMake as '" << cmakeBinary << "'" << std::endl;
    std::cout << "Calling CMake for generator '" << cmakeGenerator << "'" << std::endl;
    std::cout << "MITK was compiled in '" << mitkBinaryDirectory << "'" << std::endl;
    std::cout << "Configuring project in '" << sourceDirectory << "'" << std::endl;

    if( itksys::SystemTools::ChangeDirectory(mitkBinaryDirectory.c_str()) != 0 )
    {
      std::cerr << "Couldn't change to MITK build dir. See output above." << std::endl;
      return EXIT_FAILURE;
    }

    std::string oneCommandlineQuote("\"");

    std::string commandline( oneCommandlineQuote );
    commandline += cmakeBinary;
    commandline += oneCommandlineQuote;
    
    commandline += " -G ";
    commandline += oneCommandlineQuote;
    commandline += cmakeGenerator;
    commandline += oneCommandlineQuote;

    commandline += " -DMITK_DIR:PATH=";

    commandline += oneCommandlineQuote;
    commandline += mitkBinaryDirectory;
    commandline += oneCommandlineQuote;
    commandline += " ";
    commandline += oneCommandlineQuote;
    commandline += sourceDirectory;
    commandline += oneCommandlineQuote;

    std::cout << "Calling system() with '" 
              << commandline
              << "'" 
              << std::endl;

    int returnCode = system(commandline.c_str());

    std::cout << "system() returned " << returnCode << std::endl;

    if (returnCode != 0)
    {
      std::cerr << "Configure FAILED. See output above." << std::endl;
      return EXIT_FAILURE;
    }

    // try to build MITK external project

#ifdef WIN32
#else
    commandline = "make";
    // commented out because mbits configures with Qt4. Have to check this monday.
    //returnCode = system(commandline.c_str());

    if (returnCode != 0) // make should return 0 
    {
      std::cout << "make returned " << returnCode << std::endl;
      std::cerr << "Building the project FAILED. See output above." << std::endl;
      return EXIT_FAILURE;
    }

#endif
    // 
    // TODO extend test here to support windows...
    //
    return returnCode;
  }
  else
  {
    std::cout << "Invoke this test with three parameters:" << std::endl;
    std::cout << "   1. CMake binary including necessary path" << std::endl;
    std::cout << "   2. CMake generator name" << std::endl;
    std::cout << "   3. MITK binary path (top-level directory)" << std::endl;
    std::cout << "   4. Source directory containing CMakeLists.txt" << std::endl;
    return EXIT_FAILURE;
  }
}

