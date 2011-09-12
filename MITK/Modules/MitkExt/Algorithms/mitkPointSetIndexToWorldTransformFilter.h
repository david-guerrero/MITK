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

#ifndef _MITKPOINTSETINDEXTOWORLDTRANSFORMFILTER_H__
#define _MITKPOINTSETINDEXTOWORLDTRANSFORMFILTER_H__

#include "mitkPointSetToPointSetFilter.h"
#include "mitkPointSet.h"

#include "MitkExtExports.h"

namespace mitk
{
  //##Documentation
  //## @brief Transforms a point set object from index to world coordinates.
  //## Transposes the points coordinates with the object's mitk::Geometry3D to display 
  //## correct scene coordinates.
  //## 
  //## @ingroup Algorithms
class MitkExt_EXPORT PointSetIndexToWorldTransformFilter : public PointSetToPointSetFilter
{
public:
  /**
   * Standard mitk class macro
   */
  mitkClassMacro ( PointSetIndexToWorldTransformFilter, PointSetToPointSetFilter );

  itkNewMacro ( Self );

protected:

  /**
   * Protected constructor. Use ::New instead()
   */
  PointSetIndexToWorldTransformFilter();

  /**
   * Protected destructor. Instances are destroyed when reference count is zero
   */
  virtual ~PointSetIndexToWorldTransformFilter();
  /**
   * method executed when calling Update();
   */
  virtual void GenerateData();
  
};

}

#endif
