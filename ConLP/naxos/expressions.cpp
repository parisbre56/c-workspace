///  \file
///  \brief  Processing the expressions between integer constrained
///           variables.
///
///  \internal
///  This file is part of
///   <span style="font-variant: small-caps;"> Naxos Solver: </span>
///   A Constraint Programming Library. \n
///   Copyright � 2007-2013  Nikolaos Pothitos.
///
///  See ../license/LICENSE for the license of the library.





#include "naxos.h"
#include "internal.h"
#include <cstdlib>


using namespace std;
using namespace naxos;


namespace  {
	enum op_type {opPlus=1, opMinus, opTimes, opDiv, opCDivY, opMod, opAnd, opNand, opOr, opNor, opAbs};

		void
	exprYopC_post_constr (NsIntVar &VarX, NsIntVar &VarY, const NsInt C, const op_type opType)
	{
		Ns_Constraint  *newConstr;

		switch ( opType )    {

			case  opPlus:
				newConstr = new Ns_ConstrXeqYplusC(&VarX, &VarY, C);
				break;
			case  opMinus:
				newConstr = new Ns_ConstrXeqCminusY(&VarX, C, &VarY);
				break;
			case  opTimes:
				newConstr = new Ns_ConstrXeqYtimesC(&VarX, &VarY, C);
				break;
			case  opDiv:
				newConstr = new Ns_ConstrXeqYdivC(&VarX, &VarY, C);
				break;
			case  opCDivY:
				newConstr = new Ns_ConstrXeqCdivY(&VarX, C, &VarY);
				break;
			case  opMod:
				newConstr = new Ns_ConstrXeqYmodC(&VarX, &VarY, C);
				break;
			case  opAbs:
				newConstr = new Ns_ConstrXeqAbsY(&VarX, &VarY);
				break;
			default:
				throw  NsException("exprYopC_post_constr: Wrong `opType'");
				break;
		}

		VarX.addConstraint(newConstr);
		VarY.addConstraint(newConstr);
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}
} // namespace



	void
Ns_ExprYplusC::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(), VarY.min() + C, VarY.max() + C);

	exprYopC_post_constr(VarX, VarY, C, opPlus);
}


	NsIntVar&
Ns_ExprYplusC::post (void)  const
{
	if (C == 0)
		return  VarY;

	NsIntVar  *VarX = new NsIntVar(VarY.manager(), VarY.min() + C, VarY.max() + C);

	exprYopC_post_constr(*VarX, VarY, C, opPlus);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprCminusZ::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(), C - VarY.max(), C - VarY.min());

	exprYopC_post_constr(VarX, VarY, C, opMinus);
}


	NsIntVar&
Ns_ExprCminusZ::post (void)  const
{
	NsIntVar  *VarX = new NsIntVar(VarY.manager(), C - VarY.max(), C - VarY.min());

	exprYopC_post_constr(*VarX, VarY, C, opMinus);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





	void
Ns_ExprYtimesC::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(),
			min(VarY.min() * C, VarY.max() * C),  max(VarY.min() * C, VarY.max() * C));

	if (C != 0)	// if C==0 there is no constraint
		exprYopC_post_constr(VarX, VarY, C, opTimes);
}


	NsIntVar&
Ns_ExprYtimesC::post (void)  const
{
	if (C == 1)
		return  VarY;

	NsIntVar  *VarX = new NsIntVar(VarY.manager(),
			min(VarY.min() * C, VarY.max() * C),  max(VarY.min() * C, VarY.max() * C));

	if (C != 0)	// if C==0 there is no constraint
		exprYopC_post_constr(*VarX, VarY, C, opTimes);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





	void
Ns_ExprYdivC::post (NsIntVar &VarX)  const
{
	if ( C  >=  0 )    {
		VarX = NsIntVar(VarY.manager(),
			xDIVy(VarY.min(),C), xDIVy(VarY.max(),C));
	}  else  {
		VarX = NsIntVar(VarY.manager(),
			xDIVy(VarY.max(),C), xDIVy(VarY.min(),C));
	}

	exprYopC_post_constr(VarX, VarY, C, opDiv);
}


	NsIntVar&
Ns_ExprYdivC::post (void)  const
{
	if (C == 1)
		return  VarY;

	NsIntVar  *VarX;
	if ( C  >=  0 )    {
		VarX = new NsIntVar(VarY.manager(),
			xDIVy(VarY.min(),C), xDIVy(VarY.max(),C));
	}  else  {
		VarX = new NsIntVar(VarY.manager(),
			xDIVy(VarY.max(),C), xDIVy(VarY.min(),C));
	}

	exprYopC_post_constr(*VarX, VarY, C, opDiv);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





	void
Ns_ExprCdivZ::post (NsIntVar &VarX)  const
{
	if ( C  >=  0 )    {
		VarX = NsIntVar(VarZ.manager(),
				xDIVy(C,VarZ.max()),  xDIVy(C,VarZ.min()));
	}  else  {
		VarX = NsIntVar(VarZ.manager(),
				xDIVy(C,VarZ.min()),  xDIVy(C,VarZ.max()));
	}

	exprYopC_post_constr(VarX, VarZ, C, opCDivY);
}


	NsIntVar&
Ns_ExprCdivZ::post (void)  const
{
	NsIntVar  *VarX;
	if ( C  >=  0 )    {
		VarX = new NsIntVar(VarZ.manager(),
				xDIVy(C,VarZ.max()),  xDIVy(C,VarZ.min()));
	}  else  {
		VarX = new NsIntVar(VarZ.manager(),
				xDIVy(C,VarZ.min()),  xDIVy(C,VarZ.max()));
	}

	exprYopC_post_constr(*VarX, VarZ, C, opCDivY);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





	void
Ns_ExprYmodC::post (NsIntVar &VarX)  const
{
	NsInt  min, max;
	YmodC_min_max(&VarY, C, min, max);

	VarX = NsIntVar(VarY.manager(), min, max);

	exprYopC_post_constr(VarX, VarY, C, opMod);
}


	NsIntVar&
Ns_ExprYmodC::post (void)  const
{
	if (0 <= VarY.min()  &&  VarY.max() < C)
		return  VarY;

	NsInt  min, max;
	YmodC_min_max(&VarY, C, min, max);

	NsIntVar  *VarX = new NsIntVar(VarY.manager(), min, max);

	exprYopC_post_constr(*VarX, VarY, C, opMod);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprAbsY::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(), max(labs( max(VarY.min(),static_cast<NsInt>(0)) ), labs( min(static_cast<NsInt>(0),VarY.max()))), max(labs(VarY.min()),labs(VarY.max())));

	exprYopC_post_constr(VarX, VarY, 0, opAbs);
}


	NsIntVar&
Ns_ExprAbsY::post (void)  const
{
	NsIntVar *VarX = new NsIntVar(VarY.manager(), max(labs( max(VarY.min(),static_cast<NsInt>(0)) ), labs( min(static_cast<NsInt>(0),VarY.max()))), max(labs(VarY.min()),labs(VarY.max())));

	exprYopC_post_constr(*VarX, VarY, 0, opAbs);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





namespace  {

		void
	array_sort (NsDeque<NsInt>& domain)
	{
		assert_Ns( ! domain.empty() ,
			"NsInDomain: empty domain array");

		sort(domain.begin(), domain.end());
	}


		void
	exprInDomain_post_constr (NsIntVar& VarX,
		const NsDeque<NsInt>& domain,
		NsDeque<NsInt> *domainPrevious,
		NsDeque<NsInt> *domainNext)
	{
		Ns_Constraint  *newConstr =
			new Ns_ConstrXinDomain(&VarX, domain,
					domainPrevious, domainNext);

		VarX.addConstraint(newConstr);
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}

} // namespace


	void
Ns_ExprInDomain::post (NsIntVar &VarX)  const
{
	array_sort(domain);

	VarX = NsIntVar(pm, domain[0], domain[domain.size()-1]);

	exprInDomain_post_constr(VarX, domain, domainPrevious, domainNext);
}


	NsIntVar&
Ns_ExprInDomain::post (void)  const
{
	array_sort(domain);

	NsIntVar  *VarX = new NsIntVar(pm, domain[0],
				domain[domain.size()-1]);

	exprInDomain_post_constr(*VarX, domain, domainPrevious, domainNext);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





namespace  {
		void
	exprYplusCZspecial_post_constr (NsIntVar &VarX, NsIntVar &VarY, const NsInt C, NsIntVar &VarZ)
	{
		Ns_Constraint  *newConstr = new Ns_ConstrXeqYplusCZspecial(&VarX, &VarY, C, &VarZ);
		VarX.addConstraint(newConstr);
		VarY.addConstraint(newConstr);
		VarZ.addConstraint(newConstr);
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}
} // namespace



	void
Ns_ExprYplusCZspecial::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(),  VarY.min() + C * VarZ.min(),  VarY.max() + C * VarZ.max());

	exprYplusCZspecial_post_constr(VarX, VarY, C, VarZ);
}


	NsIntVar&
Ns_ExprYplusCZspecial::post (void)  const
{
	NsIntVar  *VarX = new NsIntVar(VarY.manager(),  VarY.min() + C * VarZ.min(),  VarY.max() + C * VarZ.max());

	exprYplusCZspecial_post_constr(*VarX, VarY, C, VarZ);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





namespace  {
	//enum op_type {opplus=1, opminus, optimes, opdiv, opmod, opand, opNand, opor, opNor};

		void
	exprYopZ_post_constr (NsIntVar &VarX, NsIntVar &VarY, NsIntVar &VarZ, const op_type opType)
	{
		Ns_Constraint  *newConstr;
		switch (opType)  {
			case  opPlus:
				newConstr = new Ns_ConstrXeqYplusZ(&VarX, &VarY, &VarZ);
				break;
			case  opMinus:
				newConstr = new Ns_ConstrXeqYplusZ(&VarY, &VarX, &VarZ);
				break;
			case  opTimes:
				newConstr = new Ns_ConstrXeqYtimesZ(&VarX, &VarY, &VarZ);
				break;
			case  opDiv:
				//VarZ.remove( 0 /*0*/ );
				newConstr = new Ns_ConstrXeqYtimesZ(&VarY, &VarX, &VarZ);
				break;
			//case  opMod:
			//	newConstr = new Ns_ConstrXeqYmodZ(&VarX, &VarY, &VarZ);
			//	break;
			case  opAnd:
				newConstr = new Ns_ConstrXeqYandZ(&VarX, &VarY, &VarZ, true);
				break;
			case  opNand:
				newConstr = new Ns_ConstrXeqYandZ(&VarX, &VarY, &VarZ, false);
				break;
			case  opOr:
				newConstr = new Ns_ConstrXeqYorZ(&VarX, &VarY, &VarZ, true);
				break;
			case  opNor:
				newConstr = new Ns_ConstrXeqYorZ(&VarX, &VarY, &VarZ, false);
				break;
			default:
				throw  NsException("exprYopZ_post_constr: Wrong `opType'");
				break;
		}

		VarX.addConstraint(newConstr);
		VarY.addConstraint(newConstr);
		VarZ.addConstraint(newConstr);
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}
} // namespace



	void
Ns_ExprYplusZ::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(),  VarY.min() + VarZ.min(),  VarY.max() + VarZ.max());

	exprYopZ_post_constr(VarX, VarY, VarZ, opPlus);
}


	NsIntVar&
Ns_ExprYplusZ::post (void)  const
{
	NsIntVar  *VarX = new NsIntVar(VarY.manager(),  VarY.min() + VarZ.min(),  VarY.max() + VarZ.max());

	exprYopZ_post_constr(*VarX, VarY, VarZ, opPlus);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprYminusZ::post (NsIntVar &VarX)  const
{
	VarX = NsIntVar(VarY.manager(),  VarY.min() - VarZ.max(),  VarY.max() - VarZ.min());

	exprYopZ_post_constr(VarX, VarY, VarZ, opMinus);
}


	NsIntVar&
Ns_ExprYminusZ::post (void)  const
{
	NsIntVar  *VarX = new NsIntVar(VarY.manager(),  VarY.min() - VarZ.max(),  VarY.max() - VarZ.min());

	exprYopZ_post_constr(*VarX, VarY, VarZ, opMinus);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprYtimesZ::post (NsIntVar &VarX)  const
{
	NsInt  min, max;
	product_min_max(&VarY, &VarZ, min, max);

	VarX = NsIntVar(VarY.manager(), min, max);

	exprYopZ_post_constr(VarX, VarY, VarZ, opTimes);
}


	NsIntVar&
Ns_ExprYtimesZ::post (void)  const
{
	NsInt  min, max;
	product_min_max(&VarY, &VarZ, min, max);

	NsIntVar  *VarX = new NsIntVar(VarY.manager(), min, max);

	exprYopZ_post_constr(*VarX, VarY, VarZ, opTimes);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



//	void
//Ns_ExprYdivZ::post (NsIntVar &VarX)  const
//{
//	NsInt  min, max;
//	quotient_min_max(&VarY, &VarZ, min, max);
//
//	VarX = NsIntVar(VarY.manager(), min, max);
//
//	exprYopZ_post_constr(VarX, VarY, VarZ, opDiv);
//}
//
//
//	NsIntVar&
//Ns_ExprYdivZ::post (void)  const
//{
//	NsInt  min, max;
//	quotient_min_max(&VarY, &VarZ, min, max);
//
//	NsIntVar  *VarX = new NsIntVar(VarY.manager(), min, max);
//
//	exprYopZ_post_constr(*VarX, VarY, VarZ, opDiv);
//
//	VarX->manager().recordIntermediateVar( VarX );
//	return  *VarX;
//}



	void
Ns_ExprConstrYandZ::postC (NsIntVar &VarX, bool positively)  const
{
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(),  VarY.min() * VarZ.min(),  VarY.max() * VarZ.max());

		exprYopZ_post_constr(VarX, VarY, VarZ, opAnd);
	} else  {
		VarX = NsIntVar(VarY.manager(),  !(VarY.max() * VarZ.max()), !(VarY.min() * VarZ.min()));

		exprYopZ_post_constr(VarX, VarY, VarZ, opNand);
	}
}


	NsIntVar&
Ns_ExprConstrYandZ::postC (bool positively)  const
{
	NsIntVar  *VarX;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(),  VarY.min() * VarZ.min(),  VarY.max() * VarZ.max());

		exprYopZ_post_constr(*VarX, VarY, VarZ, opAnd);
	} else  {
		VarX = new NsIntVar(VarY.manager(),  !(VarY.max() * VarZ.max()), !(VarY.min() * VarZ.min()));

		exprYopZ_post_constr(*VarX, VarY, VarZ, opNand);
	}

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprConstrYorZ::postC (NsIntVar &VarX, bool positively)  const
{
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(),
				(VarY.min() + VarZ.min() != 0),  (VarY.max() + VarZ.max() != 0));

		exprYopZ_post_constr(VarX, VarY, VarZ, opOr);
	} else  {
		VarX = NsIntVar(VarY.manager(),
				!(VarY.max() + VarZ.max() != 0), !(VarY.min() + VarZ.min() != 0));

		exprYopZ_post_constr(VarX, VarY, VarZ, opNor);
	}
}


	NsIntVar&
Ns_ExprConstrYorZ::postC (bool positively)  const
{
	NsIntVar  *VarX;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(),
				(VarY.min() + VarZ.min() != 0),  (VarY.max() + VarZ.max() != 0));

		exprYopZ_post_constr(*VarX, VarY, VarZ, opOr);
	} else  {
		VarX = new NsIntVar(VarY.manager(),
				!(VarY.max() + VarZ.max() != 0), !(VarY.min() + VarZ.min() != 0));

		exprYopZ_post_constr(*VarX, VarY, VarZ, opNor);
	}

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}


	Ns_Constraint*
Ns_ExprConstrYorZ::postConstraint (bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		newConstr = new Ns_ConstrXorY(&VarY, &VarZ, true);
	} else  {
		newConstr = new Ns_ConstrXorY(&VarY, &VarZ, false);
	}
	VarY.addConstraint(newConstr);
	VarZ.addConstraint(newConstr);
	return  newConstr;
}




namespace  {

	enum constr_type {cmin=1, cmax};//, csum};

		void
	exprGlob_post_constr (NsIntVar &VarX, NsIntVarArray &VarArr, const constr_type ctype)
	{
		Ns_Constraint  *newConstr;
		switch (ctype)  {
			case  cmin:
				newConstr = new Ns_ConstrXeqMin(&VarX, &VarArr);
				break;
			case  cmax:
				newConstr = new Ns_ConstrXeqMax(&VarX, &VarArr);
				break;
			//case  csum:
			//	newConstr = new Ns_ConstrXeqSum(&VarX, &VarArr);
			//	break;
			default:
				throw  NsException("exprGlob_post_constr: Wrong `ctype'");
				break;
		}

		VarX.addConstraint(newConstr);
		for (NsIntVarArray::iterator V = VarArr.begin();   V != VarArr.end();   ++V)
			V->addConstraint(newConstr);
		VarArr.addConstraint();
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}


		void
	exprGlobSum_post_constr (NsIntVar &VarX, NsIntVarArray &VarArr, const NsIndex start, const NsIndex length)
	{
		Ns_Constraint  *newConstr = new Ns_ConstrXeqSum(&VarX, &VarArr, start, length);

		VarX.addConstraint(newConstr);
		for (NsIndex  i=start;   i < start+length;   ++i)
			VarArr[i].addConstraint(newConstr);
		VarArr.addConstraint();
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}

} // namespace



	void
Ns_ExprMin::post (NsIntVar &VarX)  const
{
	assert_Ns( ! VarArr.empty() ,
			"Cannot get the min of an empty array" );

	NsInt  min, minmax;
	array_min_minmax(&VarArr, min, minmax);

	VarX = NsIntVar(VarArr[0].manager(), min, minmax);

	exprGlob_post_constr(VarX, VarArr, cmin);
}


	NsIntVar&
Ns_ExprMin::post (void)  const
{
	assert_Ns( ! VarArr.empty() ,
			"Cannot get the min of an empty array" );

	if (VarArr.size() == 1)
		return  VarArr[0];

	NsInt  min, minmax;
	array_min_minmax(&VarArr, min, minmax);

	NsIntVar  *VarX = new NsIntVar(VarArr[0].manager(), min, minmax);

	exprGlob_post_constr(*VarX, VarArr, cmin);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprMax::post (NsIntVar &VarX)  const
{
	assert_Ns( ! VarArr.empty() ,
			"Cannot get the max of an empty array" );

	NsInt  maxmin, max;
	array_maxmin_max(&VarArr, maxmin, max);

	VarX = NsIntVar(VarArr[0].manager(), maxmin, max);

	exprGlob_post_constr(VarX, VarArr, cmax);
}


	NsIntVar&
Ns_ExprMax::post (void)  const
{
	assert_Ns( ! VarArr.empty() ,
			"Cannot get the max of an empty array" );

	if (VarArr.size() == 1)
		return  VarArr[0];

	NsInt  maxmin, max;
	array_maxmin_max(&VarArr, maxmin, max);

	NsIntVar  *VarX = new NsIntVar(VarArr[0].manager(), maxmin, max);

	exprGlob_post_constr(*VarX, VarArr, cmax);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



	void
Ns_ExprSum::post (NsIntVar &VarX)  const
{
	assert_Ns( ! VarArr.empty() ,
			"Cannot get the sum of an empty array" );

	NsInt  summin, summax;
	array_sum_min_max(&VarArr, start, length, summin, summax);

	VarX = NsIntVar(VarArr[0].manager(), summin, summax);

	//if (VarArr.size() != 0)	// if equal to zero, there is no constraint
	exprGlobSum_post_constr(VarX, VarArr, start, length);
}


	NsIntVar&
Ns_ExprSum::post (void)  const
{
	assert_Ns( ! VarArr.empty() ,
			"Cannot get the sum of an empty array" );

	if (VarArr.size() == 1)
		return  VarArr[0];

	NsInt  summin, summax;
	array_sum_min_max(&VarArr, start, length, summin, summax);

	NsIntVar  *VarX = new NsIntVar(VarArr[0].manager(), summin, summax);

	//if (VarArr.size() != 0)	// if equal to zero, there is no constraint
	exprGlobSum_post_constr(*VarX, VarArr, start, length);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}



namespace  {

		void
	array_elements_min_max (const NsDeque<NsInt>& intArray,
			NsIntVar& VarIndex,
			NsInt& minElement, NsInt& maxElement)
	{
		minElement  =  NsPLUS_INF;
		maxElement  =  NsMINUS_INF;

		for (NsIntVar::const_iterator  index = VarIndex.begin();
				index != VarIndex.end();
				++index)
		{
			if ( 0 <= *index
			&& static_cast<NsIndex>(*index) < intArray.size() )
			{

				if ( intArray[*index]  <  minElement )
					minElement  =  intArray[*index];

				if ( intArray[*index]  >  maxElement )
					maxElement  =  intArray[*index];
			}
		}

		assert_Ns( minElement != NsPLUS_INF ,
				"Failed to index the integer array" );
	}


		void
	exprElement_post_constr (NsIntVar& VarIndex,
			const NsDeque<NsInt>& intArray, NsIntVar& VarX)
	{
		Ns_Constraint  *newConstr =
			new Ns_ConstrElement(&VarIndex, intArray, &VarX);

		VarX.addConstraint(newConstr);
		VarIndex.addConstraint(newConstr);
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}

} // namespace


	void
Ns_ExprElement::post (NsIntVar &VarX)  const
{
	NsInt  minElement, maxElement;
	array_elements_min_max(intArray, VarIndex, minElement, maxElement);

	VarX = NsIntVar(VarIndex.manager(), minElement, maxElement);

	exprElement_post_constr(VarIndex, intArray, VarX);
}


	NsIntVar&
Ns_ExprElement::post (void)  const
{
	NsInt  minElement, maxElement;
	array_elements_min_max(intArray, VarIndex, minElement, maxElement);

	NsIntVar  *VarX = new NsIntVar(VarIndex.manager(),
				minElement, maxElement);

	exprElement_post_constr(VarIndex, intArray, *VarX);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}










	void
Ns_ExprInverse::post (NsIntVarArray &VarArrInv)  const
{
	assert_Ns( ! VarArr.empty() , "Cannot inverse an empty array" );

	assert_Ns( VarArrInv.empty() ,  "Ns_ExprInverse::post: `VarArrInv' non-empty");

	NsIntVarArray::iterator  V;
	V = VarArr.begin();
	NsInt  min = V->min();
	NsInt  max = V->max();
	++V;
	for ( ;   V != VarArr.end();   ++V)   {
		if (V->min() < min)
			min = V->min();
		if (V->max() > max)
			max = V->max();
	}

	assert_Ns(min >= 0,  "Ns_ExprInverse::post: `VarArr' min must be >= 0");
	if (MaxDom != -1)  {
		assert_Ns(MaxDom >= max,  "Ns_ExprInverse::post: `MaxDom' is less than `VarArr' max");
		max = MaxDom;
	}

	for (NsIndex  i=0;  i <= static_cast<NsIndex>(max);  ++i)
		VarArrInv.push_back( NsIntVar(VarArr[0].manager(), -1, VarArr.size()-1) );

	Ns_Constraint  *newConstr = new Ns_ConstrInverse(&VarArrInv, &VarArr);

	for (V = VarArr.begin();   V != VarArr.end();   ++V)
		V->addConstraint(newConstr);
	VarArr.addConstraint();
	for (V = VarArrInv.begin();   V != VarArrInv.end();   ++V)
		V->addConstraint(newConstr);
	VarArrInv.addConstraint();
	newConstr->ArcCons();
	VarArr[0].manager().recordConstraint( newConstr );
}













namespace  {
		void
	exprConstrYopZ_post_constr (Ns_Constraint *newConstr, NsIntVar &VarX, NsIntVar &VarY)
	{
		VarX.addConstraint(newConstr);
		VarY.addConstraint(newConstr);
		newConstr->ArcCons();
		VarX.manager().recordConstraint( newConstr );
	}

		void
	exprConstrYopZ_post_constr (Ns_Constraint *newConstr, NsIntVar &VarX, NsIntVar &VarY, NsIntVar &VarZ)
	{
		VarZ.addConstraint(newConstr);
		exprConstrYopZ_post_constr(newConstr, VarX, VarY);
	}
} // namespace







	Ns_Constraint*
Ns_ExprConstrYlessthanC::postConstraint (bool positively)  const
{
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarY.remove( C, NsPLUS_INF/*, 0*/ );
	} else  {
		VarY.remove( NsMINUS_INF, C-1/*, 0*/ );
	}
	return  0;  // unary constraint
}


	void
Ns_ExprConstrYlessthanC::postC (NsIntVar &VarX, bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(), VarY.max() < C, VarY.min() < C);

		newConstr = new Ns_ConstrMetaXeqYlessthanC(&VarX, &VarY, C);
	} else  {
		VarX = NsIntVar(VarY.manager(), VarY.min() >= C, VarY.max() >= C);

		newConstr = new Ns_ConstrMetaXeqYgreatereqthanC(&VarX, &VarY, C);
	}
	exprConstrYopZ_post_constr(newConstr, VarX, VarY);
}


	NsIntVar&
Ns_ExprConstrYlessthanC::postC (bool positively)  const
{
	NsIntVar  *VarX;
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(), VarY.max() < C, VarY.min() < C);

		newConstr = new Ns_ConstrMetaXeqYlessthanC(VarX, &VarY, C);
	} else  {
		VarX = new NsIntVar(VarY.manager(), VarY.min() >= C, VarY.max() >= C);

		newConstr = new Ns_ConstrMetaXeqYgreatereqthanC(VarX, &VarY, C);
	}
	exprConstrYopZ_post_constr(newConstr, *VarX, VarY);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}







	Ns_Constraint*
Ns_ExprConstrYlesseqthanC::postConstraint (bool positively)  const
{
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarY.remove( C+1, NsPLUS_INF/*, 0*/ );
	} else  {
		VarY.remove( NsMINUS_INF, C/*, 0*/ );
	}
	return  0;  // unary constraint
}


	void
Ns_ExprConstrYlesseqthanC::postC (NsIntVar &VarX, bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(), VarY.max() <= C, VarY.min() <= C);

		newConstr = new Ns_ConstrMetaXeqYlesseqthanC(&VarX, &VarY, C);
	} else  {
		VarX = NsIntVar(VarY.manager(), VarY.min() > C, VarY.max() > C);

		newConstr = new Ns_ConstrMetaXeqYgreaterthanC(&VarX, &VarY, C);
	}
	exprConstrYopZ_post_constr(newConstr, VarX, VarY);
}


	NsIntVar&
Ns_ExprConstrYlesseqthanC::postC (bool positively)  const
{
	NsIntVar  *VarX;
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(), VarY.max() <= C, VarY.min() <= C);

		newConstr = new Ns_ConstrMetaXeqYlesseqthanC(VarX, &VarY, C);
	} else  {
		VarX = new NsIntVar(VarY.manager(), VarY.min() > C, VarY.max() > C);

		newConstr = new Ns_ConstrMetaXeqYgreaterthanC(VarX, &VarY, C);
	}
	exprConstrYopZ_post_constr(newConstr, *VarX, VarY);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}






//	Ns_Constraint*
//Ns_ExprConstrYgreaterthanC::postConstraint (void)  const
//{
//	while ( VarY.min() <= C )   {
//		if ( !VarY.removeSingle( VarY.min(), 0 ) )
//			return  0;	// ... to avoid an infinite loop
//	}
//	return  0;  // unary constraint
//}
//
//
//
//	void
//Ns_ExprConstrYgreaterthanC::post (NsIntVar *VarX)  const
//{
//	*VarX = NsIntVar(*VarY.manager(), VarY.min() > C, VarY.max() > C);
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYgreaterthanC(VarX, &VarY, C);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//}
//
//
//	NsIntVar&
//Ns_ExprConstrYgreaterthanC::post (void)  const
//{
//	NsIntVar  *VarX = new NsIntVar(*VarY.manager(), VarY.min() > C, VarY.max() > C);
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYgreaterthanC(VarX, &VarY, C);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//
//	VarX->manager()->recordIntermediateVar( VarX );
//	return  *VarX;
//}






//	Ns_Constraint*
//Ns_ExprConstrYgreatereqthanC::postConstraint (void)  const
//{
//	while ( VarY.min() < C )   {
//		if ( !VarY.removeSingle( VarY.min(), 0 ) )
//			return  0;	// ... to avoid an infinite loop
//	}
//	return  0;  // unary constraint
//}
//
//
//	void
//Ns_ExprConstrYgreatereqthanC::post (NsIntVar *VarX)  const
//{
//	*VarX = NsIntVar(*VarY.manager(), VarY.min() >= C, VarY.max() >= C);
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYgreatereqthanC(VarX, &VarY, C);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//}
//
//
//	NsIntVar&
//Ns_ExprConstrYgreatereqthanC::post (void)  const
//{
//	NsIntVar  *VarX = new NsIntVar(*VarY.manager(), VarY.min() >= C, VarY.max() >= C);
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYgreatereqthanC(VarX, &VarY, C);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//
//	VarX->manager()->recordIntermediateVar( VarX );
//	return  *VarX;
//}






	Ns_Constraint*
Ns_ExprConstrYeqC::postConstraint (bool positively)  const
{
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarY.remove(NsMINUS_INF, C-1/*,        0*/);
		VarY.remove(C+1,         NsPLUS_INF/*, 0*/);
	} else  {
		if (VarY.contains( C ))
			VarY.remove( C );
	}
	return  0;  // unary constraint
}


	void
Ns_ExprConstrYeqC::postC (NsIntVar &VarX, bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(), (VarY.min()==C && VarY.max()==C), VarY.contains(C));

		newConstr = new Ns_ConstrMetaXeqYeqC(&VarX, &VarY, C);
	} else  {
		VarX = NsIntVar(VarY.manager(), !VarY.contains(C), (VarY.min()!=C || VarY.max()!=C));

		newConstr = new Ns_ConstrMetaXeqYneqC(&VarX, &VarY, C);
	}
	exprConstrYopZ_post_constr(newConstr, VarX, VarY);
}


	NsIntVar&
Ns_ExprConstrYeqC::postC (bool positively)  const
{
	NsIntVar  *VarX;
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(), (VarY.min()==C && VarY.max()==C), VarY.contains(C));

		newConstr = new Ns_ConstrMetaXeqYeqC(VarX, &VarY, C);
	} else  {
		VarX = new NsIntVar(VarY.manager(), !VarY.contains(C), (VarY.min()!=C || VarY.max()!=C));

		newConstr = new Ns_ConstrMetaXeqYneqC(VarX, &VarY, C);
	}
	exprConstrYopZ_post_constr(newConstr, *VarX, VarY);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





//	Ns_Constraint*
//Ns_ExprConstrYneqC::postConstraint (void)  const
//{
//	if (VarY.contains( C ))
//		VarY.remove( C );
//	return  0;  // unary constraint
//}
//
//
//	void
//Ns_ExprConstrYneqC::post (NsIntVar *VarX)  const
//{
//	*VarX = NsIntVar(*VarY.manager(), !VarY.contains(C), (VarY.min()!=C || VarY.max()!=C));
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYneqC(VarX, &VarY, C);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//}
//
//
//	NsIntVar&
//Ns_ExprConstrYneqC::post (void)  const
//{
//	NsIntVar  *VarX = new NsIntVar(*VarY.manager(), !VarY.contains(C), (VarY.min()!=C || VarY.max()!=C));
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYneqC(VarX, &VarY, C);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//
//	VarX->manager()->recordIntermediateVar( VarX );
//	return  *VarX;
//}









	Ns_Constraint*
Ns_ExprConstrYlessthanZ::postConstraint (bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		newConstr = new Ns_ConstrXlessthanY(&VarY, &VarZ);
	} else  {
		newConstr = new Ns_ConstrXlesseqthanY(&VarZ, &VarY);
	}
	VarY.addConstraint(newConstr);
	VarZ.addConstraint(newConstr);
	return  newConstr;
}


	void
Ns_ExprConstrYlessthanZ::postC (NsIntVar &VarX, bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(), VarY.max() < VarZ.min(), VarY.min() < VarZ.max());

		newConstr = new Ns_ConstrMetaXeqYlessthanZ(&VarX, &VarY, &VarZ);
	} else  {
		VarX = NsIntVar(VarY.manager(), VarZ.max() <= VarY.min(), VarZ.min() <= VarY.max());

		newConstr = new Ns_ConstrMetaXeqYlesseqthanZ(&VarX, &VarZ, &VarY);
	}
	exprConstrYopZ_post_constr(newConstr, VarX, VarY, VarZ);
}


	NsIntVar&
Ns_ExprConstrYlessthanZ::postC (bool positively)  const
{
	NsIntVar  *VarX;
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(), VarY.max() < VarZ.min(), VarY.min() < VarZ.max());

		newConstr = new Ns_ConstrMetaXeqYlessthanZ(VarX, &VarY, &VarZ);
	} else  {
		VarX = new NsIntVar(VarY.manager(), VarZ.max() <= VarY.min(), VarZ.min() <= VarY.max());

		newConstr = new Ns_ConstrMetaXeqYlesseqthanZ(VarX, &VarZ, &VarY);
	}
	exprConstrYopZ_post_constr(newConstr, *VarX, VarY, VarZ);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}





	Ns_Constraint*
Ns_ExprConstrYlesseqthanZ::postConstraint (bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		newConstr = new Ns_ConstrXlesseqthanY(&VarY, &VarZ);
	} else  {
		newConstr = new Ns_ConstrXlessthanY(&VarZ, &VarY);
	}
	VarY.addConstraint(newConstr);
	VarZ.addConstraint(newConstr);
	return  newConstr;
}


	void
Ns_ExprConstrYlesseqthanZ::postC (NsIntVar &VarX, bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(), VarY.max() <= VarZ.min(), VarY.min() <= VarZ.max());

		newConstr = new Ns_ConstrMetaXeqYlesseqthanZ(&VarX, &VarY, &VarZ);
	} else  {
		VarX = NsIntVar(VarY.manager(), VarZ.max() < VarY.min(), VarZ.min() < VarY.max());

		newConstr = new Ns_ConstrMetaXeqYlessthanZ(&VarX, &VarZ, &VarY);
	}
	exprConstrYopZ_post_constr(newConstr, VarX, VarY, VarZ);
}


	NsIntVar&
Ns_ExprConstrYlesseqthanZ::postC (bool positively)  const
{
	NsIntVar  *VarX;
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(), VarY.max() <= VarZ.min(), VarY.min() <= VarZ.max());

		newConstr = new Ns_ConstrMetaXeqYlesseqthanZ(VarX, &VarY, &VarZ);
	} else  {
		VarX = new NsIntVar(VarY.manager(), VarZ.max() < VarY.min(), VarZ.min() < VarY.max());

		newConstr = new Ns_ConstrMetaXeqYlessthanZ(VarX, &VarZ, &VarY);
	}
	exprConstrYopZ_post_constr(newConstr, *VarX, VarY, VarZ);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}




	Ns_Constraint*
Ns_ExprConstrYeqZ::postConstraint (bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		newConstr = new Ns_ConstrXeqY(&VarY, &VarZ);
	} else  {
		newConstr = new Ns_ConstrXneqY(&VarY, &VarZ);
	}
	VarY.addConstraint(newConstr);
	VarZ.addConstraint(newConstr);
	return  newConstr;
}


	void
Ns_ExprConstrYeqZ::postC (NsIntVar &VarX, bool positively)  const
{
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = NsIntVar(VarY.manager(), VarY.max()==VarZ.min() && VarY.min()==VarZ.max(),
				!intersectionEmpty(&VarY, &VarZ));

		newConstr = new Ns_ConstrMetaXeqYeqZ(&VarX, &VarY, &VarZ, true);
	} else  {
		VarX = NsIntVar(VarY.manager(), intersectionEmpty(&VarY, &VarZ),
				!(VarY.max()==VarZ.min() && VarY.min()==VarZ.max()));

		newConstr = new Ns_ConstrMetaXeqYeqZ(&VarX, &VarY, &VarZ, false);
	}
	exprConstrYopZ_post_constr(newConstr, VarX, VarY, VarZ);
}


	NsIntVar&
Ns_ExprConstrYeqZ::postC (bool positively)  const
{
	NsIntVar  *VarX;
	Ns_Constraint  *newConstr;
	if ( ! isPositive )
		positively  =  ! positively;
	if ( positively )  {
		VarX = new NsIntVar(VarY.manager(), VarY.max()==VarZ.min() && VarY.min()==VarZ.max(),
				!intersectionEmpty(&VarY, &VarZ));

		newConstr = new Ns_ConstrMetaXeqYeqZ(VarX, &VarY, &VarZ, true);
	} else  {
		VarX = new NsIntVar(VarY.manager(), intersectionEmpty(&VarY, &VarZ),
				!(VarY.max()==VarZ.min() && VarY.min()==VarZ.max()));

		newConstr = new Ns_ConstrMetaXeqYeqZ(VarX, &VarY, &VarZ, false);
	}
	exprConstrYopZ_post_constr(newConstr, *VarX, VarY, VarZ);

	VarX->manager().recordIntermediateVar( VarX );
	return  *VarX;
}




//	Ns_Constraint*
//Ns_ExprConstrYneqZ::postConstraint (void)  const
//{
//	Ns_Constraint  *newConstr = new Ns_ConstrXneqY(&VarY, &VarZ);
//	VarY.addConstraint(newConstr);
//	VarZ.addConstraint(newConstr);
//	return  newConstr;
//}
//
//
//	void
//Ns_ExprConstrYneqZ::post (NsIntVar *VarX)  const
//{
//	*VarX = NsIntVar(*VarY.manager(), intersectionEmpty(&VarY, &VarZ),
//			!(VarY.max()==VarZ.min() && VarY.min()==VarZ.max()));
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYneqZ(VarX, &VarY, &VarZ);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY, &VarZ);
//}
//
//
//	NsIntVar&
//Ns_ExprConstrYneqZ::post (void)  const
//{
//	NsIntVar  *VarX = new NsIntVar(*VarY.manager(), intersectionEmpty(&VarY, &VarZ),
//			!(VarY.max()==VarZ.min() && VarY.min()==VarZ.max()));
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYneqZ(VarX, &VarY, &VarZ);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY, &VarZ);
//
//	VarX->manager()->recordIntermediateVar( VarX );
//	return  *VarX;
//}





//	Ns_Constraint*
//Ns_ExprConstrNegationY::postConstraint (void)  const
//{
//	while ( VarY->min() < 0 )   {
//		if ( !VarY->removeSingle( VarY->min(), 0 ) )
//			return;	// ... to avoid an infinite loop
//	}
//	while ( VarY->max() > 0 )   {
//		if ( !VarY->removeSingle( VarY->max(), 0 ) )
//			return;	// ... to avoid an infinite loop
//	}
//	return  0;
//}
//
//
//	void
//Ns_ExprConstrNegationY::post (NsIntVar *VarX)  const
//{
//	*VarX = NsIntVar(*VarY.manager(), VarY.max() < 1, VarY.min() < 1);
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYlessthanC(VarX, &VarY, 1);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//}
//
//
//	NsIntVar&
//Ns_ExprConstrNegationY::post (void)  const
//{
//	NsIntVar  *VarX = new NsIntVar(*VarY.manager(), VarY.max() < 1, VarY.min() < 1);
//
//	Ns_Constraint  *newConstr = new Ns_ConstrMetaXeqYlessthanC(VarX, &VarY, 1);
//	exprConstrYopZ_post_constr(newConstr, VarX, &VarY);
//
//	VarX->manager()->recordIntermediateVar( VarX );
//	return  *VarX;
//}



	Ns_Constraint*
Ns_ExprConstrAllDiff::postConstraint (bool positively)  const
{
	assert_Ns(positively,  "Ns_ExprConstrAllDiff::postConstraint: `positively'==false");
	if (VarArr.size() <= Capacity  ||  ( Capacity==0 && VarArr.size() <= 1 ) )
		return  0;  // no constraint

	Ns_Constraint  *newConstr;
	if ( Capacity  ==  0 )
		newConstr = new Ns_ConstrAllDiff(&VarArr);	// default case
	else
		newConstr = new Ns_ConstrAllDiffStrong(&VarArr, Capacity);

	for (NsIntVarArray::iterator X = VarArr.begin();   X != VarArr.end();   ++X)
		X->addConstraint(newConstr);
	VarArr.addConstraint();
	return  newConstr;
}



	Ns_Constraint*
Ns_ExprConstrCount::postConstraint (bool positively)  const
{
	assert_Ns(positively,  "Ns_ExprConstrCount::postConstraint: `positively'==false");

	Ns_Constraint  *newConstr = new Ns_ConstrCount(&VarArr, Values,
				Occurrences, SplitPositions, Split, Dwin);

	for (NsIntVarArray::iterator X = VarArr.begin();   X != VarArr.end();   ++X)
		X->addConstraint(newConstr);
	VarArr.addConstraint();
	return  newConstr;
}



//	Ns_Constraint*
//Ns_ExprConstrElement::postConstraint (bool positively)  const
//{
//	assert_Ns(positively,  "Ns_ExprConstrElement::postConstraint: `positively'==false");
//
//	assert_Ns( ! intArray.empty() ,
//			"Cannot get an element of an empty array" );
//
//	Ns_Constraint  *newConstr =
//		new Ns_ConstrElement(&VarIndex, intArray, &VarValue);
//
//	VarIndex.addConstraint(newConstr);
//	VarValue.addConstraint(newConstr);
//
//	return  newConstr;
//}
