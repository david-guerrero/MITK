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

#include "berryLog.h"

#include "berryBundleDirectory.h"
#include "berryInternalPlatform.h"

#include <iostream>

#include "Poco/Exception.h"
#include "Poco/FileStream.h"
#include "Poco/Path.h"
#include "Poco/File.h"

namespace berry {

BundleDirectory::BundleDirectory(const Path& path) :
   m_RootPath(path)
{
  //Path manifestPath(m_RootPath);
  //manifestPath.append(Path("META-INF/manifest.mf"));
}

BundleDirectory::~BundleDirectory()
{

}

bool BundleDirectory::operator==(const Object* o) const
{
  if (const IBundleStorage* dir = dynamic_cast<const IBundleStorage*>(o))
    return this->GetPath().toString() == dir->GetPath().toString();

  return false;
}

std::istream*
BundleDirectory::GetResource(const std::string& path) const
{
  Poco::Path resPath(m_RootPath);
  std::string resStr = resPath.append(Path(path)).toString();
  //BERRY_INFO << "Getting resource: " << resStr << std::endl;
  try {
    return new Poco::FileInputStream(resStr);
  }
  catch (...)
  {}
  return 0;
}

void
BundleDirectory::List(const std::string& path, std::vector<std::string>& files,
    bool quiet) const
{
  try
  {
    Poco::Path fullPath(m_RootPath);
    fullPath.append(path);
    Poco::File file(fullPath);
    file.list(files);
  }
  catch (Poco::FileNotFoundException& exc)
  {
    if (!quiet)
    {
      BERRY_WARN << "Warning: " << exc.displayText() << std::endl;
      throw exc;
    }
  }
  catch (const Poco::PathNotFoundException& exc)
  {
    if (!quiet)
    {
      BERRY_WARN << "Warning: " << exc.displayText() << std::endl;
      throw exc;
    }
  }
}

bool BundleDirectory::IsDirectory(const std::string& path) const
{
  Poco::Path fullPath(m_RootPath);
  fullPath.append(path);
  Poco::File file(fullPath.makeDirectory());
  BERRY_INFO(InternalPlatform::GetInstance()->ConsoleLog()) <<
    "Testing " << file.path() << " for directory: " <<
    (file.exists() && file.isDirectory() ? "true" : "false") << std::endl;
  return file.exists() && file.isDirectory();
}

Path
BundleDirectory::GetPath() const
{
  return m_RootPath;
}

Path BundleDirectory::BuildPath(const std::string& path) const
{
  Poco::Path tmp(m_RootPath);
  return tmp.append(Path(path));
}

}
