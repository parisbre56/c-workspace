#include <naxos.h>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace naxos;

/* Similar to naxos' NsCount, with the difference that occNum can be a range of numbers
 * For all values in <vars>, makes sure each <values>[i] appears <occ>[i] times
 * The number of elements in <values> and <occ> must be equal
 * Returns negative for failure
 */
int cardinality(NsProblemManager& pm,NsIntVarArray& vars,NsDeque<NsInt>& values,NsIntVarArray& occ);

/* This function returns a reference to a tempvar that is equal to the sum of the number of occurences of <val>
 * in the range of <vars> that begins from <start> and ends in <end>. 
 * <end> is not included the range, so the range is more like [<var>,<end>)
 * If <var>==<end> returns an expression equal to 0
 */
NsIntVar& occurenceSumExpression(NsProblemManager& pm,NsIntVarArray& vars, NsIndex start, NsIndex end, NsInt val);

/* Similar to NsSum but for multiplication
 * Returns the result of multiplying all <vars> in the range [<start>,<end>) with each other.
 * If <start>==<end> returns an expression equal to 1
 */
NsIntVar& multiplyAll(NsProblemManager& pm, NsIntVarArray& vars, NsIndex start, NsIndex end);

NsIntVarArray tempVars;

int main (int argc, char *argv[])
{
	try  {

		//  Checking input arguments.

		if ( argc  !=  2 )   {

			cerr << "Correct syntax is:\t" << argv[0] << " <n>\n";
			return  1;
		}

		int  n = atoi(argv[1]);


		/**   ADD YOUR CODE HERE   **/
		
		if(n<1) {
			cerr<<"ERROR: The number of fractions must be greater than 0."<<endl;
			exit(EXIT_FAILURE);
		}

		//Create problem manager
		NsProblemManager  pm;
		
		//Create the appropriate number of vars in the form of [A,B,C,D,E,F,G,H,I,...]
		//and constrain the number of occurences for each number
		NsIntVarArray fracNums;
		NsIntVarArray occNum;
		NsDeque<NsInt> values;
		
		NsIndex varnum=3*n;
		NsInt fl=n/3;//floor
		NsInt cl=(n+3-1)/3;//ceiling
		
		//Create the vars we'll use for the fractions
		for (NsIndex i=0;i<varnum;++i) {
			fracNums.push_back(NsIntVar(pm, 1, 9));
		}
		
		//Create the vars we'll use to constraint the number of occurences
		for (NsIndex i=1;i<10;++i) {
			occNum.push_back(NsIntVar(pm, fl, cl));
			values.push_back(i);
		}
		
		//Constrain the number of occurences
		if(cardinality(pm,fracNums,values,occNum)<0) {
			cerr<<"ERROR: Unable to set cardinality."<<endl;
			exit(EXIT_FAILURE);
		}
		
		//Create a list of all the top of the fractions, in the form of [A,D,G,...]
		NsIntVarArray topList;
		for(NsIndex i=0;i<fracNums.size();i+=3) {
			topList.push_back(fracNums[i]);
		}
		
		//Create a list of all the bottom of the fractions, in the form of [BC,EF,HI,...]
		NsIntVarArray bottomList;
		for(NsIndex i=1;i<fracNums.size();i+=3) {
			bottomList.push_back(10*fracNums[i]+fracNums[i+1]);
		}
		
		//Create the transformed fractions in the form of [A*EF*HI*...,D*BC*HI*...,G*BC*EF*...,...]
		NsIntVarArray transfFracs;
		for(NsIndex i=0;i<topList.size();++i) {
			transfFracs.push_back(topList[i]*multiplyAll(pm,bottomList,0,i)*multiplyAll(pm,bottomList,i+1,topList.size()));
			//Ensure that each transformed fraction is less than the previous one to eliminate symmetrical
			if(i!=0) {
				pm.add(transfFracs[i-1]<transfFracs[i]);
			}
		}
		
		//Create the transformed end result of the fraction sum in the form of (BC*EF*HI*...)
		NsIntVarArray EndSum;
		EndSum.push_back(multiplyAll(pm,bottomList,0,bottomList.size()));
		
		//The sum of the transformed fractions equals the transformed result
		pm.add(EndSum.back()==NsSum(transfFracs));
		
		//Compute solutions
		pm.addGoal( new NsgLabeling(fracNums) );
		pm.addGoal( new NsgLabeling(transfFracs) );
		pm.addGoal( new NsgLabeling(EndSum) );
		pm.addGoal( new NsgLabeling(topList) );
		pm.addGoal( new NsgLabeling(bottomList) );
		pm.addGoal( new NsgLabeling(tempVars) );
		
		while (pm.nextSolution() != false) {
			cout<<topList[0].value()<<'/'<<bottomList[0].value();
			for(NsIndex i=1;i<topList.size();++i) {
				cout<<'+'<<topList[i].value()<<'/'<<bottomList[i].value();
			}
			cout<<"=1"<<endl;
		}

	}  catch (exception& exc)  {

		cerr << exc.what() << "\n";

	}  catch (...)  {

		cerr << "Unknown exception" << "\n";
	}
	cout<<"NO MORE SOLUTIONS\nPress Enter to continue"<<endl;
	cin.ignore();
	
}

NsIntVar& occurenceSumExpression(NsProblemManager& pm,NsIntVarArray& vars, NsIndex start, NsIndex end, NsInt val) {
	if(start==end) {
		tempVars.push_back(NsIntVar (pm,0,0));
		return tempVars.back();
	}
	tempVars.push_back((vars[start]==val)+occurenceSumExpression(pm,vars,start+1,end,val));
	return tempVars.back();
}

int cardinality(NsProblemManager& pm,NsIntVarArray& vars,NsDeque<NsInt>& values,NsIntVarArray& occ) {
	if(values.size()!=occ.size()) {
		cerr<<"ERROR: Cardinality: Size mismatch between <values> and <occ> arrays."<<endl;
		return -1;
	}

	NsDeque<NsInt>::iterator valIt;
	NsIntVarArray::iterator occIt;
	//For all the values and occurences in pairs
	for(valIt=values.begin(),occIt=occ.begin();occIt!=occ.end();++valIt,++occIt) {
		//The sum of the number of all the <vars> that are equal to <valIt> is in the range <occIt>
		pm.add(*occIt==occurenceSumExpression(pm,vars,0,vars.size(),*valIt));
	}
	
	//Success
	return 0;
}

NsIntVar& multiplyAll(NsProblemManager& pm, NsIntVarArray& vars, NsIndex start, NsIndex end) {
	if(start==end) {
		tempVars.push_back(NsIntVar (pm,1,1));
		return tempVars.back();
	}
	tempVars.push_back(multiplyAll(pm,vars,start+1,end)*vars[start]);
	return tempVars.back();
}