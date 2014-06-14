#include "jobList.hpp"

#include <cstdlib>
#include <cassert>

Job::Job(int jobIDc,pid_t PIDc,std::string namec,bool runningc) {
	jobID=jobIDc;
	PID=PIDc;
	name.assign(namec);
	running=runningc;
}

JobListItem::JobListItem(Job* jobc) {
	job=jobc;
	this->nextJob=NULL;
	prevJob=NULL;
}

JobList::JobList() {
	start=NULL;
	end=NULL;
	inList=0;
}

void JobList::insertAtStart(Job* job) {
	JobListItem* jobItem=new JobListItem(job);
	jobItem->nextJob=start;
	jobItem->prevJob=NULL;
	if(start!=NULL) {
		start->prevJob=jobItem;
	}
	start=jobItem;
	if(end==NULL) {
		assert(inList==0);
		end=jobItem;
	}
	++inList;
}

Job* JobList::removeFromEnd() {
	if(end==NULL) {
		assert(inList==0);
		return NULL;
	}
	JobListItem* tempJobItem=end;
	end=end->prevJob;
	if(start==tempJobItem) {
		start=NULL;
		assert(inList==1);
	}
	else {
		end->nextJob=NULL;
	}
	--inList;
	tempJobItem->nextJob=NULL;
	tempJobItem->prevJob=NULL;
	Job* retJob=tempJobItem->job;
	delete tempJobItem;
	return retJob;
}

Job* JobList::popJobWithID(int id) {
	if(start==NULL) {
		assert(inList==0);
		return NULL;
	}
	JobListItem* tempJobItem=start;
	while(tempJobItem!=NULL) {
		if(tempJobItem->job->jobID==id) {
			if(start==tempJobItem) {
				start=tempJobItem->nextJob;
				if(start!=NULL) {
					start->prevJob=NULL;
				}
				else {
					assert(inList==1);
				}
			}
			else {
				tempJobItem->prevJob->nextJob=tempJobItem->nextJob;
			}
			if(end==tempJobItem) {
				end=tempJobItem->prevJob;
				if(end!=NULL) {
					end->nextJob=NULL;
				}
				else {
					assert(inList==1);
				}
			}
			else {
				tempJobItem->nextJob->prevJob=tempJobItem->prevJob;
			}
			
			
			tempJobItem->nextJob=NULL;
			tempJobItem->prevJob=NULL;
			--inList;
			Job* retJob=tempJobItem->job;
			delete tempJobItem;
			return retJob;
		}
		
		tempJobItem=tempJobItem->nextJob;
	}
	
	return NULL;
}

Job* JobList::popJobWithPID(pid_t pid) {
	if(start==NULL) {
		assert(inList==0);
		return NULL;
	}
	JobListItem* tempJobItem=start;
	while(tempJobItem!=NULL) {
		if(tempJobItem->job->jobID==pid) {
			if(start==tempJobItem) {
				start=tempJobItem->nextJob;
				if(start!=NULL) {
					start->prevJob=NULL;
				}
				else {
					assert(inList==1);
				}
			}
			else {
				tempJobItem->prevJob->nextJob=tempJobItem->nextJob;
			}
			if(end==tempJobItem) {
				end=tempJobItem->prevJob;
				if(end!=NULL) {
					end->nextJob=NULL;
				}
				else {
					assert(inList==1);
				}
			}
			else {
				tempJobItem->nextJob->prevJob=tempJobItem->prevJob;
			}
			
			
			tempJobItem->nextJob=NULL;
			tempJobItem->prevJob=NULL;
			--inList;
			Job* retJob=tempJobItem->job;
			delete tempJobItem;
			return retJob;
		}
		
		tempJobItem=tempJobItem->nextJob;
	}
	
	return NULL;
}

void JobList::deleteAll() {
	Job* tempJob;
	while((tempJob=this->removeFromEnd())!=NULL) {
		delete tempJob;
	}
}

JobList::~JobList() {
	JobListItem* tempJobItem=this->start;
	JobListItem* toDelete=NULL;
	while(tempJobItem!=NULL) {
		toDelete=tempJobItem;
		tempJobItem=tempJobItem->nextJob;
		delete toDelete;
	}
}