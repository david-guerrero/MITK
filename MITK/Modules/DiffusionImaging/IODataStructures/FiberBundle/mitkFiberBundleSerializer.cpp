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

#include "mitkFiberBundleSerializer.h"
#include "mitkFiberBundle.h"
#include "mitkFiberBundleWriter.h"

#include <itksys/SystemTools.hxx>


MITK_REGISTER_SERIALIZER(FiberBundleSerializer)


mitk::FiberBundleSerializer::FiberBundleSerializer()
{
}


mitk::FiberBundleSerializer::~FiberBundleSerializer()
{
}


std::string mitk::FiberBundleSerializer::Serialize()
{
  const FiberBundle* fb = dynamic_cast<const FiberBundle*>( m_Data.GetPointer() );
  if (fb == NULL)
  {
    MITK_ERROR << " Object at " << (const void*) this->m_Data
              << " is not an mitk::FiberBundle. Cannot serialize as FiberBundle.";
    return "";
  }

  std::string filename( this->GetUniqueFilenameInWorkingDirectory() );
  filename += "_";
  filename += m_FilenameHint;
  filename += ".fib";

  std::string fullname(m_WorkingDirectory);
  fullname += "/";
  fullname += itksys::SystemTools::ConvertToOutputPath(filename.c_str());

  try
  {
    FiberBundleWriter::Pointer writer = FiberBundleWriter::New();
    writer->SetFileName(fullname);
    writer->SetInputFiberBundle(const_cast<FiberBundle*>(fb));
    writer->Write();
  }
  catch (std::exception& e)
  {
    MITK_ERROR << " Error serializing object at " << (const void*) this->m_Data
              << " to " 
              << fullname 
              << ": " 
              << e.what();
    return "";
  }
  return filename;
}

