//
// AbstractNode.cpp
//
// $Id$
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/DOM/AbstractNode.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/ChildNodesList.h"
#include "Poco/DOM/EventDispatcher.h"
#include "Poco/DOM/DOMException.h"
#include "Poco/DOM/EventException.h"
#include "Poco/DOM/DOMImplementation.h"
#include "Poco/DOM/Attr.h"
#include "Poco/XML/Name.h"
#include "Poco/DOM/AutoPtr.h"


namespace Poco {
namespace XML {


const XMLString AbstractNode::NODE_NAME = toXMLString("#node");
const XMLString AbstractNode::EMPTY_STRING;


AbstractNode::AbstractNode(Document* pOwnerDocument):
	_pParent(0),
	_pNext(0),
	_pOwner(pOwnerDocument),
	_pEventDispatcher(0)
{
}


AbstractNode::AbstractNode(Document* pOwnerDocument, const AbstractNode& node):
	_pParent(0),
	_pNext(0),
	_pOwner(pOwnerDocument),
	_pEventDispatcher(0)
{
}


AbstractNode::~AbstractNode()
{
	delete _pEventDispatcher;
	if (_pNext) _pNext->release();
}


void AbstractNode::autoRelease()
{
	_pOwner->autoReleasePool().add(this);
}


const XMLString& AbstractNode::nodeName() const
{
	return NODE_NAME;
}


const XMLString& AbstractNode::getNodeValue() const
{
	return EMPTY_STRING;
}


void AbstractNode::setNodeValue(const XMLString& value)
{
	throw DOMException(DOMException::NO_DATA_ALLOWED_ERR);
}


Node* AbstractNode::parentNode() const
{
	return _pParent;
}


NodeList* AbstractNode::childNodes() const
{
	return new ChildNodesList(this);
}


Node* AbstractNode::firstChild() const
{
	return 0;
}


Node* AbstractNode::lastChild() const
{
	return 0;
}


Node* AbstractNode::previousSibling() const
{
	if (_pParent)
	{
		AbstractNode* pSibling = _pParent->_pFirstChild;
		while (pSibling)
		{
		    if (pSibling->_pNext == this) return pSibling;
		    pSibling = pSibling->_pNext;
		}
	}
	return 0;
}


Node* AbstractNode::nextSibling() const
{
	return _pNext;
}


NamedNodeMap* AbstractNode::attributes() const
{
	return 0;
}


Document* AbstractNode::ownerDocument() const
{
	return _pOwner;
}


Node* AbstractNode::insertBefore(Node* newChild, Node* refChild)
{
	throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
}


Node* AbstractNode::replaceChild(Node* newChild, Node* oldChild)
{
	throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
}


Node* AbstractNode::removeChild(Node* oldChild)
{
	throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
}


Node* AbstractNode::appendChild(Node* newChild)
{
	throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
}


bool AbstractNode::hasChildNodes() const
{
	return false;
}


Node* AbstractNode::cloneNode(bool deep) const
{
	return copyNode(deep, _pOwner);
}


void AbstractNode::normalize()
{
}


bool AbstractNode::isSupported(const XMLString& feature, const XMLString& version) const
{
	return DOMImplementation::instance().hasFeature(feature, version);
}


const XMLString& AbstractNode::namespaceURI() const
{
	return EMPTY_STRING;
}


XMLString AbstractNode::prefix() const
{
	return EMPTY_STRING;
}


const XMLString& AbstractNode::localName() const
{
	return EMPTY_STRING;
}


bool AbstractNode::hasAttributes() const
{
	return false;
}


XMLString AbstractNode::innerText() const
{
	return EMPTY_STRING;
}


void AbstractNode::addEventListener(const XMLString& type, EventListener* listener, bool useCapture)
{
	if (_pEventDispatcher)
		_pEventDispatcher->removeEventListener(type, listener, useCapture);
	else
		_pEventDispatcher = new EventDispatcher;
	
	_pEventDispatcher->addEventListener(type, listener, useCapture);
}


void AbstractNode::removeEventListener(const XMLString& type, EventListener* listener, bool useCapture)
{
	if (_pEventDispatcher)
		_pEventDispatcher->removeEventListener(type, listener, useCapture);
}


bool AbstractNode::dispatchEvent(Event* evt)
{
	if (eventsSuspended()) return true;

	if (evt->type().empty()) throw EventException(EventException::UNSPECIFIED_EVENT_TYPE_ERR);

	evt->setTarget(this);
	evt->setCurrentPhase(Event::CAPTURING_PHASE);

	if (_pParent) _pParent->captureEvent(evt);

	if (_pEventDispatcher && !evt->isStopped())
	{
		evt->setCurrentPhase(Event::AT_TARGET);
		evt->setCurrentTarget(this);
		_pEventDispatcher->dispatchEvent(evt);
	}
	if (!evt->isStopped() && evt->bubbles() && _pParent)
	{
		evt->setCurrentPhase(Event::BUBBLING_PHASE);
		_pParent->bubbleEvent(evt);
	}

	return evt->isCanceled();
}


void AbstractNode::captureEvent(Event* evt)
{
	if (_pParent)
		_pParent->captureEvent(evt);
	
	if (_pEventDispatcher && !evt->isStopped())
	{
		evt->setCurrentTarget(this);
		_pEventDispatcher->captureEvent(evt);
	}
}


void AbstractNode::bubbleEvent(Event* evt)
{
	evt->setCurrentTarget(this);
	if (_pEventDispatcher)
	{
		_pEventDispatcher->bubbleEvent(evt);
	}
	if (_pParent && !evt->isStopped())
		_pParent->bubbleEvent(evt);
}


void AbstractNode::dispatchSubtreeModified()
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMSubtreeModified, this, true, false, 0);
	dispatchEvent(pEvent.get());
}


void AbstractNode::dispatchNodeInserted()
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMNodeInserted, this, true, false, parentNode());
	dispatchEvent(pEvent.get());
}


void AbstractNode::dispatchNodeRemoved()
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMNodeRemoved, this, true, false, parentNode());
	dispatchEvent(pEvent.get());
}


void AbstractNode::dispatchNodeRemovedFromDocument()
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMNodeRemovedFromDocument, this, false, false, 0);
	dispatchEvent(pEvent.get());
}


void AbstractNode::dispatchNodeInsertedIntoDocument()
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMNodeInsertedIntoDocument, this, false, false, 0);
	dispatchEvent(pEvent.get());
}


void AbstractNode::dispatchAttrModified(Attr* pAttr, MutationEvent::AttrChangeType changeType, const XMLString& prevValue, const XMLString& newValue)
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMAttrModified, this, true, false, pAttr, prevValue, newValue, pAttr->name(), changeType);
	dispatchEvent(pEvent.get());
}


void AbstractNode::dispatchCharacterDataModified(const XMLString& prevValue, const XMLString& newValue)
{
	AutoPtr<MutationEvent> pEvent = new MutationEvent(_pOwner, MutationEvent::DOMCharacterDataModified, this, true, false, 0, prevValue, newValue, EMPTY_STRING, MutationEvent::MODIFICATION);
	dispatchEvent(pEvent.get());
}


bool AbstractNode::events() const
{
	return _pOwner->events();
}


bool AbstractNode::eventsSuspended() const
{
	return _pOwner->eventsSuspended();
}


void AbstractNode::setOwnerDocument(Document* pOwnerDocument)
{
	_pOwner = pOwnerDocument;
}


} } // namespace Poco::XML
