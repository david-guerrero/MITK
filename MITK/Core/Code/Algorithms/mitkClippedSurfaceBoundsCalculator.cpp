#include "mitkClippedSurfaceBoundsCalculator.h"
#include "mitkLine.h"

#define ROUND_P(x) ((int)((x)+0.5))

mitk::ClippedSurfaceBoundsCalculator::ClippedSurfaceBoundsCalculator(
    const mitk::PlaneGeometry* geometry, 
    mitk::Image::Pointer image)
{
  // initialize with meaningless slice indices
  m_MinMaxOutput.clear();
  for(int i = 0; i < 3; i++)
  {
    m_MinMaxOutput.push_back(
        OutputType( std::numeric_limits<int>::max() , 
                    std::numeric_limits<int>::min() ));
  }


  this->SetInput(geometry, image);
}

mitk::ClippedSurfaceBoundsCalculator::~ClippedSurfaceBoundsCalculator()
{
}

void 
mitk::ClippedSurfaceBoundsCalculator::SetInput(
    const mitk::PlaneGeometry* geometry, 
    mitk::Image* image)
{
  if(geometry && image)
  {
    this->m_PlaneGeometry = geometry;
    this->m_Image = image;
  }
}

mitk::ClippedSurfaceBoundsCalculator::OutputType 
mitk::ClippedSurfaceBoundsCalculator::GetMinMaxSpatialDirectionX()
{
  return this->m_MinMaxOutput[0];
}

mitk::ClippedSurfaceBoundsCalculator::OutputType 
mitk::ClippedSurfaceBoundsCalculator::GetMinMaxSpatialDirectionY()
{
  return this->m_MinMaxOutput[1];
}

mitk::ClippedSurfaceBoundsCalculator::OutputType 
mitk::ClippedSurfaceBoundsCalculator::GetMinMaxSpatialDirectionZ()
{
  return this->m_MinMaxOutput[2];
}

void mitk::ClippedSurfaceBoundsCalculator::Update()
{
  // SEE HEADER DOCUMENTATION for explanation

  typedef std::vector< std::pair<mitk::Point3D, mitk::Point3D> > EdgesVector;

  this->m_MinMaxOutput.clear();
  for(int i = 0; i < 3; i++)
  {
    this->m_MinMaxOutput.push_back(OutputType( std::numeric_limits<int>::max() , std::numeric_limits<int>::min() ));
  }

  Point3D origin;
  Vector3D xDirection, yDirection, zDirection;
  const Vector3D spacing = m_Image->GetGeometry()->GetSpacing();

  origin = m_Image->GetGeometry()->GetOrigin();           //Left, bottom, front

  //Get axis vector for the spatial directions
  xDirection = m_Image->GetGeometry()->GetAxisVector(1);
  yDirection = m_Image->GetGeometry()->GetAxisVector(0);
  zDirection = m_Image->GetGeometry()->GetAxisVector(2);

  /* 
   *  For the calculation of the intersection points we need as corner points the center-based image coordinates.
   * With the method GetCornerPoint() of the class Geometry3D we only get the corner-based coordinates.
   * Therefore we need to calculate the center-based corner points here. For that we add/substract the corner-
   * based coordinates with the spacing of the geometry3D.
   */
  for( int i = 0; i < 3; i++ )
  {
    if(xDirection[i] < 0)
    {
      xDirection[i] += spacing[i];
    }
    else if( xDirection[i] > 0 )
    {
      xDirection[i] -= spacing[i];
    }

    if(yDirection[i] < 0)
    {
      yDirection[i] += spacing[i];
    }
    else if( yDirection[i] > 0 )
    {
      yDirection[i] -= spacing[i];
    }

    if(zDirection[i] < 0)
    {
      zDirection[i] += spacing[i];
    }
    else if( zDirection[i] > 0 )
    {
      zDirection[i] -= spacing[i];
    }
  }

  Point3D leftBottomFront, leftTopFront, leftBottomBack, leftTopBack;
  Point3D rightBottomFront, rightTopFront, rightBottomBack, rightTopBack;

  leftBottomFront = origin;
  leftTopFront = origin + yDirection;
  leftBottomBack = origin + zDirection;
  leftTopBack = origin + yDirection + zDirection;
  rightBottomFront = origin + xDirection;
  rightTopFront = origin + xDirection + yDirection;
  rightBottomBack = origin + xDirection + zDirection;
  rightTopBack = origin + xDirection + yDirection + zDirection;

  EdgesVector edgesOf3DBox;

  edgesOf3DBox.push_back(std::make_pair(leftBottomBack,     // x = left=xfront, y=bottom=yfront, z=front=zfront
                                        leftTopFront));     // left, top, front

  edgesOf3DBox.push_back(std::make_pair(leftBottomFront,    // left, bottom, front
                                        leftBottomBack));   // left, bottom, back

  edgesOf3DBox.push_back(std::make_pair(leftBottomFront,    // left, bottom, front
                                        rightBottomFront)); // right, bottom, front

  edgesOf3DBox.push_back(std::make_pair(leftTopFront,       // left, top, front
                                        rightTopFront));    // right, top, front

  edgesOf3DBox.push_back(std::make_pair(leftTopFront,       // left, top, front
                                        leftTopBack));      // left, top, back

  edgesOf3DBox.push_back(std::make_pair(rightTopFront,      // right, top, front
                                        rightTopBack));     // right, top, back

  edgesOf3DBox.push_back(std::make_pair(rightTopFront,      // right, top, front
                                        rightBottomFront)); // right, bottom, front

  edgesOf3DBox.push_back(std::make_pair(rightBottomFront,   // right, bottom, front
                                        rightBottomBack));  // right, bottom, back

  edgesOf3DBox.push_back(std::make_pair(rightBottomBack,    // right, bottom, back
                                        leftBottomBack));   // left, bottom, back

  edgesOf3DBox.push_back(std::make_pair(rightBottomBack,    // right, bottom, back
                                        rightTopBack));     // right, top, back

  edgesOf3DBox.push_back(std::make_pair(rightTopBack,       // right, top, back
                                        leftTopBack));      // left, top, back

  edgesOf3DBox.push_back(std::make_pair(leftTopBack,        // left, top, back
                                        leftBottomBack));   // left, bottom, back



  for (EdgesVector::iterator iterator = edgesOf3DBox.begin(); iterator != edgesOf3DBox.end();iterator++)
  {
    Point3D startPoint = (*iterator).first;   // start point of the line
    Point3D endPoint   = (*iterator).second;  // end point of the line
    Vector3D lineDirection = endPoint - startPoint;

    mitk::Line3D line(startPoint, lineDirection);

    Point3D intersectionWorldPoint;
    intersectionWorldPoint.Fill(std::numeric_limits<int>::min());

    // Get intersection point of line and plane geometry
    m_PlaneGeometry->IntersectionPoint(line, intersectionWorldPoint);  

    double t = -1.0;
    bool isIntersectionPointOnLine;
    isIntersectionPointOnLine = m_PlaneGeometry->IntersectionPointParam(line, t);

    mitk::Point3D intersectionIndexPoint;
    //Get index point
    m_Image->GetGeometry()->WorldToIndex(intersectionWorldPoint, intersectionIndexPoint);    

    if(0.0 <= t && t <= 1.0 && isIntersectionPointOnLine)
    {
      for(int dim = 0; dim < 3; dim++)
      {
        // minimum
        //If new point value is lower than old
        if( this->m_MinMaxOutput[dim].first > ROUND_P(intersectionIndexPoint[dim]) )    
        {
            this->m_MinMaxOutput[dim].first = ROUND_P(intersectionIndexPoint[dim]);     //set new value
        }

        // maximum
        //If new point value is higher than old
        if( this->m_MinMaxOutput[dim].second < ROUND_P(intersectionIndexPoint[dim]) ) 
        {
            this->m_MinMaxOutput[dim].second = ROUND_P(intersectionIndexPoint[dim]);     //set new value
        }
      }
    }
  }
}

