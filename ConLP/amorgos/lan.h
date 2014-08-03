#ifndef NS_LAN_H
#define NS_LAN_H


#include <naxos.h>
#include "heuristics.h"

int VarOrderHeurFiltered(const naxos::NsIntVarArray& Vars, unsigned *assigns,
                         unsigned asLimit);


namespace naxos {



class goalLan : public NsGoal {
private:
	NsIntVarArray& Vars;
	unsigned assignLimit;

	ValueHeuristic *valHeur;
public:
	goalLan(NsIntVarArray& Vars_init, int aLimit, 
	        ValueHeuristic *valHeuristic = new ValHeurFirst)
		: Vars(Vars_init), assignLimit(aLimit), valHeur(valHeuristic) {}
	NsGoal* GOAL(void);
};


class goalLanDestructor : public NsGoal {
private:
	NsIntVarArray& Vars;
	unsigned *assigns;
public:
	goalLanDestructor(NsIntVarArray& Vars_init, unsigned *asgns)
		: Vars(Vars_init), assigns(asgns) {}

	NsGoal* GOAL (void)
	{
		Vars[0].removeAll();
		return 0;
	}	

	~goalLanDestructor (void)
	{
		delete assigns;
	}
};
		

class goalLanLabeling : public NsGoal {
private:
	NsIntVarArray& Vars;
	unsigned assignLimit;
	unsigned *assigns;

	ValueHeuristic *valHeur;
public:
	goalLanLabeling (NsIntVarArray& Vars_init, unsigned* asgns,
	                 unsigned aLimit, ValueHeuristic *valHeuristic)
		  : Vars(Vars_init), assignLimit(aLimit), valHeur(valHeuristic)
		  { assigns=asgns; }

	NsGoal*  GOAL (void);
};


class goalLanInDomain : public NsGoal {
private:
	NsIntVarArray&  Vars;
	const int  index;
	unsigned assignLimit;
	unsigned *assigns;

	ValueHeuristic *valHeur;
public:
	goalLanInDomain(NsIntVarArray& Vars_init,int index_init,
	                unsigned *asgns, unsigned aLimit,
					ValueHeuristic *valHeuristic)
			: Vars(Vars_init), index(index_init), assignLimit(aLimit),
			  valHeur(valHeuristic) { assigns=asgns; }
	NsGoal*  GOAL (void);
};



} // end namespace


#endif  // NS_LAN_H
