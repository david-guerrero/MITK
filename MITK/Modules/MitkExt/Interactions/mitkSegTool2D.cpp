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

#include "mitkSegTool2D.h"
#include "mitkToolManager.h"

#include "mitkDataStorage.h"
#include "mitkBaseRenderer.h"

#include "mitkPlaneGeometry.h"

#include "mitkExtractImageFilter.h"
#include "mitkExtractDirectedPlaneImageFilter.h"

//Include of the new ImageExtractor
#include "mitkExtractDirectedPlaneImageFilterNew.h"
#include "mitkPlanarCircle.h"


#define ROUND(a)     ((a)>0 ? (int)((a)+0.5) : -(int)(0.5-(a)))

mitk::SegTool2D::SegTool2D(const char* type)
:Tool(type),
 m_LastEventSender(NULL),
 m_LastEventSlice(0),
 m_Contourmarkername ("Contourmarker"),
 m_RememberContourPositions (false)
{
  // great magic numbers
  CONNECT_ACTION( 80, OnMousePressed );
  CONNECT_ACTION( 90, OnMouseMoved );
  CONNECT_ACTION( 42, OnMouseReleased );
  CONNECT_ACTION( 49014, OnInvertLogic );
}

mitk::SegTool2D::~SegTool2D()
{
}

bool mitk::SegTool2D::OnMousePressed (Action*, const StateEvent* stateEvent)
{
  const PositionEvent* positionEvent = dynamic_cast<const PositionEvent*>(stateEvent->GetEvent());
  if (!positionEvent) return false;

  if ( positionEvent->GetSender()->GetMapperID() != BaseRenderer::Standard2D ) return false; // we don't want anything but 2D

  m_LastEventSender = positionEvent->GetSender();
  m_LastEventSlice = m_LastEventSender->GetSlice();

  return true;
}

bool mitk::SegTool2D::OnMouseMoved   (Action*, const StateEvent* stateEvent)
{
  const PositionEvent* positionEvent = dynamic_cast<const PositionEvent*>(stateEvent->GetEvent());
  if (!positionEvent) return false;
  
  if ( m_LastEventSender != positionEvent->GetSender() ) return false;
  if ( m_LastEventSlice  != m_LastEventSender->GetSlice() ) return false;

  return true;
}

bool mitk::SegTool2D::OnMouseReleased(Action*, const StateEvent* stateEvent)
{
  const PositionEvent* positionEvent = dynamic_cast<const PositionEvent*>(stateEvent->GetEvent());
  if (!positionEvent) return false;

  if ( m_LastEventSender != positionEvent->GetSender() ) return false;
  if ( m_LastEventSlice  != m_LastEventSender->GetSlice() ) return false;
  
  return true;
}
    
bool mitk::SegTool2D::OnInvertLogic(Action*, const StateEvent* stateEvent)
{
  const PositionEvent* positionEvent = dynamic_cast<const PositionEvent*>(stateEvent->GetEvent());
  if (!positionEvent) return false;
  
  if ( m_LastEventSender != positionEvent->GetSender() ) return false;
  if ( m_LastEventSlice  != m_LastEventSender->GetSlice() ) return false;

  return true;
}

bool mitk::SegTool2D::DetermineAffectedImageSlice( const Image* image, const PlaneGeometry* plane, int& affectedDimension, int& affectedSlice )
{
  assert(image);
  assert(plane);

  // compare normal of plane to the three axis vectors of the image
  Vector3D normal       = plane->GetNormal();
  Vector3D imageNormal0 = image->GetSlicedGeometry()->GetAxisVector(0);
  Vector3D imageNormal1 = image->GetSlicedGeometry()->GetAxisVector(1);
  Vector3D imageNormal2 = image->GetSlicedGeometry()->GetAxisVector(2);

  normal.Normalize();
  imageNormal0.Normalize();
  imageNormal1.Normalize();
  imageNormal2.Normalize();

  imageNormal0.Set_vnl_vector( vnl_cross_3d<ScalarType>(normal.Get_vnl_vector(),imageNormal0.Get_vnl_vector()) );
  imageNormal1.Set_vnl_vector( vnl_cross_3d<ScalarType>(normal.Get_vnl_vector(),imageNormal1.Get_vnl_vector()) );
  imageNormal2.Set_vnl_vector( vnl_cross_3d<ScalarType>(normal.Get_vnl_vector(),imageNormal2.Get_vnl_vector()) );

  double eps( 0.00001 );
  // transversal
  if ( imageNormal2.GetNorm() <= eps )
  {
    affectedDimension = 2;
  }
  // sagittal
  else if ( imageNormal1.GetNorm() <= eps )
  {
    affectedDimension = 1;
  }
  // frontal
  else if ( imageNormal0.GetNorm() <= eps )
  {
    affectedDimension = 0;
  }
  else
  {
    affectedDimension = -1; // no idea
    return false;
  }

  // determine slice number in image
  Geometry3D* imageGeometry = image->GetGeometry(0);
  Point3D testPoint = imageGeometry->GetCenter();
  Point3D projectedPoint;
  plane->Project( testPoint, projectedPoint );

  Point3D indexPoint;

  imageGeometry->WorldToIndex( projectedPoint, indexPoint );
  affectedSlice = ROUND( indexPoint[affectedDimension] );
  MITK_DEBUG << "indexPoint " << indexPoint << " affectedDimension " << affectedDimension << " affectedSlice " << affectedSlice;

  // check if this index is still within the image
  if ( affectedSlice < 0 || affectedSlice >= static_cast<int>(image->GetDimension(affectedDimension)) ) return false;
 
  return true;
}
    
mitk::Image::Pointer mitk::SegTool2D::GetAffectedImageSliceAs2DImage(const PositionEvent* positionEvent, const Image* image)
{
  if (!positionEvent) return NULL;
  
  assert( positionEvent->GetSender() ); // sure, right?
  unsigned int timeStep = positionEvent->GetSender()->GetTimeStep( image ); // get the timestep of the visible part (time-wise) of the image

  // first, we determine, which slice is affected
  const PlaneGeometry* planeGeometry( dynamic_cast<const PlaneGeometry*> (positionEvent->GetSender()->GetCurrentWorldGeometry2D() ) );

  if ( !image || !planeGeometry ) return NULL;

  int affectedDimension( -1 );
  int affectedSlice( -1 );
  DetermineAffectedImageSlice( image, planeGeometry, affectedDimension, affectedSlice );
  if ( DetermineAffectedImageSlice( image, planeGeometry, affectedDimension, affectedSlice ) )
  {
    try
    {
      // now we extract the correct slice from the volume, resulting in a 2D image
      ExtractImageFilter::Pointer extractor= ExtractImageFilter::New();
      extractor->SetInput( image );
      extractor->SetSliceDimension( affectedDimension );
      extractor->SetSliceIndex( affectedSlice );
      extractor->SetTimeStep( timeStep );
      extractor->Update();

      // here we have a single slice that can be modified
      Image::Pointer slice = extractor->GetOutput();
      
      return slice;
    }
    catch(...)
    {
      // not working
      return NULL;
    }
  }
  else
  {
      ExtractDirectedPlaneImageFilterNew::Pointer newExtractor = ExtractDirectedPlaneImageFilterNew::New();
      newExtractor->SetInput( image );
      newExtractor->SetActualInputTimestep( timeStep );
      newExtractor->SetCurrentWorldGeometry2D( planeGeometry );
      newExtractor->Update();
      Image::Pointer slice = newExtractor->GetOutput();
      return slice;
  }
}

mitk::Image::Pointer mitk::SegTool2D::GetAffectedWorkingSlice(const PositionEvent* positionEvent)
{
  DataNode* workingNode( m_ToolManager->GetWorkingData(0) );
  if ( !workingNode ) return NULL;
  
  Image* workingImage = dynamic_cast<Image*>(workingNode->GetData());
  if ( !workingImage ) return NULL;
  
  return GetAffectedImageSliceAs2DImage( positionEvent, workingImage );
}

mitk::Image::Pointer mitk::SegTool2D::GetAffectedReferenceSlice(const PositionEvent* positionEvent)
{
  DataNode* referenceNode( m_ToolManager->GetReferenceData(0) );
  if ( !referenceNode ) return NULL;
  
  Image* referenceImage = dynamic_cast<Image*>(referenceNode->GetData());
  if ( !referenceImage ) return NULL;
  
  return GetAffectedImageSliceAs2DImage( positionEvent, referenceImage );
}

void mitk::SegTool2D::AddContourmarker ( const PositionEvent* positionEvent )
{
    mitk::PlaneGeometry* currentGeometry2D = dynamic_cast<mitk::PlaneGeometry*>( const_cast<mitk::Geometry2D*>(positionEvent->GetSender()->GetCurrentWorldGeometry2D()));
    if ( ( currentGeometry2D != NULL ) && ( !ContourmarkerAlreadyExists( currentGeometry2D )) )
    {
        //Creating PlanarFigure which serves as marker
        mitk::PlanarCircle::Pointer contourMarker = mitk::PlanarCircle::New();
        contourMarker->SetGeometry2D( currentGeometry2D );

        //Here we check which consecutive number must be the suffix to the markers name
        std::stringstream markerStream;
        markerStream << m_Contourmarkername;
        int markerCount = 0;
        mitk::DataNode* workingNode (m_ToolManager->GetWorkingData(0));
        while (m_ToolManager->GetDataStorage()->GetNamedDerivedNode(markerStream.str().c_str(), workingNode))
        {
            m_ToolManager->GetDataStorage()->GetNamedDerivedNode(markerStream.str().c_str(),workingNode)->SetProperty( "visible", mitk::BoolProperty::New(false) );
            markerCount++;
            markerStream.str("");
            markerStream.clear();
            markerStream << m_Contourmarkername ;
            markerStream << " ";
            markerStream << markerCount;
        }

        //Now we place the figure in the image and the DataManager as a new Node
        Point2D controlPoint2D;
        Point3D controlPoint3D = positionEvent->GetWorldPosition();
        currentGeometry2D->Map(controlPoint3D ,controlPoint2D);
        DataNode::Pointer rotatedContourNode = DataNode::New();
        rotatedContourNode->SetData(contourMarker);
        rotatedContourNode->SetProperty( "name", StringProperty::New(markerStream.str()) );
        rotatedContourNode->SetBoolProperty( "PlanarFigureInitializedWindow", true, positionEvent->GetSender() );
        rotatedContourNode->SetProperty( "includeInBoundingBox", BoolProperty::New(false));
        m_ToolManager->GetDataStorage()->Add(rotatedContourNode, workingNode);
    }
}

bool mitk::SegTool2D::ContourmarkerAlreadyExists ( const mitk::PlaneGeometry* currentGeometry2D )
{
    itk::VectorContainer<unsigned int, mitk::DataNode::Pointer>::ConstPointer allNodes = m_ToolManager->GetDataStorage()->GetDerivations( m_ToolManager->GetWorkingData(0) );
    mitk::DataNode* currentNode;
    for( unsigned int i = 0; i < allNodes->Size(); i++ )
    {
        currentNode = allNodes->GetElement(i);
        std::string nodeName = currentNode->GetName();
        const mitk::PlaneGeometry* nodeGeometry = dynamic_cast<mitk::PlaneGeometry*>( currentNode->GetData()->GetGeometry() );
        if ( !nodeGeometry ) continue;
        Point3D nodeCenter = nodeGeometry->GetCenter();
        Point3D currentCenter = currentGeometry2D->GetCenter();
        /*mitk::ScalarType distance1 = currentGeometry2D->DistanceFromPlane(nodeCenter);
        mitk::ScalarType distance2 = nodeGeometry->DistanceFromPlane(currentCenter);*/
        Vector3D nodeNormal = nodeGeometry->GetNormal();
        Vector3D currentNormal = currentGeometry2D->GetNormal();
        //Timestep...

        bool isSameGeometry = ( (nodeCenter == currentCenter) && (nodeNormal == currentNormal));


        /*bool isSameGeometry = ( ( nodeGeometry->GetOrigin() == currentGeometry2D->GetOrigin() ) && 
            ( nodeGeometry->GetBounds() == currentGeometry2D->GetBounds() ) &&
            ( nodeGeometry->GetIndexToWorldTransform()->GetMatrix() == currentGeometry2D->GetIndexToWorldTransform()->GetMatrix() ) && 
            ( nodeGeometry->GetIndexToWorldTransform()->GetOffset() == currentGeometry2D->GetIndexToWorldTransform()->GetOffset() ) &&
            ( nodeGeometry->GetSpacing() == currentGeometry2D->GetSpacing() ) &&
            ( nodeGeometry->GetTimeBounds() ) == currentGeometry2D->GetTimeBounds() ) &&
            ( nodeGeometry->GetImageGeometry() == currentGeometry2D->GetImageGeometry() );*/

        int stringPosition = nodeName.find( m_Contourmarkername );
        if ( ( stringPosition == 0 ) && ( isSameGeometry ) )
            return true;
    }

    return false;
}


void mitk::SegTool2D::InteractiveSegmentationBugMessage( const std::string& message )
{
  MITK_ERROR << "********************************************************************************" << std::endl
            << " " << message << std::endl
            << "********************************************************************************" << std::endl
            << "  " << std::endl
            << " If your image is rotated or the 2D views don't really contain the patient image, try to press the button next to the image selection. " << std::endl
            << "  " << std::endl
            << " Please file a BUG REPORT: " << std::endl
            << " http://bugs.mitk.org" << std::endl
            << " Contain the following information:" << std::endl
            << "  - What image were you working on?" << std::endl
            << "  - Which region of the image?" << std::endl
            << "  - Which tool did you use?" << std::endl
            << "  - What did you do?" << std::endl
            << "  - What happened (not)? What did you expect?" << std::endl;
}

