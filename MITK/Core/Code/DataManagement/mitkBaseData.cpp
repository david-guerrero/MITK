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


#include "mitkBaseData.h"
#include <itkObjectFactoryBase.h>
#include <itkSmartPointerForwardReference.txx>

template class MITK_CORE_EXPORT itk::SmartPointerForwardReference<mitk::BaseProcess>;

#define MITK_WEAKPOINTER_PROBLEM_WORKAROUND_ENABLED

mitk::BaseData::BaseData() : 
  m_RequestedRegionInitialized(false), m_SmartSourcePointer(NULL), 
  m_SourceOutputIndexDuplicate(0), m_Initialized(true), 
  m_Unregistering(false), m_CalculatingExternalReferenceCount(false), 
  m_ExternalReferenceCount(-1)
{
  m_TimeSlicedGeometry = TimeSlicedGeometry::New();
  m_PropertyList = PropertyList::New(); 
}

mitk::BaseData::BaseData( const BaseData &other ): 
m_RequestedRegionInitialized(other.m_RequestedRegionInitialized), 
m_SmartSourcePointer(other.m_SmartSourcePointer), 
m_SourceOutputIndexDuplicate(other.m_SourceOutputIndexDuplicate),
m_Initialized(other.m_Initialized), m_Unregistering(other.m_Unregistering),
m_CalculatingExternalReferenceCount(other.m_CalculatingExternalReferenceCount),
m_ExternalReferenceCount(other.m_ExternalReferenceCount)
{
  m_TimeSlicedGeometry = dynamic_cast<mitk::TimeSlicedGeometry*>(other.m_TimeSlicedGeometry->Clone().GetPointer());
  m_PropertyList = other.m_PropertyList->Clone(); 
}

mitk::BaseData::~BaseData() 
{
  m_SmartSourcePointer = NULL;
}

void mitk::BaseData::InitializeTimeSlicedGeometry(unsigned int timeSteps)
{
  mitk::TimeSlicedGeometry::Pointer timeGeometry = this->GetTimeSlicedGeometry();

  mitk::Geometry3D::Pointer g3d = mitk::Geometry3D::New();
  g3d->Initialize();

 if ( timeSteps > 1 )
 {
    mitk::ScalarType timeBounds[] = {0.0, 1.0};
    g3d->SetTimeBounds( timeBounds );
 }

  // The geometry is propagated automatically to the other items,
  // if EvenlyTimed is true...
  timeGeometry->InitializeEvenlyTimed( g3d.GetPointer(), timeSteps );
}

void mitk::BaseData::UpdateOutputInformation()
{
  if ( this->GetSource() )
  {
    this->GetSource()->UpdateOutputInformation();
  }
  if(m_TimeSlicedGeometry.IsNotNull())
    m_TimeSlicedGeometry->UpdateInformation();
}

const mitk::TimeSlicedGeometry* mitk::BaseData::GetUpdatedTimeSlicedGeometry()
{
  SetRequestedRegionToLargestPossibleRegion();

  UpdateOutputInformation();

  return GetTimeSlicedGeometry();
}

void mitk::BaseData::Expand( unsigned int timeSteps )
{
  if( m_TimeSlicedGeometry.IsNotNull() )
    m_TimeSlicedGeometry->ExpandToNumberOfTimeSteps( timeSteps );
}

const mitk::Geometry3D* mitk::BaseData::GetUpdatedGeometry(int t)
{
  SetRequestedRegionToLargestPossibleRegion();

  UpdateOutputInformation();

  return GetGeometry(t);
}

void mitk::BaseData::SetGeometry(Geometry3D* aGeometry3D)
{
if(aGeometry3D!=NULL)
  {
    TimeSlicedGeometry::Pointer timeSlicedGeometry = dynamic_cast<TimeSlicedGeometry*>(aGeometry3D);
    if ( timeSlicedGeometry.IsNotNull() )
      m_TimeSlicedGeometry = timeSlicedGeometry;
    else
    {
      timeSlicedGeometry = TimeSlicedGeometry::New();
      m_TimeSlicedGeometry = timeSlicedGeometry;   
      timeSlicedGeometry->InitializeEvenlyTimed(aGeometry3D, 1);
    }
    Modified();
  }
  else if( m_TimeSlicedGeometry.IsNotNull() ) 
  {
    m_TimeSlicedGeometry = NULL;
    Modified();
  }
  return;
}

void mitk::BaseData::SetClonedGeometry(const Geometry3D* aGeometry3D)
{
  SetGeometry(static_cast<mitk::Geometry3D*>(aGeometry3D->Clone().GetPointer()));
}

void mitk::BaseData::SetClonedGeometry(const Geometry3D* aGeometry3D, unsigned int time)
{
  if (m_TimeSlicedGeometry)
  {
    m_TimeSlicedGeometry->SetGeometry3D(static_cast<mitk::Geometry3D*>(aGeometry3D->Clone().GetPointer()), time);
  }
}

bool mitk::BaseData::IsEmptyTimeStep(unsigned int) const
{
  return IsInitialized() == false;
}

bool mitk::BaseData::IsEmpty() const
{
  if(IsInitialized() == false)
    return true;
  const TimeSlicedGeometry* timeGeometry = const_cast<BaseData*>(this)->GetUpdatedTimeSlicedGeometry();
  if(timeGeometry == NULL)
    return true;
  unsigned int timeSteps = timeGeometry->GetTimeSteps();
  for ( unsigned int t = 0 ; t < timeSteps ; ++t )
  {
    if(IsEmptyTimeStep(t) == false)
      return false;
  }
  return true;
}

itk::SmartPointerForwardReference<mitk::BaseProcess> mitk::BaseData::GetSource() const
{
  return static_cast<mitk::BaseProcess*>(Superclass::GetSource().GetPointer());
}

int mitk::BaseData::GetExternalReferenceCount() const
{
  if(m_CalculatingExternalReferenceCount==false) //this is only needed because a smart-pointer to m_Outputs (private!!) must be created by calling GetOutputs.
  {
    m_CalculatingExternalReferenceCount = true;

    m_ExternalReferenceCount = -1;

    int realReferenceCount = GetReferenceCount();

    if(GetSource()==NULL) 
    {
      m_ExternalReferenceCount = realReferenceCount;
      m_CalculatingExternalReferenceCount = false;
      return m_ExternalReferenceCount;
    }

    mitk::BaseProcess::DataObjectPointerArray outputs = m_SmartSourcePointer->GetOutputs();

    unsigned int idx;
    for (idx = 0; idx < outputs.size(); ++idx)
    {
      //references of outputs that are not referenced from someone else (reference additional to the reference from this BaseProcess object) are interpreted as non-existent 
      if(outputs[idx]==this)
        --realReferenceCount;
    }
    m_ExternalReferenceCount = realReferenceCount;
    if(m_ExternalReferenceCount<0)
      m_ExternalReferenceCount=0;
    m_CalculatingExternalReferenceCount = false;
  }
  else
    return -1;
  return m_ExternalReferenceCount;
}

void mitk::BaseData::UnRegister() const
{
#ifdef MITK_WEAKPOINTER_PROBLEM_WORKAROUND_ENABLED
  if(GetReferenceCount()>1)
  {
    Superclass::UnRegister();
    if((m_Unregistering==false) && (m_SmartSourcePointer.IsNotNull()))
    {
      m_Unregistering=true;
      // the order of the following boolean statement is important:
      // this->GetSource() returns a SmartPointerForwardReference,
      // which increases and afterwards decreases the reference count,
      // which may result in an ExternalReferenceCount of 0, causing
      // BaseProcess::UnRegister() to destroy us (also we already
      // about to do that).
      if((this->m_SmartSourcePointer->GetExternalReferenceCount()==0) || (this->GetSource()==NULL))
        m_SmartSourcePointer=NULL; // now the reference count is zero and this object has been destroyed; thus nothing may be done after this line!!
      else
        m_Unregistering=false;
    }
  }
  else
#endif
    Superclass::UnRegister(); // now the reference count is zero and this object has been destroyed; thus nothing may be done after this line!!
}

void mitk::BaseData::ConnectSource(itk::ProcessObject *arg, unsigned int idx) const
{
#ifdef MITK_WEAKPOINTER_PROBLEM_WORKAROUND_ENABLED
  itkDebugMacro( "connecting source  " << arg
    << ", source output index " << idx);

  if ( GetSource() != arg || m_SourceOutputIndexDuplicate != idx)
  {
    m_SmartSourcePointer = dynamic_cast<mitk::BaseProcess*>(arg);
    m_SourceOutputIndexDuplicate = idx;
    Modified();
  }
#endif
}

mitk::PropertyList::Pointer mitk::BaseData::GetPropertyList() const
{
  return m_PropertyList;
}


mitk::BaseProperty::Pointer mitk::BaseData::GetProperty(const char *propertyKey) const
{
  return m_PropertyList->GetProperty(propertyKey);
}

void mitk::BaseData::SetProperty(const char *propertyKey,
                                 BaseProperty* propertyValue)
{
  m_PropertyList->SetProperty(propertyKey, propertyValue);
}

void mitk::BaseData::SetPropertyList(PropertyList *pList)
{
  m_PropertyList = pList;
}

void mitk::BaseData::SetOrigin(const mitk::Point3D& origin)
{
  mitk::TimeSlicedGeometry* timeSlicedGeometry = GetTimeSlicedGeometry();
  
  assert(timeSlicedGeometry!=NULL);

  mitk::Geometry3D* geometry;

  unsigned int steps = timeSlicedGeometry->GetTimeSteps();

  for(unsigned int timestep = 0; timestep < steps; ++timestep)
  {
    geometry = GetGeometry(timestep);
    if(geometry != NULL)
    {
      geometry->SetOrigin(origin);
    }
    if(GetTimeSlicedGeometry()->GetEvenlyTimed())
    {
      GetTimeSlicedGeometry()->InitializeEvenlyTimed(geometry, steps);
      break;
    }
  }
}

unsigned long mitk::BaseData::GetMTime() const
{
  unsigned long time = Superclass::GetMTime();
  if(m_TimeSlicedGeometry.IsNotNull())
  {
    if((time < m_TimeSlicedGeometry->GetMTime()))
    {
      Modified();
      return Superclass::GetMTime();
    }
    //unsigned long geometryTime = m_TimeSlicedGeometry->GetMTime();
    //if(time < geometryTime)
    //{
    //  return geometryTime;
    //}
  }
  return time;
}

void mitk::BaseData::CopyInformation( const itk::DataObject* data )
{
  const Self* bd = dynamic_cast<const Self*>(data);
  if (bd != NULL)
  {
    m_TimeSlicedGeometry = dynamic_cast<TimeSlicedGeometry*>(bd->GetTimeSlicedGeometry()->Clone().GetPointer());
    m_PropertyList = bd->GetPropertyList()->Clone();
  }
  else
  {
    // pointer could not be cast back down; this can be the case if your filters input
    // and output objects differ in type; then you have to write your own GenerateOutputInformation method
    itkExceptionMacro(<< "mitk::BaseData::CopyInformation() cannot cast "
      << typeid(data).name() << " to "
      << typeid(Self*).name() );
  }

}

bool mitk::BaseData::IsInitialized() const
{
  return m_Initialized;
}

void mitk::BaseData::Clear()
{
  this->ClearData();
  this->InitializeEmpty();
}

void mitk::BaseData::ClearData()
{
  if(m_Initialized)
  {
    ReleaseData();
    m_Initialized = false;
  }
}

void mitk::BaseData::ExecuteOperation(mitk::Operation* /*operation*/)
{
  //empty by default. override if needed!
}

void mitk::BaseData::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << std::endl;
  os << indent << " TimeSlicedGeometry: ";
  if(GetTimeSlicedGeometry() == NULL)
    os << "NULL" << std::endl;
  else
    GetTimeSlicedGeometry()->Print(os, indent);
}

