/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-05-13 18:06:46 +0200 (Mi, 13 Mai 2009) $
Version:   $Revision: 6590 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MITK_FIBER_BUNDLE_IO_FACTORY_H_HEADER__
#define __MITK_FIBER_BUNDLE_IO_FACTORY_H_HEADER__

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "mitkBaseData.h"
#include "MitkDiffusionImagingExports.h"

//NOTE>umbenennen in internal FiberBundleIOFactory


namespace mitk
{
//##Documentation
//## @brief Create instances of NrrdQBallImageReader objects using an object factory.
//##
//## @ingroup IO
class MitkDiffusionImaging_EXPORT FiberBundleIOFactory : public itk::ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef FiberBundleIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
    
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);
  static FiberBundleIOFactory* FactoryNew() { return new FiberBundleIOFactory;}
  /** Run-time type information (and related methods). */
  itkTypeMacro(FiberBundleIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    FiberBundleIOFactory::Pointer FiberBundleIOFactory = FiberBundleIOFactory::New();
    ObjectFactoryBase::RegisterFactory(FiberBundleIOFactory);
  }
  
protected:
  FiberBundleIOFactory();
  ~FiberBundleIOFactory();

private:
  FiberBundleIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
  
} // end namespace mitk

#endif  // __MITK_FIBER_BUNDLE_IO_FACTORY_H_HEADER__
