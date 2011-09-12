/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2010-06-18 20:12:43 +0200 (Fr, 18 Jun 2010) $
Version:   $Revision: 23881 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "mitkImageCaster.h"
#include "mitkImageAccessByItk.h"


vtkRenderer* mitk::RendererAccess::m_3DRenderer = 0;

void mitk::RendererAccess::Set3DRenderer(vtkRenderer* renwin4)
{
  m_3DRenderer = renwin4;
}

vtkRenderer* mitk::RendererAccess::Get3DRenderer()
{
  return m_3DRenderer;
}

void mitk::ImageCaster::CastBaseData(mitk::BaseData* const mitkBaseData, itk::SmartPointer<mitk::Image>& mitkoutputimage){
    try
    {
        mitkoutputimage = dynamic_cast<mitk::Image*>(mitkBaseData);
    }
    catch(...)
    {
        return;
    }
}

void mitk::Caster::Cast(mitk::BaseData* mitkBaseData, mitk::Surface* surface){
        surface = dynamic_cast<mitk::Surface*>(mitkBaseData);
}

#define DefineMitkImageCasterMethods(r, data, type) \
  void mitk::ImageCaster::CastToItkImage(const mitk::Image* mitkImage, itk::SmartPointer<itk::Image<MITK_PP_TUPLE_REM(2)type> >& itkOutputImage) { \
    mitk::CastToItkImage<itk::Image<MITK_PP_TUPLE_REM(2)type> >(mitkImage, itkOutputImage); \
  } \
  void mitk::ImageCaster::CastToMitkImage(const itk::Image<MITK_PP_TUPLE_REM(2)type>* itkImage, itk::SmartPointer<mitk::Image>& mitkOutputImage) { \
    mitk::CastToMitkImage<itk::Image<MITK_PP_TUPLE_REM(2)type> >(itkImage, mitkOutputImage); \
  }

MITK_PP_SEQ_FOR_EACH(DefineMitkImageCasterMethods, _, MITK_ACCESSBYITK_TYPES_DIMN_SEQ(2))
MITK_PP_SEQ_FOR_EACH(DefineMitkImageCasterMethods, _, MITK_ACCESSBYITK_TYPES_DIMN_SEQ(3))
