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

#include "mitkFileWriter.h"

bool mitk::FileWriter::CanWriteDataType( DataNode* )
{
  //TODO #345 check for writing permission
  return false;
}

std::string mitk::FileWriter::GetWritenMIMEType()
{
  return "";
}

void mitk::FileWriter::SetInput( DataNode* )
{
}


std::string mitk::FileWriter::GetFileExtension()
{
  return "";
}

std::string mitk::FileWriter::GetPossibleFileExtensionsAsString()
{
  std::vector<std::string> possibleFileExtensions = this->GetPossibleFileExtensions();
  std::stringstream stream;
  for (unsigned int i=0; i<possibleFileExtensions.size()-1; i++)
  {
    stream<<"*"<<possibleFileExtensions.at(i)<<" ";
  }
  stream<<"*"<<possibleFileExtensions.at(possibleFileExtensions.size()-1);
  return stream.str();
}

bool mitk::FileWriter::IsExtensionValid(std::string extension)
{
  std::vector<std::string> possibleFileExtensions = this->GetPossibleFileExtensions();
  for (unsigned int i=0; i<possibleFileExtensions.size(); i++)
  {
    if (strcmp(extension.c_str(),possibleFileExtensions.at(i).c_str())==0)
      return true;
  }
  return false;
}


mitk::FileWriter::FileWriter() :
m_CanWriteToMemory(false),
m_WriteToMemory(false)
{
}

mitk::FileWriter::~FileWriter()
{
}

bool mitk::FileWriter::CanWriteToMemory(  )
{
  return  m_CanWriteToMemory;
}
void mitk::FileWriter::SetWriteToMemory( bool write )
{
  m_WriteToMemory = write;
}
bool mitk::FileWriter::GetWriteToMemory(  )
{
  return  m_WriteToMemory;
}
const char*  mitk::FileWriter::GetMemoryPointer()
{
  return (const char*) m_MemoryBuffer;
}
unsigned int mitk::FileWriter::GetMemorySize()
{
  return  m_MemoryBufferSize;
}
void mitk::FileWriter::ReleaseMemory()
{
  // do nothing here
}
