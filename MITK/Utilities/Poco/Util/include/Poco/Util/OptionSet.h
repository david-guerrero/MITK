//
// OptionSet.h
//
// $Id$
//
// Library: Util
// Package: Options
// Module:  OptionSet
//
// Definition of the OptionSet class.
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


#ifndef Util_OptionSet_INCLUDED
#define Util_OptionSet_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/Option.h"
#include <vector>


namespace Poco {
namespace Util {


class Util_API OptionSet
	/// A collection of Option objects.
{
public:
	typedef std::vector<Option> OptionVec;
	typedef OptionVec::const_iterator Iterator;

	OptionSet();
		/// Creates the OptionSet.

	OptionSet(const OptionSet& options);
		/// Creates an option set from another one.

	~OptionSet();
		/// Destroys the OptionSet.

	OptionSet& operator = (const OptionSet& options);
		/// Assignment operator.

	void addOption(const Option& option);
		/// Adds an option to the collection.
	
	bool hasOption(const std::string& name, bool matchShort = false) const;
		/// Returns a true iff an option with the given name exists.
		///
		/// The given name can either be a fully specified short name,
		/// or a partially specified full name. If a partial name
		/// matches more than one full name, false is returned.
		/// The name must either match the short or full name of an
		/// option. Comparison case sensitive for the short name and
		/// not case sensitive for the full name.
		
	const Option& getOption(const std::string& name, bool matchShort = false) const;
		/// Returns a reference to the option with the given name.
		///
		/// The given name can either be a fully specified short name,
		/// or a partially specified full name.
		/// The name must either match the short or full name of an
		/// option. Comparison case sensitive for the short name and
		/// not case sensitive for the full name.
		/// Throws a NotFoundException if no matching option has been found.
		/// Throws an UnknownOptionException if a partial full name matches
		/// more than one option.

	Iterator begin() const;
		/// Supports iterating over all options.
		
	Iterator end() const;
		/// Supports iterating over all options.
	
private:	
	OptionVec _options;
};


} } // namespace Poco::Util


#endif // Util_OptionSet_INCLUDED