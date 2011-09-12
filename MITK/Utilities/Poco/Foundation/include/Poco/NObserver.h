//
// NObserver.h
//
// $Id$
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
//
// Definition of the NObserver class template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_NObserver_INCLUDED
#define Foundation_NObserver_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractObserver.h"


namespace Poco {


template <class C, class N>
class NObserver: public AbstractObserver
	/// This template class implements an adapter that sits between
	/// a NotificationCenter and an object receiving notifications
	/// from it. It is quite similar in concept to the 
	/// RunnableAdapter, but provides some NotificationCenter
	/// specific additional methods.
	/// See the NotificationCenter class for information on how
	/// to use this template class.
	///
	/// This class template is quite similar to the Observer class
	/// template. The only difference is that the NObserver
	/// expects the callback function to accept a const AutoPtr& 
	/// instead of a plain pointer as argument, thus simplifying memory
	/// management.
{
public:
	typedef AutoPtr<N> NotificationPtr;
	typedef void (C::*Callback)(const NotificationPtr&);

	NObserver(C& object, Callback method): 
		_pObject(&object), 
		_method(method)
	{
	}
	
	NObserver(const NObserver& observer):
		AbstractObserver(observer),
		_pObject(observer._pObject), 
		_method(observer._method)
	{
	}
	
	~NObserver()
	{
	}
	
	NObserver& operator = (const NObserver& observer)
	{
		if (&observer != this)
		{
			_pObject = observer._pObject;
			_method  = observer._method;
		}
		return *this;
	}
	
	void notify(Notification* pNf) const
	{
		N* pCastNf = dynamic_cast<N*>(pNf);
		if (pCastNf)
		{
			NotificationPtr ptr(pCastNf, true);
			(_pObject->*_method)(ptr);
		}
	}
	
	bool equals(const AbstractObserver& abstractObserver) const
	{
		const NObserver* pObs = dynamic_cast<const NObserver*>(&abstractObserver);
		return pObs && pObs->_pObject == _pObject && pObs->_method == _method;
	}

	bool accepts(Notification* pNf) const
	{
		return dynamic_cast<N*>(pNf) != 0;
	}
	
	AbstractObserver* clone() const
	{
		return new NObserver(*this);
	}
	
private:
	NObserver();

	C*       _pObject;
	Callback _method;
};


} // namespace Poco


#endif // Foundation_NObserver_INCLUDED