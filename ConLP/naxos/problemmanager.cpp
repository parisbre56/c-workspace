///  \file
///  \brief  Data structures and algorithms for a constraint satisfaction
///           problem management and solving.
///
///  \internal
///  This file is part of
///   <span style="font-variant: small-caps;"> Naxos Solver: </span>
///   A Constraint Programming Library. \n
///   Copyright � 2007-2013  Nikolaos Pothitos.
///
///  See ../license/LICENSE for the license of the library.





#include "naxos.h"


using namespace naxos;
using namespace std;








Ns_StackSearch::goal_iterator::goal_iterator (Ns_StackSearch& stackOfStacks_init)
	: stackOfStacks(&stackOfStacks_init),
	  curr_Stack_it(stackOfStacks->begin())
{
	if ( curr_Stack_it  !=  stackOfStacks->end()
	    && (curr_node_it = curr_Stack_it->stackAND.begin())  ==  curr_Stack_it->stackAND.end() )
	{
		*this  =  curr_Stack_it->delayedGoal;
	}
}





Ns_StackSearch::goal_iterator&
Ns_StackSearch::goal_iterator::operator ++ (void)
{
	assert_Ns( stackOfStacks != 0 ,
		"Ns_StackSearch::goal_iterator::++: Unitialized `*this'");

	assert_Ns( curr_Stack_it != stackOfStacks->end()
			&& curr_node_it != curr_Stack_it->stackAND.end() ,
			"Ns_StackSearch::goal_iterator::end: Bad request `++(something.end())'");

	if ( ++curr_node_it  ==  curr_Stack_it->stackAND.end() )
		*this = curr_Stack_it->delayedGoal;

	return  *this;
}








///  Writes to a file a view of the search tree in a Graphviz supported format.

	void
Ns_StackSearch::searchToGraphFile (const char *fileName)
{
	fileSearchGraph.open(fileName);


	assert_Ns( fileSearchGraph ,
		"Ns_StackSearch::searchToGraphFile: Could not open file");


	fileSearchGraph << "digraph  \"Search Tree\"  {\n\n"
	                << "\tnode [shape=point];\n\n"
		        << "\tedge [arrowhead=none];\n";
}






///  If it does not exist, creates a \c validHistoryId entry for the current search node.

    void
Ns_StackSearch::push (const value_type& newNode)
{
	++nSearchTreeNodes;

	if ( ! empty() )   {

		top().has_a_child  =  true;

		//top().timePrevious  =  clock();
	}

	NsStack<Ns_SearchNode>::push(newNode);

	if ( history_time.size()  <  size() )
		history_time.push_back( history_time_t() );
	//else
	//	history_time[size()-1].validate();
}




///  Records the solution node to the goals graph file (if created).

    void
Ns_StackSearch::solutionNode (const NsIntVar *vObjective)
{
	if ( fileSearchGraph )    {

		fileSearchGraph << "\n\t\"("
			<< size()-1 << ","
			<< history_time[size()-1].validHistoryId
			<< ")"
			<< ( ( top().has_a_child ) ? "LastChild" : "" )
			<< "\" [shape=doublecircle, height=0.1, label=\"\"];\n";

		//  If the node has children, and it is a solution, then it
		//   it is the last child of itself.  (Besides, after the
		//   success it will be popped by the backtracking
		//   algorithm.)


		if ( vObjective  !=  0 )    {

			recordObjective  =  true;

			objectiveValue   =  vObjective->max();
		}
	}
}




///  Invalidates the \c validHistoryId for the current search node.

	void
Ns_StackSearch::pop (void)
{
	//clock_t  timeNow = clock();

	if ( fileSearchGraph  &&  size() - 1 > 0  &&  top().has_a_child )  {

		fileSearchGraph << "\n\t\"("
			<< size()-1 << ","
			<< history_time[size()-1].validHistoryId << ")\"   ->   \"("
			<< size()-1 << ","
			<< history_time[size()-1].validHistoryId << ")LastChild\"";


		if ( recordObjective )    {

			recordObjective  =  false;

			fileSearchGraph << "   [fontsize=9";

			fileSearchGraph << ", headlabel=\""
				<< objectiveValue << "\"";

			fileSearchGraph //<< ", label=\""
				//<< timeNow - top().timePrevious
				<< "\"]";
		}

		fileSearchGraph << ";\n";

		//  If the node has children, then it it is the last child
		//   of itself.  (Besides, the `goalNextChoice'--the second
		//   subgoal of an OR-goal--is executed one level before
		//   the execution of the first subgoal, in the search
		//   tree.)
	}

	if ( fileSearchGraph  &&  size() - 1 > 1 )    {

		fileSearchGraph << "\n\t\"("
			<< size()-2 << ","
			<< history_time[size()-2].validHistoryId << ")\"   ->   \"("
			<< size()-1 << ","
			<< history_time[size()-1].validHistoryId << ")\"";


		if ( recordObjective )    {

			recordObjective  =  false;

			fileSearchGraph << "   [fontsize=9";

			fileSearchGraph << ", headlabel=\""
				<< objectiveValue << "\"";

			fileSearchGraph //<< ", label=\""
				//<< timeNow - top().timePrevious
				<< "\"]";
		}

		fileSearchGraph << ";\n";
	}


	history_time[ size() - 1 ].invalidate();


	NsStack<Ns_SearchNode>::pop();

	//if ( ! empty() )    {

	//	top().timeChildren  =  timeNow - top().timePrevious;

	//	top().timePrevious  =  timeNow;
	//}
}





///  Writes to a file a view of the constraint network in a Graphviz supported format.

	void
NsProblemManager::constraintsToGraphFile (const char *fileName)
{
	fileConstraintsGraph.open(fileName);


	assert_Ns( fileConstraintsGraph ,
	"NsProblemManager::constraintsToGraphFile: Could not open file");


	fileConstraintsGraph
		<< "digraph  \"Constraint Network\"  {\n\n"
		<< "\tnode [shape=plaintext, fontsize=13, height=0.05];\n\n"
		<< "\tedge [arrowsize=0.5, fontsize=10];\n";


	fileConstraintsGraph << "\n\n\n\t/*  Variables  */\n";


	for (NsDeque<const NsIntVar*>::const_iterator
			v = vars.begin();
			v != vars.end();
			++v)
	{
		fileConstraintsGraph << "\n\tVar" << *v
			<< "  [label=\"["
			<< (*v)->min();

		if ( ! (*v)->isBound() )
			fileConstraintsGraph << ".." << (*v)->max();

		fileConstraintsGraph << "]\"];\n";

		//fileConstraintsGraph << "\n\t/* Constraints: "
		//	<< (*v)->constraints.size() << " */\n";
	}


	fileConstraintsGraph << "\n\n\n\t/*  Intermediate Variables (drawn with a smaller font)  */\n";


	for (NsDeque<NsIntVar*>::const_iterator
			v = intermediateVars.begin();
			v != intermediateVars.end();
			++v)
	{
		fileConstraintsGraph << "\n\tVar" << *v
			<< "  [fontsize=9];\n";

		//fileConstraintsGraph << "\n\t/* Constraints: "
		//	<< (*v)->constraints.size() << " */\n";
	}


	fileConstraintsGraph << "\n\n\n\t/*  Constraints  */\n";


	for (Ns_constraints_array_t::iterator  c = constraints.begin();
			c != constraints.end();
			++c)
	{
		(*c)->toGraphFile(fileConstraintsGraph);
	}


	fileConstraintsGraph << "}\n";


	fileConstraintsGraph.close();
}








///  Fetches the next constraint to currentConstr, that concerns the variable varFired.

///  \remarks
///  \li
///  In implementing the queue, to reduce the number of queue operations,
///   one way is simply enqueue the variable whose domain has shrunk,
///   instead of enqueue all relevant arcs. When we dequeue a variable from
///   the queue, we just revise all constraints pointing to this variable.
///   The method is also called variable oriented propagation. (This idea
///   appeared in [McG79] and in [CJ98].)
///  Source: <em>An Optimal Coarse-grained Arc Consistency Algorithm,</em>
///   by C.\ Bessiere et al.
///
///  \li
///  Strictly speaking, in AC-3, arc (\a i, \a j) is not enqueued when arc
///   (\a j, \a i) is made consistent. This optimization could be added in
///   AC-5 by adding \a j as an argument to Enqueue and adding the
///   constraint \a k != \a j to its definition.
///  Source: <em>A Generic Arc-Consistency Algorithm and its
///   Specializations,</em> by P.\ van Hentenryck et al.
///
///
///\note
///
///\code
///NsIntVar  V1(pm,0,0), V2(pm,0,1);
///
///for (int i=0;  i < 640;  ++i)
///	pm.add( V1  <  V2 );
///
///pm.nextSolution();
///\endcode
///
///  The following, commented-out code did not work with the above code
///   (that a Solver's programmer could develop).  We think that the culprit
///   is an STL bug...
///
///\code
///Ns_Constraint  *temp;
///
///for ( ;  currentConstr != varFired->constraints_begin();  ++currentConstr)
///{
///	temp  =  *currentConstr;
///
///	if ( temp  !=  constrFired )    {
///
///		++currentConstr;
///		return  temp;
///	}
///}
///\endcode

	Ns_Constraint*
Ns_QueueItem::getNextConstraint (/*const bool onlyNecessary*/)
{
	////  The k-bounds-consistency level:
	//const NsUInt  k = 25;


	for ( ;  currentConstr < varFired->constraints.size();  ++currentConstr)   {

		switch ( varFired->constraints[currentConstr].constr->revisionType )   {

			case  Ns_Constraint::VALUE_CONSISTENCY :

				do {
					++currentRemovedValue;

				} while ( currentRemovedValue - 1 < removedValues.size()
					&&  removedValues[currentRemovedValue-1].constrFired
						== varFired->constraints[currentConstr].constr );

				if ( currentRemovedValue - 1  ==  removedValues.size() )
					currentRemovedValue  =  0;
				else
					return  varFired->constraints[currentConstr].constr;

				break;


			case  Ns_Constraint::BOUNDS_CONSISTENCY :

				//if ( onlyNecessary  &&  ! varFired->isBound() )    {
				//if ( varFired->size()  >  k )    {

				//	//stale  =  true;

				//	continue;
				//}

				if ( removedBoundRec.removedBound
					&& varFired->constraints[currentConstr].constr  !=
								removedBoundRec.constrFired  )
				{	//  No need to check the constraint that initiated the propagation.
					return  varFired->constraints[currentConstr++].constr;
				}

				break;


			case  Ns_Constraint::BIDIRECTIONAL_CONSISTENCY :

				//if ( onlyNecessary  &&  ! varFired->isBound() )    {
				//if ( varFired->size()  >  k )    {

				//	//stale  =  true;

				//	continue;
				//}

				if ( removedBoundRec.removedBound
					&& removedBoundRec.removalTime  >=
						varFired->constraints[currentConstr].constr->lastConstraintCheckTime )
				{
					return  varFired->constraints[currentConstr++].constr;
				}

				break;


			//case  Ns_Constraint::BIDIRECTIONAL_CONSISTENCY_OUT_OF_QUEUE :

			//	//  Nothing to be done.

			//	break;


			default:

				throw  NsException("Ns_QueueItem::getNextConstraint: Invalid `constr->revisionType'");

				break;
		};
	}


#if 0
	//  First, we check the constraints that impose Arc Consistency.

	if ( currentRemovedValue  <  removedValues.size() )    {

		for ( ; ; )      {

			//  Cyclic increase of the currentConstr.

			if ( currentConstr /*- varFired->constraintsBoundsCons.size()*/  ==  varFired->constraintsArcCons.size() )    {

				currentConstr  =  0;//varFired->constraintsBoundsCons.size();

				if ( ++currentRemovedValue  >=  removedValues.size() )    {

					removedValues.clear();

					currentRemovedValue  =  0;

					break;
				}
			}


			//  No need to check the constraint that initiated the propagation.
			if ( varFired->constraintsArcCons[currentConstr /*- varFired->constraintsBoundsCons.size()*/]
					!=  removedValues[currentRemovedValue].constrFired )
			{
				return  varFired->constraintsArcCons[currentConstr++ /*- varFired->constraintsBoundsCons.size()*/];
			}


			++currentConstr;
		}
	}




	//  Second, we check the constraints that impose Bounds Consistency.

	if ( removedBoundRec.removedBound )    {

		if ( currentConstr < varFired->constraintsBoundsCons.size() )     {

			//  No need to check the constraint that initiated the propagation.
			if ( varFired->constraintsBoundsCons[currentConstr]  ==  removedBoundRec.constrFired )
				++currentConstr;

			if ( currentConstr  <  varFired->constraintsBoundsCons.size() )
				return  varFired->constraintsBoundsCons[currentConstr++];
		}

	}  //else if ( currentConstr < varFired->constraintsBoundsCons.size() )    {
	//
	//	currentConstr  =  varFired->constraintsBoundsCons.size();
	//}
#endif




	////  We have reached the end.

	//if ( stale )    {

	//	currentConstr  =  0;

	//	currentRemovedValue  =  0;

	//	removedValues.clear();
	//}

	return  0;
}




namespace  {

		void
	destroy_goal (NsGoal* g)
	{
		if ( g  !=  0 )    {

			if ( g->isGoalAND()  ||  g->isGoalOR() )   {

				destroy_goal( g->getFirstSubGoal() );

				destroy_goal( g->getSecondSubGoal() );
			}

			delete  g;
		}
	}

}  // end namespace





Ns_StackSearch::Ns_StackSearch (void)
	: nSearchTreeNodes(0), recordObjective(false)
{    }



	void
Ns_StackSearch::clear (void)
{
	while ( ! empty() )   {

		destroy_goal( top().goalNextChoice );

		pop();
	}
}


Ns_StackSearch::~Ns_StackSearch (void)
{
	clear();


	if ( fileSearchGraph )    {

		fileSearchGraph << "}\n";

		fileSearchGraph.close();
	}
}


Ns_StackGoals::~Ns_StackGoals (void)
{
	while ( ! empty() )   {

		destroy_goal( top() );

		pop();
	}
}




NsProblemManager::~NsProblemManager (void)
{
	//  All the queues Q should by destroyed before the intermediateVars
	//   array, because it uses variables of the array.
	while ( ! searchNodes.empty() )
		searchNodes.pop();

	//  Constraints destruction.
	for (Ns_constraints_array_t::iterator  c = constraints.begin();
			c != constraints.end();
			++c)
	{
		delete  *c;
	}


	//  Intermediate variables destruction.
	for (NsDeque<NsIntVar*>::iterator  v = intermediateVars.begin();
			v != intermediateVars.end();
			++v)
	{
		delete  *v;
	}
}





///  Adds a constraint to the problem.

	void
NsProblemManager::add (const Ns_ExprConstr& expr)
{
	assert_Ns( firstNextSolution ,
			"NsProblemManager::add: Cannot add a constraint because search has already started" );

	Ns_Constraint  *newConstr = expr.postConstraint();

	if ( newConstr  ==  0 )
		return;	// unary constraint

	newConstr->ArcCons();

	recordConstraint( newConstr );
}





///  Adds a soft constraint to the problem (with the corresponding \a weight).

	void
NsProblemManager::add (const Ns_ExprConstr& expr, const NsInt weight)
{
	assert_Ns( firstNextSolution ,
		"NsProblemManager::add: Cannot add a constraint because search has already started" );

	assert_Ns( vMinObj == 0 ,
		"NsProblemManager::add: `NsProblemManager::minimize()' should not be used together with soft constraints" );

	vSoftConstraintsTerms.push_back( weight * expr.post() );
}





///  Imposes arc consistency.

	bool
NsProblemManager::arcConsistent (void)
{
	//clock_t  timeStarted = clock();


	if ( foundInconsistency )   {

		foundInconsistency  =  false;
		getQueue().clear();
		++nFailures;
		return  false;
	}

	Ns_Constraint  *c;
	NsIntVar  *vFired;

	//while  ( getQueue().current  !=  getQueue().end() )   {
	while  ( ! getQueue().empty() )   {

		vFired  =  getQueue().front().getVarFired();

		//  To avoid changing the queue item `Q.front()'
		//   during this iteration...
		vFired->queueItem  =  0;

		//bool  onlyNecessary = false;//( getQueue().size() < 1 );//( searchNodes.top().timeChildren == Ns_SearchNode::NEVER
		//|| clock() - searchNodes.top().timePrevious > searchNodes.top().timeChildren );


		while ( (c = getQueue().front().getNextConstraint(/*onlyNecessary*/))  !=  0 )    {

			//  Change the following for AC-3.
			//c->ArcCons();
			c->LocalArcCons( getQueue().front() );

			c->lastConstraintCheckTime  =  ++nConstraintChecks;

			if ( foundInconsistency )   {

				foundInconsistency  =  false;
				getQueue().clear();
				++nFailures;
				return  false;
			}
		}

		getQueue().pop();

		//if ( getQueue().current->stale )
		//	++getQueue().current;
		//else
			//getQueue().current  =  getQueue().erase( getQueue().current );

		//if ( getQueue().current  ==  getQueue().end()
		//	&&  ! onlyNecessary   &&  ! getQueue().empty()  )
		//		getQueue().pushToEnd(getQueue().begin());
	}

	//if ( ! getQueue().empty() )
	//	cout << getCurrentHistoryId().level << "\t" << getQueue().size() << "\n";

	//clock_t  timeAC = clock() - timeStarted;
	//if ( timeAC > 10000 )    {

	//	for (NsIndex i=0;  i < getCurrentHistoryId().level;  ++i)
	//		cout << " ";

	//	cout << searchNodes.mean() << " " << timeAC << "\n";
	//}

	return  true;
}




///  Backtracks the search process to the previous choice point.

	bool
NsProblemManager::backtrack (void)
{
	NsGoal  *goalNextChoice;

	for ( ; ; )    {

		if ( backtrackLim != 0  &&  nBacktracks >= backtrackLim )
			return  false;
		++nBacktracks;

		assert_Ns( ! searchNodes.empty() ,  "NsProblemManager::backtrack: `searchNodes' is empty");

		goalNextChoice  =  searchNodes.top().goalNextChoice;
		if ( goalNextChoice  ==  0 )
			return  false;

		//Ns_StackDomains_t&  domainsOriginal  =  searchNodes.top().domainsStore.domainsOriginal;
		//while ( ! domainsOriginal.empty() )   {
		//	domainsOriginal.top().Var->setDomain( domainsOriginal.top().Dom );
		//	domainsOriginal.pop();
		//}

		searchNodes.top().bitsetsStore.restore();

		searchNodes.pop();
		searchNodes.top().stackAND.push( goalNextChoice );


		if ( vMinObj  !=  0 )   {

			vMinObj->remove(bestMinObjValue, NsPLUS_INF/*, 0*/);

			if ( foundInconsistency )   {

				foundInconsistency  =  false;
				getQueue().clear();
				continue;
			}
		}

		return  true;
	}
}




///  Reverts the domains of the constrained variables (except for the `objective' variable) in the state that they were after the first arcConsistent() has been called.

	void
NsProblemManager::restart (void)
{
	firstNextSolution  =  true;


	// For any case, we clear the propagation engine's members.
	foundInconsistency  =  false;
	getQueue().clear();


	bool  foundSecondFrame  =  false;

	NsGoal  *goalNextChoice;

	assert_Ns( ! searchNodes.empty() ,  "NsProblemManager::restart: `searchNodes' is empty");

	do   {
		goalNextChoice  =  searchNodes.top().goalNextChoice;
		if ( goalNextChoice  ==  0 )
			foundSecondFrame  =  true;

		//Ns_StackDomains_t&  domainsOriginal  =  searchNodes.top().domainsStore.domainsOriginal;
		//while ( ! domainsOriginal.empty() )   {
		//	domainsOriginal.top().Var->setDomain( domainsOriginal.top().Dom );
		//	domainsOriginal.pop();
		//}

		searchNodes.top().bitsetsStore.restore();

		searchNodes.pop();
		searchNodes.top().stackAND.push( goalNextChoice );
		// We keeped the above line because of Memory Management
		//  reasons (in order to delete the `goalNextChoice').

		assert_Ns( ! searchNodes.empty() ,  "`restart()' call, before `nextSolution()'");

	} while ( ! foundSecondFrame );


	searchNodes.pop();
	searchNodes.reset();
	searchNodes.push( Ns_SearchNode( 0, searchNodes.gbegin() ) );


	if ( vMinObj  !=  0 )
		vMinObj->remove( bestMinObjValue, NsPLUS_INF/*, 0*/ );
}





///  Finds next solution of the problem.  Returns \c false when no solution found.

	bool
NsProblemManager::nextSolution (void)
{
	bool  isArcCons = true;

	if ( firstNextSolution )   {

		firstNextSolution  =  false;

		//  Soft constraints objective.
		if ( vMinObj == 0   &&  ! vSoftConstraintsTerms.empty() )
			minimize( - NsSum( vSoftConstraintsTerms ) );

		isArcCons  =  arcConsistent();

		//  Throwing away unnesessary `bitsetsStore' in the first frame.
		searchNodes.top().bitsetsStore.clear();

		//  (A) Cutting from the stackAND of the base frame...
		Ns_StackGoals  tempStackAND;

		while ( ! searchNodes.top().stackAND.empty() )   {

			tempStackAND.push( searchNodes.top().stackAND.top() );

			searchNodes.top().stackAND.pop();
		}

		// A push of frame, for the purposes of NsProblemManager::restart().
		//  We took care placing it *after* the arcConsistent() call (because
		//  in future, we will not be able to revert to the current `Q').
		searchNodes.push( Ns_SearchNode( 0, searchNodes.gbegin() ) );

		//  (B) ...and pasting to the stackAND of the new frame.
		while ( ! tempStackAND.empty() )   {

			searchNodes.top().stackAND.push( tempStackAND.top() );

			tempStackAND.pop();
		}

		if ( searchNodes.top().stackAND.empty()  &&  searchNodes.top().delayedGoal == searchNodes.gend() )
			return  isArcCons;
	}

	if ( calledTimeLimit && timeLim  !=  0 )   {

		calledTimeLimit  =  false;

		if ( isRealTime )   {

			startRealTime  =  time(0);
			assert_Ns(startRealTime != -1,  "Could not find time for `realTimeLimit'");

		}  else  {
			startTime  =  clock();
			assert_Ns(startTime != -1,  "Could not find time for `timeLimit'");
		}
	}


	if ( ( ! isArcCons  ||  ! arcConsistent() )
			|| (searchNodes.top().stackAND.empty()
				&&  searchNodes.top().delayedGoal == searchNodes.gend()) )
	{
		if ( ! backtrack() )
			return  false;
	}


	NsGoal  *CurrGoal, *NewGoal;
	bool  popped_a_goal;

	while ( timeLim == 0   ||  (   isRealTime  &&  difftime(time(0),startRealTime)  <= timeLim)
	                       ||  ( ! isRealTime  &&  static_cast<unsigned long>(clock()-startTime) <= timeLim*static_cast<unsigned long>(CLOCKS_PER_SEC) ) )
	{

		popped_a_goal  =  false;

		if ( ! searchNodes.top().stackAND.empty() )   {

			CurrGoal  =  searchNodes.top().stackAND.top();
			searchNodes.top().stackAND.pop();
			popped_a_goal  =  true;

		}  else  {
			assert_Ns( searchNodes.top().delayedGoal  !=  searchNodes.gend() ,
					"NsProblemManager::nextSolution: No goal to execute");
			CurrGoal  =  *searchNodes.top().delayedGoal;
			++searchNodes.top().delayedGoal;
		}

		assert_Ns( CurrGoal != 0 ,  "NsProblemManager::nextSolution: Zero goal to execute");


		if ( CurrGoal->isGoalAND() )   {

			searchNodes.top().stackAND.push( CurrGoal->getSecondSubGoal() );
			searchNodes.top().stackAND.push( CurrGoal->getFirstSubGoal() );
			if ( popped_a_goal )
				delete  CurrGoal;
			//cout << "--- AND ---\n";


		}  else if ( CurrGoal->isGoalOR() )   {

			searchNodes.push( Ns_SearchNode( CurrGoal->getSecondSubGoal(), searchNodes.gbegin() ) );
			searchNodes.top().stackAND.push( CurrGoal->getFirstSubGoal() );
			if ( popped_a_goal )
				delete  CurrGoal;
			//cout << "--- OR  ---\n";


		}  else  {

			++nGoals;

			NewGoal  =  CurrGoal->GOAL();

			if ( popped_a_goal )
				delete  CurrGoal;

			//for (unsigned i=0;  i<VarGoal->size();  ++i)
			//	cout << " " << (*VarGoal)[i];
			//cout << "\n";

			if ( ! arcConsistent() )   {

				//cout << "<BACKTRACK>\n";
				destroy_goal( NewGoal );

				if ( ! backtrack() )
					return  false;

			} else if ( NewGoal  !=  0 )   {

				searchNodes.top().stackAND.push( NewGoal );

			} else if ( searchNodes.top().stackAND.empty()
					&&  searchNodes.top().delayedGoal == searchNodes.gend() )
			{
				if ( vMinObj  !=  0 )   {
					assert_Ns(bestMinObjValue > vMinObj->max(),
							"NsProblemManager::nextSolution: Wrong objective value");
					bestMinObjValue  =  vMinObj->max();
					//  We have taken care about the rare (and odd) case
					//   where the domain of vMinObj has been augmented.
				}

				searchNodes.solutionNode(vMinObj);

				return  true;
			}
		}
	}

	return  false;
}
