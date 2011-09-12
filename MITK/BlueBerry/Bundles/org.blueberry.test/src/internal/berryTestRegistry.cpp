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

#include "berryTestRegistry.h"
#include "berryTestDescriptor.h"

#include <berryPlatform.h>
#include <service/berryIExtensionPointService.h>

namespace berry
{

const std::string TestRegistry::TAG_TEST = "test";
const std::string TestRegistry::ATT_ID = "id";
const std::string TestRegistry::ATT_CLASS = "class";
const std::string TestRegistry::ATT_DESCRIPTION = "description";
const std::string TestRegistry::ATT_UITEST = "uitest";

const std::string TestRegistry::TEST_MANIFEST = "CppUnitTest";

TestRegistry::TestRegistry()
{
  std::vector<IConfigurationElement::Pointer> elements(
      Platform::GetExtensionPointService()->GetConfigurationElementsFor(
          "org.blueberry.tests"));

  for (std::vector<IConfigurationElement::Pointer>::iterator i =
      elements.begin(); i != elements.end(); ++i)
  {
    if ((*i)->GetName() == TAG_TEST)
    {
      this->ReadTest(*i);
    }
  }
}

const std::vector<ITestDescriptor::Pointer>&
TestRegistry::GetTestsForId(const std::string& pluginid)
{
  return mapIdToTests[pluginid];
}

void TestRegistry::ReadTest(IConfigurationElement::Pointer testElem)
{
  ITestDescriptor::Pointer descriptor(new TestDescriptor(testElem));
  poco_assert(descriptor->GetId() != "");
  mapIdToTests[descriptor->GetContributor()].push_back(descriptor);
}

}
