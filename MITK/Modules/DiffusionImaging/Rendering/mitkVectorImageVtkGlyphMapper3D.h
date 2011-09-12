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

#ifndef _MITK_VECTOR_IMAGE_VTK_MAPPER_3D__H
#define _MITK_VECTOR_IMAGE_VTK_MAPPER_3D__H

#include "MitkDiffusionImagingExports.h"

#include "mitkVtkMapper3D.h"
#include "mitkImage.h"

class vtkMaskedGlyph3D;
class vtkActor;
class vtkPolyDataMapper;
class vtkMaskPoints;


namespace mitk
{

class MitkDiffusionImaging_EXPORT VectorImageVtkGlyphMapper3D : public VtkMapper3D
{
public:
    mitkClassMacro( VectorImageVtkGlyphMapper3D, VtkMapper3D );

    itkNewMacro( Self );

    enum GlyphType {LineGlyph, ArrowGlyph};
    
    itkSetMacro(MaximumNumberOfPoints, unsigned int);
    itkGetMacro(MaximumNumberOfPoints, unsigned int);
    
    itkSetMacro(UseMaskPoints, bool);
    itkGetMacro(UseMaskPoints, bool);
    itkBooleanMacro(UseMaskPoints);
    
    itkSetMacro(RandomMode, bool);
    itkGetMacro(RandomMode, bool);
    itkBooleanMacro(RandomMode);

    virtual vtkProp* GetVtkProp(mitk::BaseRenderer* renderer);

protected:
    /**
     * Constructor. Doesn't do anything...
     */
    VectorImageVtkGlyphMapper3D();

    /**
     * Destructor
     */
    virtual ~VectorImageVtkGlyphMapper3D();

    /**
     * Generate a vtkPolyData by creating vectors as glyphs
     */
    virtual void GenerateData();

    /**
     * This method is called, each time a specific renderer is updated.
     */
    virtual void GenerateDataForRenderer( mitk::BaseRenderer* renderer );

    /**
     * Returns the input data object of the given filter. In this
     * case, a mitk::Image is returned.
     */
    Image* GetInput();
    
    vtkMaskedGlyph3D* m_Glyph3DGenerator;
    vtkActor* m_Glyph3DActor;
    vtkPolyDataMapper* m_Glyph3DMapper;
    
    GlyphType m_GlyphType;
    unsigned int m_MaximumNumberOfPoints;
    bool m_UseMaskPoints;
    bool m_RandomMode;
    
};


} //end of namespace mitk

#endif



