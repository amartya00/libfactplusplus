/* This file is part of the FaCT++ DL reasoner
Copyright (C) 2009-2016 by Dmitry Tsarkov

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

#ifndef TONTOLOGYPRINTERLISP_H
#define TONTOLOGYPRINTERLISP_H

#include "tExpressionPrinterLISP.h"
#include "tDLAxiom.h"
#include "tOntology.h"

class TLISPOntologyPrinter: public DLAxiomVisitor
{
protected:	// members
		/// main stream
	std::ostream& o;
		/// printer for the expressions
	TLISPExpressionPrinter LEP;

protected:	// methods
		/// helper to print several expressions in a row
	template<class Container>
	void print ( const Container& c )
	{
		for ( const auto& expr: c )
			expr->accept(LEP);
	}
		/// helper to print a string
	TLISPOntologyPrinter& operator << ( const char* str ) { o << str; o.flush(); return *this; }
		/// helper to print an expression
	TLISPOntologyPrinter& operator << ( const TDLExpression* expr ) { expr->accept(LEP); return *this; }

public:		// visitor interface
	virtual void visit ( const TDLAxiomDeclaration& axiom )
	{
		const TDLExpression* decl = axiom.getDeclaration();
		bool cname = dynamic_cast<const TDLConceptName*>(decl) != nullptr;
		bool iname = dynamic_cast<const TDLIndividualName*>(decl) != nullptr;
		bool rname = dynamic_cast<const TDLObjectRoleName*>(decl) != nullptr;
		bool dname = dynamic_cast<const TDLDataRoleName*>(decl) != nullptr;

		// do not print TOP/BOT/datatypes
		if ( !cname && !iname && !rname && !dname )
			return;

		*this << "(def" <<
			(cname ? "primconcept" :
			 iname ? "individual" :
			 rname ? "primrole" :
				    "datarole")
			  << decl << ")\n";
	}

	virtual void visit ( const TDLAxiomEquivalentConcepts& axiom ) { o << "(equal_c"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomDisjointConcepts& axiom ) { o << "(disjoint_c"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomDisjointUnion& axiom )
		{ o << "(disjoint_c"; print(axiom); o << ")\n(equal_c" << axiom.getC() << " (or"; print(axiom); o << "))\n"; }
	virtual void visit ( const TDLAxiomEquivalentORoles& axiom ) { o << "(equal_r"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomEquivalentDRoles& axiom ) { o << "(equal_r"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomDisjointORoles& axiom ) { o << "(disjoint_r"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomDisjointDRoles& axiom ) { o << "(disjoint_r"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomSameIndividuals& axiom ) { o << "(same"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomDifferentIndividuals& axiom ) { o << "(different"; print(axiom); o << ")\n"; }
	virtual void visit ( const TDLAxiomFairnessConstraint& axiom ) { o << "(fairness"; print(axiom); o << ")\n"; }

	virtual void visit ( const TDLAxiomRoleInverse& axiom ) { *this << "(equal_r" << axiom.getRole() << " (inv" << axiom.getInvRole() << "))\n"; }
	virtual void visit ( const TDLAxiomORoleSubsumption& axiom ) { *this << "(implies_r" << axiom.getSubRole() << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomDRoleSubsumption& axiom ) { *this << "(implies_r" << axiom.getSubRole() << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomORoleDomain& axiom ) { *this << "(domain" << axiom.getRole() << axiom.getDomain() << ")\n"; }
	virtual void visit ( const TDLAxiomDRoleDomain& axiom ) { *this << "(domain" << axiom.getRole() << axiom.getDomain() << ")\n"; }
	virtual void visit ( const TDLAxiomORoleRange& axiom ) { *this << "(range" << axiom.getRole() << axiom.getRange() << ")\n"; }
	virtual void visit ( const TDLAxiomDRoleRange& axiom ) { *this << "(range" << axiom.getRole() << axiom.getRange() << ")\n"; }
	virtual void visit ( const TDLAxiomRoleTransitive& axiom ) { *this << "(transitive" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomRoleReflexive& axiom ) { *this << "(reflexive" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomRoleIrreflexive& axiom ) { *this << "(irreflexive" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomRoleSymmetric& axiom ) { *this << "(symmetric" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomRoleAsymmetric& axiom ) { *this << "(asymmetric" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomORoleFunctional& axiom ) { *this << "(functional" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomDRoleFunctional& axiom ) { *this << "(functional" << axiom.getRole() << ")\n"; }
	virtual void visit ( const TDLAxiomRoleInverseFunctional& axiom ) { *this << "(functional (inv" << axiom.getRole() << "))\n"; }

	virtual void visit ( const TDLAxiomConceptInclusion& axiom ) { *this << "(implies_c" << axiom.getSubC() << axiom.getSupC() << ")\n"; }
	virtual void visit ( const TDLAxiomInstanceOf& axiom ) { *this << "(instance" << axiom.getIndividual()  << axiom.getC() << ")\n"; }
	virtual void visit ( const TDLAxiomRelatedTo& axiom )
		{ *this << "(related" << axiom.getIndividual() << axiom.getRelation() << axiom.getRelatedIndividual() << ")\n"; }
	virtual void visit ( const TDLAxiomRelatedToNot& axiom )
		{ *this<< "(instance" << axiom.getIndividual() << " (all" << axiom.getRelation() << "(not" << axiom.getRelatedIndividual() << ")))\n"; }
	virtual void visit ( const TDLAxiomValueOf& axiom )
		{ *this << "(instance" << axiom.getIndividual() << " (some" << axiom.getAttribute() << axiom.getValue() << "))\n"; }
	virtual void visit ( const TDLAxiomValueOfNot& axiom )
		{ *this << "(instance" << axiom.getIndividual() << " (all" << axiom.getAttribute() << "(not " << axiom.getValue() << ")))\n"; }

public:		// interface
		/// init c'tor
	TLISPOntologyPrinter ( std::ostream& o_ ) : o(o_), LEP(o_) {}
	virtual ~TLISPOntologyPrinter ( void ) {}
	void recordDataRole ( const char* name ) { o << "(defdatarole " << name << ")\n"; }
}; // TLISPOntologyPrinter

#endif
