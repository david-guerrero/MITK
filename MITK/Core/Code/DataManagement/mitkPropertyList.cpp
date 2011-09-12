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


#include "mitkPropertyList.h"

#include "mitkProperties.h"
#include "mitkStringProperty.h"
#include "mitkVector.h"


mitk::BaseProperty* mitk::PropertyList::GetProperty(const std::string& propertyKey) const
{
    PropertyMap::const_iterator it;
    
    it=m_Properties.find( propertyKey );
    if(it!=m_Properties.end())
      return it->second;
    else 
        return NULL;
}


void mitk::PropertyList::SetProperty(const std::string& propertyKey, BaseProperty* property)
{
  if (!property) return;
  //make sure that BaseProperty*, which may have just been created and never been 
  //assigned to a SmartPointer, is registered/unregistered properly. If we do not
  //do that, it will a) not deleted in case it is identical to the old one or
  //b) possibly deleted when temporarily added to a smartpointer somewhere below.
  BaseProperty::Pointer tmpSmartPointerToProperty = property;

  PropertyMap::iterator it( m_Properties.find( propertyKey ) );
  
  // Is a property with key @a propertyKey contained in the list?
  if( it != m_Properties.end() )
  {
    // yes
    //is the property contained in the list identical to the new one?
    if( it->second == property) 
    {
      // yes? do nothing and return.
      return;
    }

    // compatible? then use operator= to assign value
    if (it->second->Assignable( *property ))
    {
      bool changed = (it->second->GetValueAsString() != property->GetValueAsString());
      *(static_cast<BaseProperty*>(it->second.GetPointer())) = *property;
      // muellerm,20.10: added modified event
      if(changed)
        this->Modified();
      return;
    }
    
    if ( typeid( *(it->second.GetPointer()) ) != typeid( *property ) )
    {
      // Accept only if the two types are matching. For replacing, use 
      // ReplaceProperty.
      MITK_ERROR << "In " __FILE__ ", l." << __LINE__ 
        << ": Trying to set existing property to a property with different type." 
        << " Use ReplaceProperty() instead."
        << std::endl;
      return;
    }

    // If types are identical, but the property has no operator= (s.a.),
    // overwrite the pointer.
    it->second = property;
    return;
  }

  //no? add/replace it.
  PropertyMapElementType newProp;
  newProp.first = propertyKey;
  newProp.second = property;
  m_Properties.insert ( newProp );
  this->Modified();
}


void mitk::PropertyList::ReplaceProperty(const std::string& propertyKey, BaseProperty* property)
{
  if (!property) return;

  PropertyMap::iterator it( m_Properties.find( propertyKey ) );
  
  // Is a property with key @a propertyKey contained in the list?
  if( it != m_Properties.end() )
  {
    it->second=NULL;
    m_Properties.erase(it);
  }

  //no? add/replace it.
  PropertyMapElementType newProp;
  newProp.first = propertyKey;
  newProp.second = property;
  m_Properties.insert ( newProp );
  Modified();
}


mitk::PropertyList::PropertyList()
{
}


mitk::PropertyList::~PropertyList()
{
  Clear();
}


/**
 * Consider the list as changed when any of the properties has changed recently.
 */
unsigned long mitk::PropertyList::GetMTime() const
{
  for ( PropertyMap::const_iterator it = m_Properties.begin() ;
        it != m_Properties.end();
        ++it )
  {
    if( it->second.IsNull() )
    {
      itkWarningMacro(<< "Property '" << it->first <<"' contains nothing (NULL).");
      continue;
    }
    if( Superclass::GetMTime() < it->second->GetMTime() )
    {
      Modified();
      break;
    }
  }
    
  return Superclass::GetMTime();
}


bool mitk::PropertyList::DeleteProperty(const std::string& propertyKey)
{
  PropertyMap::iterator it;  
  it=m_Properties.find( propertyKey );

  if(it!=m_Properties.end())
  {
    it->second=NULL;
    m_Properties.erase(it);
    Modified();
    return true;
  }
  return false;
}


mitk::PropertyList::Pointer mitk::PropertyList::Clone()
{
  mitk::PropertyList::Pointer newPropertyList = PropertyList::New();

  // copy the map
  newPropertyList->m_Properties = m_Properties;

  return newPropertyList.GetPointer();
}


void mitk::PropertyList::Clear()
{
  PropertyMap::iterator it = m_Properties.begin(), end = m_Properties.end();
  while(it!=end)
  {
    it->second = NULL;
    ++it;
  }
  m_Properties.clear();
}


void mitk::PropertyList::ConcatenatePropertyList(PropertyList *pList, bool replace)
{
  if (pList)
  {
    const PropertyMap* propertyMap = pList->GetMap();

    for ( PropertyMap::const_iterator iter = propertyMap->begin(); // m_PropertyList is created in the constructor, so we don't check it here
          iter != propertyMap->end();
          ++iter )
    {
      const std::string key = iter->first;
      BaseProperty* value = iter->second;
      if (replace)
      {
        ReplaceProperty( key.c_str(), value );
      }
      else
      {
        SetProperty( key.c_str(), value );
      }
    }
  }
}

bool mitk::PropertyList::GetBoolProperty(const char* propertyKey, bool& boolValue) const
{
  BoolProperty *gp = dynamic_cast<BoolProperty*>( GetProperty(propertyKey) );
  if ( gp != NULL )
  {
    boolValue = gp->GetValue();
    return true;
  }
  return false;
  // Templated Method does not work on Macs
  //return GetPropertyValue<bool>(propertyKey, boolValue);
}


bool mitk::PropertyList::GetIntProperty(const char* propertyKey, int &intValue) const
{
  IntProperty *gp = dynamic_cast<IntProperty*>( GetProperty(propertyKey) );
  if ( gp != NULL )
  {
    intValue = gp->GetValue();
    return true;
  }
  return false;
  // Templated Method does not work on Macs
  //return GetPropertyValue<int>(propertyKey, intValue);
}


bool mitk::PropertyList::GetFloatProperty(const char* propertyKey, float &floatValue) const
{
  FloatProperty *gp = dynamic_cast<FloatProperty*>( GetProperty(propertyKey) );
  if ( gp != NULL )
  {
    floatValue = gp->GetValue();
    return true;
  }
  return false;
  // Templated Method does not work on Macs
  //return GetPropertyValue<float>(propertyKey, floatValue);
}


bool mitk::PropertyList::GetStringProperty(const char* propertyKey, std::string& stringValue) const
{
  StringProperty* sp= dynamic_cast<StringProperty*>(GetProperty(propertyKey));
  if ( sp != NULL )
  {
    stringValue = sp->GetValue();
    return true;
  }
  return false;
}


void mitk::PropertyList::SetIntProperty(const char* propertyKey, int intValue)
{
  SetProperty(propertyKey, mitk::IntProperty::New(intValue));
}


void mitk::PropertyList::SetBoolProperty( const char* propertyKey, bool boolValue)
{
  SetProperty(propertyKey, mitk::BoolProperty::New(boolValue));
}


void mitk::PropertyList::SetFloatProperty( const char* propertyKey, float floatValue)
{
  SetProperty(propertyKey, mitk::FloatProperty::New(floatValue));
}


void mitk::PropertyList::SetStringProperty( const char* propertyKey, const char* stringValue)
{
  SetProperty(propertyKey, mitk::StringProperty::New(stringValue));
}
