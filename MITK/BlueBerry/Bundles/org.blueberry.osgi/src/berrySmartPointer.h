/*=========================================================================

 Program:   BlueBerry Platform
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

#ifndef BERRYOSGISMARTPOINTER_H_
#define BERRYOSGISMARTPOINTER_H_

#include <iostream>
#include <stdexcept>

#include <org_blueberry_osgi_Export.h>
#include "berryException.h"

#include <berryConfig.h>

#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
#include "berryDebugUtil.h"
#include <Poco/Mutex.h>
#endif

namespace berry
{

template<class T> class WeakPointer;

/** \class SmartPointer
 * \brief Implements transparent reference counting.
 *
 * SmartPointer is a copy of itk::SmartPointer.
 * It implements reference counting by overloading
 * operator -> (and *) among others. This allows natural interface
 * to the class referred to by the pointer without having to invoke
 * special Register()/UnRegister() methods directly.
 *
 */
template<class TObjectType>
class SmartPointer
{
public:
  typedef TObjectType ObjectType;
  typedef SmartPointer Self;

  /** Constructor  */
  SmartPointer() :
    m_Pointer(0)
  {
#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
    DebugInitSmartPointer();
#endif

  }

  /** Constructor to pointer p  */
  explicit SmartPointer(ObjectType *p) :
    m_Pointer(p)
  {
    if (m_Pointer)
      this->Register();

#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
    DebugInitSmartPointer();
#endif
  }

  /** Copy constructor  */
  SmartPointer(const SmartPointer<ObjectType> &p) :
    m_Pointer(p.m_Pointer)
  {
    this->Register();

#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
    DebugInitSmartPointer();
#endif
  }

  template<class Other>
  SmartPointer(const SmartPointer<Other>& ptr) :
    m_Pointer(const_cast<Other*> (ptr.GetPointer()))
  {
    if (m_Pointer)
      this->Register();

#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
    DebugInitSmartPointer();
#endif
  }

  template<class Other>
  explicit SmartPointer(const WeakPointer<Other>& wp)
  {
    if (wp.m_Pointer)
    {
      this->m_Pointer = wp.m_Pointer;
      this->Register();

      #if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
        DebugInitSmartPointer();
      #endif
    }
    else
    {
      throw BadWeakPointerException();
    }
  }

  /** Destructor  */
  ~SmartPointer()
  {
#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
    if (m_Pointer) DebugRemoveSmartPointer();
#endif

    this->UnRegister();
    m_Pointer = 0;
  }

  template<class Other>
  SmartPointer<Other> Cast() const
  {
    Other* pOther = dynamic_cast<Other*> (m_Pointer);
    return SmartPointer<Other> (pOther);
  }

  /** Overload operator ->  */
  ObjectType *operator ->() const
  {
    return m_Pointer;
  }

//  /** Return pointer to object.  */
//  operator ObjectType *() const
//  {
//    return m_Pointer;
//  }

  ObjectType & operator*() const
  {
    poco_assert( m_Pointer != 0 );
    return *m_Pointer;
  }

  /** Test if the pointer has been initialized */
  bool IsNotNull() const
  {
    return m_Pointer != 0;
  }
  bool IsNull() const
  {
    return m_Pointer == 0;
  }

  typedef ObjectType * Self::*unspecified_bool_type;

  operator unspecified_bool_type () const
  {
    return m_Pointer == 0 ? 0: &Self::m_Pointer;
  }

  /** Template comparison operators. */
  template<typename R>
  bool operator ==(const R* o) const
  {
    return (m_Pointer == 0 ? o == 0 : (o && m_Pointer->operator==(o)));
  }

  template<typename R>
  bool operator ==(const SmartPointer<R>& r) const
  {
    const R* o = r.GetPointer();
    return (m_Pointer == 0 ? o == 0 : (o && m_Pointer->operator==(o)));
  }

  bool operator ==(int r) const
  {
    if (r == 0)
      return m_Pointer == 0;

    throw std::invalid_argument("Can only compare to 0");
  }

  template<typename R>
  bool operator !=(const R* r) const
  {
    return !(this->operator==(r));
  }

  template<typename R>
  bool operator !=(const SmartPointer<R>& r) const
  {
    return !(this->operator==(r));
  }

  bool operator !=(int r) const
  {
    if (r == 0)
      return m_Pointer != 0;

    throw std::invalid_argument("Can only compare to 0");
  }

//  /** Template comparison operators using operator==. */
//  template<typename R>
//  bool CompareTo(const SmartPointer<R>& r) const
//  {
//    return m_Pointer == 0 ? r == 0 : r.GetPointer() && m_Pointer->operator==(r.GetPointer());
//  }

//  template<typename R>
//  bool CompareTo(R r) const
//  {
//    //const ObjectType* o = static_cast<const ObjectType*> (r);
//    return m_Pointer == 0 ? r == 0 : (r && m_Pointer->operator==(r));
//  }

  /** Access function to pointer. */
  ObjectType *GetPointer() const
  {
    return m_Pointer;
  }

  /** Comparison of pointers. Less than comparison.  */
  bool operator <(const SmartPointer &r) const
  {
    return (void*) m_Pointer < (void*) r.m_Pointer;
  }

  /** Comparison of pointers. Greater than comparison.  */
  bool operator>(const SmartPointer &r) const
  {
    return (void*) m_Pointer > (void*) r.m_Pointer;
  }

  /** Comparison of pointers. Less than or equal to comparison.  */
  bool operator <=(const SmartPointer &r) const
  {
    return (void*) m_Pointer <= (void*) r.m_Pointer;
  }

  /** Comparison of pointers. Greater than or equal to comparison.  */
  bool operator >=(const SmartPointer &r) const
  {
    return (void*) m_Pointer >= (void*) r.m_Pointer;
  }

  /** Overload operator assignment.  */
  SmartPointer &operator =(const SmartPointer &r)
  {
    return this->operator =(r.GetPointer());
  }

  /** Overload operator assignment. */
  template<typename R>
  SmartPointer &operator =(const SmartPointer<R>& r)
  {
    return this->operator =(r.GetPointer());
  }

  /** Overload operator assignment.  */
  SmartPointer &operator =(ObjectType *r)
  {
    if (m_Pointer != r)
    {
#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)
      DebugAssignSmartPointer(r, m_Pointer);
#endif
      ObjectType* tmp = m_Pointer; //avoid recursive unregisters by retaining temporarily
      m_Pointer = r;
      this->Register();
      if (tmp)
      {
        tmp->UnRegister();
      }
    }
    return *this;
  }

  /** Function to print object pointed to  */
  ObjectType *Print(std::ostream& os) const
  {
    // This prints the object pointed to by the pointer
    (*m_Pointer).Print(os);
    return m_Pointer;
  }

private:

  /** The pointer to the object referred to by this smart pointer. */
  ObjectType* m_Pointer;

  void Register()
  {
    if (m_Pointer)
    {
      m_Pointer->Register();
    }
  }

  void UnRegister()
  {
    if (m_Pointer)
    {
      m_Pointer->UnRegister();
    }
  }

#if defined(BLUEBERRY_DEBUG_SMARTPOINTER)

  unsigned int m_Id;
  Poco::FastMutex m_Mutex;

  void DebugInitSmartPointer()
  {
    {
      Poco::FastMutex::ScopedLock lock(m_Mutex);
      if (m_Pointer)
      {
        unsigned int& counter = DebugUtil::GetSmartPointerCounter();
        m_Id = ++counter;
        DebugUtil::RegisterSmartPointer(m_Id, m_Pointer);
      }
      else m_Id = 0;
    }

    //if (DebugUtil::GetSmartPointerCounter() == Platform::GetConfiguration().getInt(Platform::DEBUG_ARG_SMARTPOINTER_ID))
    //throw 1;
  }

  void DebugRemoveSmartPointer()
  {
    Poco::FastMutex::ScopedLock lock(m_Mutex);
    DebugUtil::UnregisterSmartPointer(m_Id, m_Pointer);
  }

  void DebugAssignSmartPointer(const ObjectType* newObject, const ObjectType* oldObject)
  {
    Poco::FastMutex::ScopedLock lock(m_Mutex);
    if (oldObject)
      DebugUtil::UnregisterSmartPointer(m_Id, oldObject);

    if (newObject)
    {
      if (m_Id < 1)
      {
        unsigned int& counter = DebugUtil::GetSmartPointerCounter();
        m_Id = ++counter;
      }
      DebugUtil::RegisterSmartPointer(m_Id, newObject);
    }
  }

public:

  int GetId()
  {
    return m_Id;
  }

private:
#endif
};

template<typename T>
std::ostream& operator<<(std::ostream& os, SmartPointer<T> p)
{
  p.Print(os);
  return os;
}

}

#endif /*BERRYOSGISMARTPOINTER_H_*/
