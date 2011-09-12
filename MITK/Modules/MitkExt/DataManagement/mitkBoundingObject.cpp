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


#include "mitkBoundingObject.h"
#include "mitkBaseProcess.h"

mitk::BoundingObject::BoundingObject()
  : Surface(), m_Positive(true)
{
//  Initialize(1);

  /* bounding box around the unscaled bounding object */ 
  ScalarType bounds[6]={-1,1,-1,1,-1,1};  //{xmin,x_max, ymin,y_max,zmin,z_max}
  GetGeometry()->SetBounds(bounds);
  GetTimeSlicedGeometry()->UpdateInformation();
}

mitk::BoundingObject::~BoundingObject() 
{
} 

mitk::ScalarType mitk::BoundingObject::GetVolume()
{
  return 0.0;
}

void mitk::BoundingObject::FitGeometry(mitk::Geometry3D* aGeometry3D)
{
  GetGeometry()->SetIdentity();
  GetGeometry()->Compose(aGeometry3D->GetIndexToWorldTransform());
  
  if (aGeometry3D->GetImageGeometry())
  {
    // remove 0.5 offset, if necessary
    aGeometry3D->ChangeImageGeometryConsideringOriginOffset(false); 
    GetGeometry()->SetOrigin(aGeometry3D->GetCenter());
    aGeometry3D->ChangeImageGeometryConsideringOriginOffset(true);
  }
  else
  {
    GetGeometry()->SetOrigin(aGeometry3D->GetCenter());
  }
  

  mitk::Vector3D size;
  for(unsigned int i=0; i < 3; ++i)
    size[i] = aGeometry3D->GetExtentInMM(i)/2.0;
  GetGeometry()->SetSpacing( size );
  GetTimeSlicedGeometry()->UpdateInformation();
}
