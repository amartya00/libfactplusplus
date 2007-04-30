/* This file is part of the FaCT++ DL reasoner
Copyright (C) 2003-2007 by Dmitry Tsarkov

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef _DATATYPECENTER_H
#define _DATATYPECENTER_H

#include "tDataType.h"
#include "dltree.h"

class DataTypeReasoner;

class DataTypeCenter
{
public:	// interface
		/// vector of DATATYPEs
	typedef std::vector<TDataType*> TypesVector;

		/// iterator (RW) to access types
	typedef TypesVector::iterator iterator;
		/// iterator (RO) to access types
	typedef TypesVector::const_iterator const_iterator;

protected:	// members
		/// vector of registered data types; initially contains unrestricted NUMBER and STRING
	TypesVector Types;

protected:	// methods
		/// register data type with a given name
	void RegisterDataType ( const std::string& name )
	{
		Types.push_back(new TDataType());
		Types.back()->setType(name);
	}

	// iterators

		/// begin (RW)
	iterator begin ( void ) { return Types.begin(); }
		/// end (RW)
	iterator end ( void ) { return Types.end(); }
		/// begin (RO)
	const_iterator begin ( void ) const { return Types.begin(); }
		/// end (RO)
	const_iterator end ( void ) const { return Types.end(); }

	// access to datatypes

		/// get type corresponding to Numbers
	TDataType* getNumberDataType ( void ) const { return *begin(); }
		/// get type corresponding to Strings
	TDataType* getStringDataType ( void ) const { return *(begin()+1); }
		/// get type by name
	TDataType* getTypeByName ( const std::string& name ) const;

		/// get type corresponding to given sample
	TDataType* getTypeBySample ( const TDataEntry* sample ) const
	{
		const TDataEntry* type = sample->isBasicDataType() ? sample : sample->getType();

		for ( const_iterator p = begin(), p_end = end(); p < p_end; ++p )
			if ( type == (*p)->getType() )
				return *p;

		return NULL;
	}

	// DLTree wrapping interface

		/// get DLTree by a given TDE
	static DLTree* wrap ( const TDataEntry* t ) { return new DLTree(TLexeme(DATAEXPR,const_cast<TDataEntry*>(t))); }
		/// get TDE by a given DLTree
	static TDataEntry* unwrap ( const DLTree* t ) { return static_cast<TDataEntry*>(t->Element().getName()); }

		/// return registered facet of the type, defined by SAMPLE;
	TDataInterval* getFacet ( const DLTree* sample ) const
		{ return getTypeBySample(unwrap(sample))->getFacet(); }

public:		// interface
		// c'tor: create NUMBER and STRING datatypes
	DataTypeCenter ( void )
	{
		// register primitive DataTypes
		RegisterDataType ( "Number" );
		RegisterDataType ( "String" );
	}
		/// d'tor: delete all datatypes
	~DataTypeCenter ( void );

	// DLTree interface

		/// get NUMBER DT that can be used in TBox
	DLTree* getNumberType ( void ) { return wrap(getNumberDataType()->getType()); }
		/// get STRING DT that can be used in TBox
	DLTree* getStringType ( void ) { return wrap(getStringDataType()->getType()); }

		/// return registered data value by given NAME of a Type, given by SAMPLE
	DLTree* getDataValue ( const std::string& name, const DLTree* sample ) const throw(CantRegName)
	{
		TDataType* type = getTypeBySample(unwrap(sample));
		TDataEntry* ret = NULL;
		try
		{
			ret = type->get(name);
		} catch ( CantRegName e )
		{
			e.setType(type->getType()->getName());
			throw e;
		}
		return wrap(ret);
	}
		/// return registered data expression of the type, defined by SAMPLE;
	DLTree* getDataExpr ( const DLTree* sample ) const
		{ return wrap(getTypeBySample(unwrap(sample))->getExpr()); }
		/// define named datatype as equal to given EXPR. FIXME!! stub for JNI for now
	DLTree* getDataType ( const std::string& name ATTR_UNUSED, DLTree* expr )
		{ return expr; }

		/// apply FACET to a data expression EXPR
	DLTree* applyFacet ( DLTree* expr, const TDataInterval* facet ) const
	{
		unwrap(expr)->getFacet()->update(*facet);
		return expr;
	}
		/// apply facet, obtained from TYPE to a data expression EXPR
	DLTree* applyFacet ( DLTree* expr, const DLTree* type ) const
		{ return applyFacet ( expr, unwrap(type)->getFacet() ); }

	//------------------------------------------------------------
	//	Facets interface. Every facet is actually an appropriate Type's expression
	//	with given restriction
	//------------------------------------------------------------
		/// facet for >=
	TDataInterval* getMinInclusiveFacet ( DLTree* value ) const
	{
		TDataInterval* ret = getFacet(value);
		ret->updateMin ( /*excl=*/false, unwrap(value) );
		return ret;
	}
		/// facet for >
	TDataInterval* getMinExclusiveFacet ( DLTree* value ) const
	{
		TDataInterval* ret = getFacet(value);
		ret->updateMin ( /*excl=*/true, unwrap(value) );
		return ret;
	}
		/// facet for <=
	TDataInterval* getMaxInclusiveFacet ( DLTree* value ) const
	{
		TDataInterval* ret = getFacet(value);
		ret->updateMax ( /*excl=*/false, unwrap(value) );
		return ret;
	}
		/// facet for <
	TDataInterval* getMaxExclusiveFacet ( DLTree* value ) const
	{
		TDataInterval* ret = getFacet(value);
		ret->updateMax ( /*excl=*/true, unwrap(value) );
		return ret;
	}

	// reasoning interface

		/// init DT reasoner
	void initDataTypeReasoner ( DataTypeReasoner& DTReasoner ) const;
		/// lock/unlock types for additional elements
	void setLocked ( bool val );
}; // DataTypeCenter

#endif
