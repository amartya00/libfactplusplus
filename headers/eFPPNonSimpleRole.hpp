/* This file is part of the FaCT++ DL reasoner
Copyright (C) 2007-2015 by Dmitry Tsarkov

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EFPPNONSIMPLEROLE_H
#define EFPPNONSIMPLEROLE_H

#include "eFaCTPlusPlus.hpp"

/// exception thrown in case non-simple role used where only simple role can be used
class EFPPNonSimpleRole: public EFaCTPlusPlus
{
private:	// members
		/// saved name of the role
	const std::string roleName;
		/// error string
	std::string str;

public:		// interface
		/// c'tor: create an output string
	EFPPNonSimpleRole ( const std::string& name )
		: EFaCTPlusPlus()
		, roleName(name)
	{
		str = "Non-simple role '";
		str += name;
		str += "' is used as a simple one";
		reason = str.c_str();
	}
		/// empty d'tor
	virtual ~EFPPNonSimpleRole ( void ) noexcept {}

		/// access to the role
	const char* getRoleName ( void ) const { return roleName.c_str(); }
}; // EFPPNonSimpleRole

#endif


