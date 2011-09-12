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


#ifndef MITKTRANFERFUNCTIONPROPERTY_H_HEADER_INCLUDED
#define MITKTRANFERFUNCTIONPROPERTY_H_HEADER_INCLUDED

#include "mitkBaseProperty.h"
#include "mitkTransferFunction.h"


namespace mitk {

  class MITK_CORE_EXPORT TransferFunctionProperty : public BaseProperty
  {
    public:
      mitkClassMacro(TransferFunctionProperty, BaseProperty);

      itkNewMacro(TransferFunctionProperty);
      mitkNewMacro1Param(TransferFunctionProperty, mitk::TransferFunction::Pointer);

      itkSetMacro(Value, mitk::TransferFunction::Pointer );
      itkGetConstMacro(Value, mitk::TransferFunction::Pointer );

      /**
       *
       */
      virtual bool operator==(const BaseProperty& property) const {

        const Self *other = dynamic_cast<const Self*>(&property);

        if(other==NULL) 
          return false;
        else
          return *(m_Value.GetPointer()) == *(other->m_Value.GetPointer());
      }
      virtual BaseProperty& operator=(const BaseProperty& other) { return Superclass::operator=(other); }
      /**
       *
       */
      std::string GetValueAsString() const {
        std::stringstream myStr;
        myStr << GetValue() ;
        return myStr.str(); 
      }

      protected:
        mitk::TransferFunction::Pointer m_Value;  
        
        TransferFunctionProperty()
          : BaseProperty()
        {};

        virtual ~TransferFunctionProperty()
        {
        };

        TransferFunctionProperty( mitk::TransferFunction::Pointer value )
          : BaseProperty(), m_Value( value )
        {};
  };


  //  typedef GenericProperty<mitk::TransferFunction::Pointer> TransferFunctionProperty;
} // namespace mitk  

#endif /* MITKTRANFERFUNCTIONPROPERTY_H_HEADER_INCLUDED */ 
