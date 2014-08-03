#include <naxos.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "strtok.h"

using namespace std;
using namespace naxos;

//Aids in the sorting via the std::sort method.
//If toCompare->at(a) < toCompare->at(b), returns true.
bool dequeComparison(NsIndex a, NsIndex b);

struct  task_t  {
	NsInt  machine_type;
	NsInt  duration;
	NsInt  staff;
	string name;
};

NsIntVarArray tempVars;
NsDeque<NsInt>* toCompare;

int main (int argc, char *argv[])
{
	try  {

		//  Checking input arguments.

		if ( argc  !=  2 )   {

			cerr << "Correct syntax is:\t" << argv[0]
				<< " <input_filename>\n";
			return  1;
		}


		//  Opening input filename, e.g. `jobshop_opt_data.pl'.

		ifstream  file(argv[1]);

		if ( ! file )   {

			cerr << "Could not open `" << argv[1] << "'\n";
			return  1;
		}


		NsDeque< NsDeque<task_t> >  TasksOfJob;
		NsDeque<NsInt>              MachinesPerType;
		NsInt                       Staff=0;

		unsigned  job=0;
		unsigned task=0;
		string    input, tok;

		//  Reading input file line by line.

		while ( getline(file,input) )    {

			StrTokenizer  tokens(input, "([, ]).");

			tok  =  tokens.next();

			if ( tok  ==  "job" )    {

				TasksOfJob.push_back( NsDeque<task_t>() );

				tokens.next();	// Ignore a token.
				while ( ! (tok = tokens.next()).empty() )
					TasksOfJob.back().push_back( task_t() );

			}  else if ( tok  ==  "task" )    {

				tokens.next();	// Ignore a token.
				TasksOfJob[job][task].machine_type =
					atoi(tokens.next().substr(1).c_str()) - 1;
				TasksOfJob[job][task].duration =
					atoi(tokens.next().c_str());
				TasksOfJob[job][task].staff =
					atoi(tokens.next().c_str());
				TasksOfJob[job][task].name= "t"+to_string(job+1)+to_string(task+1);

				++task;
				if ( task  ==  TasksOfJob[job].size() )   {
					task = 0;
					++job;
				}

			}  else if ( tok  ==  "machine" )    {

				tokens.next();	// Ignore a token.
				MachinesPerType.push_back( atoi(tokens.next().c_str()) );

			}  else if ( tok  ==  "staff" )    {

				Staff  =  atoi(tokens.next().c_str());

			}  else if ( tok  !=  "" )    {

				cerr << "Unknown predicate `" << tok << "'\n";
				return  1;
			}
		}

		file.close();


		//  Print problem representation.
		for (job=0;  job < TasksOfJob.size();  ++job)   {
			for (task=0;  task < TasksOfJob[job].size();  ++task)   {
				cout << "TasksOfJob[" << job << "][" << task
					<< "].machine_type = "
					<< TasksOfJob[job][task].machine_type << "\n";
				cout << "TasksOfJob[" << job << "][" << task
					<< "].duration = "
					<< TasksOfJob[job][task].duration << "\n";
				cout << "TasksOfJob[" << job << "][" << task
					<< "].staff = "
					<< TasksOfJob[job][task].staff << "\n";
			}
			cout << "\n";
		}
		for (unsigned mpt=0;  mpt < MachinesPerType.size();  ++mpt)   {
			cout << "MachinesPerType[" << mpt << "] = "
				<< MachinesPerType[mpt] << "\n";
		}
		cout << "\nStaff = " << Staff << "\n";



		/**   ADD YOUR CODE HERE   **/
		NsProblemManager pm;
		
		NsDeque<NsInt> machines;
		NsDeque<task_t> tasks;
		
		NsIntVarArray taskStartTime;
		NsIntVarArray taskEndTime;
		NsIntVarArray taskMachineAssigment;
		
		NsIntVarArray endTimeList;
		NsIntVarArray startTimeList;
		
		for(NsIndex i=0;i<MachinesPerType.size();++i) {
			for(NsIndex j=MachinesPerType[i];j>0;--j) {
				machines.push_back(i);
			}
		}
		
		NsInt maxEndTime=0;
		
		for(NsIndex i=0;i<TasksOfJob.size();++i) {
			for(NsIndex j=0;j<TasksOfJob[i].size();++j) {
				maxEndTime+=TasksOfJob[i][j].duration;
			}
		}
		
		for(NsIndex i=0;i<TasksOfJob.size();++i) {
			
			tasks.push_back(TasksOfJob[i][0]);
			taskStartTime.push_back(NsIntVar(pm,0,maxEndTime));
			taskEndTime.push_back(NsIntVar(pm,0,maxEndTime));
			//Constraint start_time + duration = end_time
			pm.add(taskStartTime.back()+TasksOfJob[i][0].duration==taskEndTime.back());
			taskMachineAssigment.push_back(NsIntVar(pm,0,machines.size()));
			//Constraint machine assigment only to valid machines
			pm.add(machines[taskMachineAssigment.back()]==TasksOfJob[i][0].machine_type);
			
			//Put the start time of the first task of this job in a list of start times so that 
			//we can compute the minimum end time and set it to be equal to 0
			startTimeList.push_back(taskStartTime.back());
			
			for(NsIndex j=1;j<TasksOfJob[i].size();++j) {
				
				tasks.push_back(TasksOfJob[i][j]);
				taskStartTime.push_back(NsIntVar(pm,0,maxEndTime));
				taskEndTime.push_back(NsIntVar(pm,0,maxEndTime));
				//Constraint start_time + duration = end_time
				pm.add(taskStartTime.back()+TasksOfJob[i][j].duration==taskEndTime.back());
				taskMachineAssigment.push_back(NsIntVar(pm,0,machines.size()));
				//Constraint machine assigment only to valid machines
				pm.add(machines[taskMachineAssigment.back()]==TasksOfJob[i][j].machine_type);
				//Constrain this task must start after the previous one for the same job has finished
				pm.add(taskStartTime.back() >= taskEndTime[taskEndTime.size()-2]);
			}
			
			//Put the end time of the final task of this job in a list of end times we will 
			//use to compute the maximum end time so that we can minimize it
			//Since the tasks of each job are completed sequentially, we are certain that
			//one of the final tasks will have the maximum end time of all tasks, which is what we seek to minimize
			endTimeList.push_back(taskEndTime.back());
		}
		
		//Constraint only one task can use one machine at the same time
		//For all tasks
		for(NsIndex i=0;i<tasks.size();++i) {
			//For all tasks except the i-th
			for(NsIndex j=0;j<tasks.size();++j) {
				if(j==i) {
					continue;
				}
							//If task i uses the same machine as j, 
				pm.add( (taskMachineAssigment[i]==taskMachineAssigment[j] && 
									//then i must start after j ends or j must start after i ends
									(taskStartTime[i]>=taskEndTime[j] || taskStartTime[j]>=taskEndTime[i]) )
							//Else they must not use the same machine
							|| (taskMachineAssigment[i]!=taskMachineAssigment[j]) );
				
			}
		}
		
		//Constraint min start time == 0 because we are certain that a task can start at time 0
		pm.add(NsMin(startTimeList)==0);
		
		//Add the minimization of max start time as one of our goals
		tempVars.push_back(NsMax(endTimeList));
		NsIndex ETLMaxInd=tempVars.size()-1;
		pm.minimize(tempVars.back());
		
		//Staff constraint
		NsIntVarArray staffUseList;
		//For all time units
		for(NsInt time=0;time<=tempVars[ETLMaxInd].max();++time) {
			//Compute the staff used in this time unit
			//For task 0
			//If this task happens within the time unit, add its staff to the sum for this time unit
			tempVars.push_back( (taskStartTime[0]>=time && taskEndTime[0]<time) * tasks[0].staff );
			//For all other tasks
			for(NsIndex i=1;i<tasks.size();++i) {
				//If this task happens within the time unit, add its staff to the sum for this time unit
				tempVars.push_back( tempVars.back() + ( (taskStartTime[0]>=time && taskEndTime[0]<time) * tasks[0].staff ) );
			}
			//Add the sum of active staff for this time unit 
			//to a list of staff for all time units so that we can compute the maximum
			staffUseList.push_back(tempVars.back());
		}
		
		//The maximum number of staff used throughout the execution of all tasks
		//must be less than or equal to the number of availiable staff
		pm.add(NsMax(staffUseList)<=Staff);
		
		//Start computing solutions
		pm.addGoal(new NsgLabeling(taskStartTime));
		pm.addGoal(new NsgLabeling(taskEndTime));
		pm.addGoal(new NsgLabeling(taskMachineAssigment));
		pm.addGoal(new NsgLabeling(startTimeList));
		pm.addGoal(new NsgLabeling(endTimeList));
		pm.addGoal(new NsgLabeling(staffUseList));
		pm.addGoal(new NsgLabeling(tempVars));
		bool solutionFound=false;
		NsDeque<NsInt> startTimes;
		NsDeque<NsInt> endTimes;
		NsDeque<NsInt> machineAssigments;
		startTimes.resize(tasks.size());
		endTimes.resize(tasks.size());
		machineAssigments.resize(tasks.size());
		while(pm.nextSolution() != false) {
			//Store the solution
			solutionFound=true;
			for(NsIndex i = 0; i<tasks.size() ; ++i) {
				startTimes[i]=taskStartTime[i].value();
				endTimes[i]=taskEndTime[i].value();
				machineAssigments[i]=taskMachineAssigment[i].value();
			}
		}
		if(!solutionFound) {
			cout<<"NO SOLUTION FOUND"<<endl;
		}
		else {
			//Display the solution in the requested manner
			
			NsDeque< NsDeque <NsIndex> > indexesPerMachine;
			indexesPerMachine.resize(machines.size());
			//For all tasks put their indexes in a list according to their machine
			for(NsIndex i = 0; i<tasks.size() ; ++i) {
				indexesPerMachine[machineAssigments[i]].push_back(i);
			}
			//For all machines, sort their indexes so that they show the order in which the tasks will be executed
			for(NsIndex i = 0; i<indexesPerMachine.size(); ++i) {
				toCompare=&startTimes;
				sort(indexesPerMachine[i].begin(),indexesPerMachine[i].end(),dequeComparison);
			}
			//For each machine, print all the tasks in the requested format
			cout<<'[';
			cout<<"execs(m"<<(machines[0]+1)<<",[";
			NsIndex temp=indexesPerMachine[0][0];
			cout<<"t("<<tasks[temp].name<<','<<startTimes[temp]<<','<<endTimes[temp]<<')';
			for(NsIndex j = 1; j<indexesPerMachine[0].size(); ++j) {
				temp=indexesPerMachine[0][j];
				cout<<",t("<<tasks[temp].name<<','<<startTimes[temp]<<','<<endTimes[temp]<<')';
			}
			cout<<"])";
			for(NsIndex i = 1; i<indexesPerMachine.size(); ++i) {
				cout<<",\nexecs(m"<<(machines[i]+1)<<",[";
				temp=indexesPerMachine[i][0];
				cout<<"t("<<tasks[temp].name<<','<<startTimes[temp]<<','<<endTimes[temp]<<')';
				for(NsIndex j = 1; j<indexesPerMachine[i].size(); ++j) {
					temp=indexesPerMachine[i][j];
					cout<<",t("<<tasks[temp].name<<','<<startTimes[temp]<<','<<endTimes[temp]<<')';
				}
				cout<<"])";
			}
			cout<<']'<<endl;
		}
	
	}  catch (exception& exc)  {

		cerr << exc.what() << "\n";

	}  catch (...)  {

		cerr << "Unknown exception" << "\n";
	}
	
	cout<<"END\nPress Enter to continue"<<endl;
	cin.ignore();
}

bool dequeComparison(NsIndex a, NsIndex b) {
	return toCompare->at(a) < toCompare->at(b);
}