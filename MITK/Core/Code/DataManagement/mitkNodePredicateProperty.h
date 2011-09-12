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


#ifndef MITKNODEPREDICATEPROPERTY_H_HEADER_INCLUDED_
#define MITKNODEPREDICATEPROPERTY_H_HEADER_INCLUDED_

#include "mitkNodePredicateBase.h"
#include "mitkBaseProperty.h"
#include "mitkWeakPointer.h"

namespace mitk {

    //##Documentation
    //## @brief Predicate that evaluates if the given DataNode has a specific property. 
    //## If the second parameter is NULL, it will only be checked whether there is a property with the specified name.
    //## 
    //## 
    //## 
    //## @ingroup DataStorage
    class MITK_CORE_EXPORT NodePredicateProperty : public NodePredicateBase
    {
    public:
      mitkClassMacro(NodePredicateProperty, NodePredicateBase);
      mitkNewMacro1Param(NodePredicateProperty, const char*);
      mitkNewMacro2Param(NodePredicateProperty, const char*, mitk::BaseProperty*);

      //##Documentation
      //## @brief Standard Destructor
      virtual ~NodePredicateProperty();

      //##Documentation
      //## @brief Checks, if the nodes contains a property that is equal to m_ValidProperty
      virtual bool CheckNode(const mitk::DataNode* node) const;

    protected:
      //##Documentation
      //## @brief Constructor to check for a named property
      NodePredicateProperty(const char* propertyName, mitk::BaseProperty* p);

      //##Documentation
      //## @brief Constructor to check for the existence of a property with a given name
      NodePredicateProperty(const char* propertyName);

      //mitk::WeakPointer<mitk::BaseProperty> m_ValidProperty;
      mitk::BaseProperty::Pointer m_ValidProperty;
      //mitk::BaseProperty* m_ValidProperty;
      std::string m_ValidPropertyName;
    };

} // namespace mitk

#endif /* MITKNODEPREDICATEPROPERTY_H_HEADER_INCLUDED_ */
