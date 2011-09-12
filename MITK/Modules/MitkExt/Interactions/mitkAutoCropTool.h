/*=========================================================================
 
Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision: 1.0 $
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.
 
=========================================================================*/

#ifndef mitkAutoCropTool_h_Included
#define mitkAutoCropTool_h_Included

#include "mitkCommon.h"
#include "MitkExtExports.h"
#include "mitkSegmentationsProcessingTool.h"

namespace mitk
{

/**
  \brief Crops selected segmentations.

  \ingroup ToolManagerEtAl
  \sa mitk::Tool
  \sa QmitkInteractiveSegmentation

  Last contributor: $Author$
*/
class MitkExt_EXPORT AutoCropTool : public SegmentationsProcessingTool
{
  public:
    
    mitkClassMacro(AutoCropTool, SegmentationsProcessingTool);
    itkNewMacro(AutoCropTool);

    virtual const char** GetXPM() const;
    virtual const char* GetName() const;

  protected:
    
    virtual bool ProcessOneWorkingData( DataNode* node );
    virtual std::string GetErrorMessage();

    AutoCropTool(); // purposely hidden
    virtual ~AutoCropTool();
};

} // namespace

#endif

