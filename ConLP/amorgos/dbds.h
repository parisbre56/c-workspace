#ifndef NS_DBDS_H
#define NS_DBDS_H

#include <cassert>

#include <naxos.h>

#include "heuristics.h"
#include "dfs.h"
#include "onesamp.h"


namespace  naxos  {

class goalDbds : public NsGoal {
private:
	NsIntVarArray&  Vars;

	ValueHeuristic *valHeur;
public:
	goalDbds(NsIntVarArray& Vars_init,
	         ValueHeuristic *valHeuristic = new ValHeurFirst)
			: Vars(Vars_init), valHeur(valHeuristic) {}
	NsGoal*  GOAL (void);
};

class goalDbdsStepping : public NsGoal  {
private:
	NsIntVarArray&  Vars;
	unsigned int depthLimit;

	ValueHeuristic *valHeur;
public:
	goalDbdsStepping(NsIntVarArray& Vars_init, unsigned limit,
					ValueHeuristic *valHeuristic)
			: Vars(Vars_init), depthLimit(limit), valHeur(valHeuristic) {}
	NsGoal*  GOAL (void);
};


class goalDbdsLabeling : public NsGoal  {
private:
	NsIntVarArray&  Vars;
	int currDepth;
	int depthLimit;

	ValueHeuristic *valHeur;
public:
	goalDbdsLabeling(NsIntVarArray& Vars_init, int curr, int limit,
					ValueHeuristic *valHeuristic)
			: Vars(Vars_init), currDepth(curr), depthLimit(limit),
			  valHeur(valHeuristic) {}
	NsGoal*  GOAL (void);
};


} // end namespace



#endif  // NS_DBDS_H
