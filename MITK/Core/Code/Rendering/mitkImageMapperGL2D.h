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


#ifndef MITKIMAGEMAPPER2D_H_HEADER_INCLUDED_C10E906E
#define MITKIMAGEMAPPER2D_H_HEADER_INCLUDED_C10E906E

#include "mitkCommon.h"
#include "mitkGLMapper2D.h"
#include "mitkBaseRenderer.h"

#include <map>
#include <assert.h>

#include <itkCommand.h>
#include <itkEventObject.h>

#include <vtkSystemIncludes.h>


class iil4mitkPicImage;
class Vtk2itk;
class vtkImageReslice;
class vtkLookupTable;
class vtkGeneralTransform;
class vtkImageChangeInformation;
class vtkPoints;
class vtkMitkThickSlicesFilter;

namespace mitk {



/** \brief Mapper to resample and display 2D slices of a 3D image.
 *
 * Currently implemented for mapping on PlaneGeometry and
 * AbstractTransformGeometry. The resulting 2D image (by reslicing the
 * underlying 3D input image appropriately) can either be directly rendered
 * in a 2D view or just be calculated to be used later on by another
 * rendering entity, e.g. in texture mapping in a 3D view.
 *
 * This results in a flipped version when used for texture mapping. Furthermore,
 * not the complete rectangular area described by the Geometry2D from the renderer
 * is resampled, @em if the Geometry2D is larger than the image dimension in the
 * requested direction. This results in a stretched version when used for texture
 * mapping.
 *

 * Properties that can be set for images and influence the imageMapper2D are:
 *
 *   - \b "modality": (mitkModalityProperty) Modality of the image
 *   - \b "opacity": (FloatProperty) Opacity of the image
 *   - \b "color": (ColorProperty) Color of the image
 *   - \b "use color": (BoolProperty) Use the color of the image or not
 *   - \b "binary": (BoolProperty) is the image a binary image or not
 *   - \b "outline binary": (BoolProperty) show outline of the image or not
 *   - \b "texture interpolation": (BoolProperty) texture interpolation of the image
 *   - \b "reslice interpolation": (VtkResliceInterpolationProperty) reslice interpolation of the image
 *   - \b "in plane resample extent by geometry": (BoolProperty) Do it or not
 *   - \b "bounding box": (BoolProperty) Is the Bounding Box of the image shown or not
 *   - \b "layer": (IntProperty) Layer of the image
 *   - \b "volume annotation color": (ColorProperty) color of the volume annotation
 *   - \b "volume annotation unit": (StringProperty) annotation unit as string (does not implicit convert the unit!) 
          unit is ml/cm3

 * The default properties are:

 *   - \b "opacity", mitk::FloatProperty::New(0.3f), renderer, overwrite )
 *   - \b "color", ColorProperty::New(1.0,0.0,0.0), renderer, overwrite )
 *   - \b "use color", mitk::BoolProperty::New( true ), renderer, overwrite )
 *   - \b "binary", mitk::BoolProperty::New( true ), renderer, overwrite )
 *   - \b "outline binary", mitk::BoolProperty::New( false ), renderer, overwrite )
 *   - \b "texture interpolation", mitk::BoolProperty::New( mitk::DataNodeFactory::m_TextureInterpolationActive ) )
 *   - \b "reslice interpolation", mitk::VtkResliceInterpolationProperty::New() )
 *   - \b "in plane resample extent by geometry", mitk::BoolProperty::New( false ) )
 *   - \b "bounding box", mitk::BoolProperty::New( false ) )
 *   - \b "layer", mitk::IntProperty::New(10), renderer, overwrite)

 * If the modality-property is set for an image, the mapper uses modality-specific default properties,
 * e.g. color maps, if they are defined.

 * \ingroup Mapper
 */
class MITK_CORE_EXPORT ImageMapperGL2D : public GLMapper2D
{

public:
  
  /** Standard class typedefs. */
  mitkClassMacro( ImageMapperGL2D,GLMapper2D );
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Some convenient typedefs. */
  typedef mitk::Image                    InputImageType;
  typedef InputImageType::Pointer        InputImagePointer;
  typedef InputImageType::ConstPointer   InputImageConstPointer;
  typedef SlicedData::RegionType         InputImageRegionType;

  /** \brief Get the Image to map */
  const InputImageType *GetInput(void);

  /** \brief Calls Update() for all associated renderers. */
  virtual void GenerateAllData();

  /** \brief Renders the (priorly) resampled image onto the screen. */
  virtual void Paint( mitk::BaseRenderer *renderer );

  /** \brief Checks whether this mapper needs to update itself and generate
   * data. */
  virtual void Update(mitk::BaseRenderer * renderer);

  virtual void ApplyProperties(mitk::BaseRenderer* renderer);

  /** \brief Internal storage class for data needed for rendering into a
   * renderer
   */
  class MITK_CORE_EXPORT RendererInfo
  {
    /** \brief internal id of the renderer the data is stored for */
    int m_RendererID;

    /** \brief stored iil4mitkPicImage containing the texture to display for
     * 2D rendering (cf. m_Image) */
    iil4mitkPicImage* m_iil4mitkImage;
    mitk::BaseRenderer* m_Renderer;

  public:
    /** \brief timestamp of last update of stored data */
    itk::TimeStamp m_LastUpdateTime;

    /** \brief stored data as a mitkIpPicDescriptor */
    mitkIpPicDescriptor *m_Pic;

    /** \brief number of pixels per mm in x- and y-direction of the resampled */
    Vector2D m_PixelsPerMM;

    /** \brief Extent (in pixels) of the image */
    Vector2D m_Extent;

    /** \brief Overlap (in pixels) to ensure coverage of rotated images also */
    Vector2D m_Overlap;

    /** \brief Using unit spacing for resampling makes life easier */
    vtkImageChangeInformation *m_UnitSpacingImageFilter;

    /** \brief The actual reslicer (one per renderer) */
    vtkImageReslice *m_Reslicer;

    /** \brief Thickslices post filtering */
    vtkMitkThickSlicesFilter *m_TSFilter;

    /** \brief Extracted image for 3D rendering (cf. m_iil4mitkImage) */
    vtkImageData *m_Image;

    /** \brief Reference geometry associated with the world geometry */
    const Geometry3D *m_ReferenceGeometry;
   
    bool m_TextureInterpolation;

    /** \brief stores the id of the observer for delete event of renderer */
    unsigned long m_ObserverID;

    RendererInfo();

    ~RendererInfo();

    inline bool IsInitialized() const
    {
      return m_RendererID >= 0;
    }

    void Initialize( int rendererID, mitk::BaseRenderer *renderer, 
      unsigned long observerID );

    void Set_iil4mitkImage(iil4mitkPicImage* iil4mitkImage);

    inline iil4mitkPicImage* Get_iil4mitkImage() const
    {
      return m_iil4mitkImage;
    }

    inline int GetRendererID() const
    {
      return m_RendererID;
    }

    void RemoveObserver();

    void Squeeze();
  }; // RendererInfo



  /** \brief Get the internal id of the renderer
   * \sa RendererInfo
   */
  virtual int GetAssociatedChannelNr( mitk::BaseRenderer *renderer );

  /** \brief Get the RendererInfo for \a renderer */
  const RendererInfo *GetRendererInfo( mitk::BaseRenderer *renderer )
  {
    return &this->AccessRendererInfo(renderer);
  }

  /** \brief Release memory allocated for buffering */
  virtual void Clear();

  static void SetDefaultProperties(mitk::DataNode* node, mitk::BaseRenderer* renderer = NULL, bool overwrite = false);

protected:
  ImageMapperGL2D();

  virtual ~ImageMapperGL2D();

  /** Does the actual resampling, without rendering the image yet. */
  virtual void GenerateDataForRenderer(mitk::BaseRenderer *renderer);
  
  /** \brief Get the RendererInfo for @a renderer */
  inline RendererInfo & AccessRendererInfo( mitk::BaseRenderer* renderer )
  {
    RendererInfo& rendererInfo = m_RendererInfo[renderer];
    if(rendererInfo.IsInitialized()==false)
    {
      // Add observer for renderer reset events (RendererInfo will 
      // automatically be removed from list when a Renderer is deleted)
      //
      // Note: observer ID is passed to rendererInfo, which will take
      // responsiblity to remove the observer upon its destruction
      typedef itk::MemberCommand< ImageMapperGL2D > MemberCommandType;
      MemberCommandType::Pointer deleteRendererCommand = 
        MemberCommandType::New();

      deleteRendererCommand->SetCallbackFunction(
        this, &ImageMapperGL2D::DeleteRendererCallback );
      
      unsigned long observerID = renderer->AddObserver( 
        BaseRenderer::RendererResetEvent(), deleteRendererCommand );

      // Initialize RendererInfo
      rendererInfo.Initialize( ImageMapperGL2D::numRenderer++, renderer, observerID );
    }

    return rendererInfo;
  }

  void DeleteRendererCallback( itk::Object *object, const itk::EventObject & );


  double CalculateSpacing( const mitk::Geometry3D *geometry, 
    const mitk::Vector3D &d ) const;

  bool LineIntersectZero( vtkPoints *points, int p1, int p2, 
    vtkFloatingPointType *bounds );

  bool CalculateClippedPlaneBounds( const Geometry3D *boundingGeometry, 
    const PlaneGeometry *planeGeometry, vtkFloatingPointType *bounds );

  /** \brief Number of renderers data is stored for
   * \todo General concept for keeping data for rendering required
   * \todo static?
   */
  static int numRenderer;

protected:
  typedef std::map<BaseRenderer*,RendererInfo> RendererInfoMap;


  /** \brief Map of instances of RendererInfo
   * \sa RendererInfo
   */
  RendererInfoMap m_RendererInfo;

  vtkGeneralTransform *m_ComposedResliceTransform;

private:
  int m_iil4mitkMode;

};

} // namespace mitk

#endif /* MITKIMAGEMAPPER2D_H_HEADER_INCLUDED_C10E906E */
