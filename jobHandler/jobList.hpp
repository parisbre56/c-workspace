#ifndef __JOB_LIST_HPP__
#define __JOB_LIST_HPP__

#include <string>

class Job {
public:
	int jobID;
	pid_t PID;
	std::string name;
	bool running;
	
	Job(int jobIDc,pid_t PIDc,std::string namec,bool runningc);
};

class JobListItem {
public:
	Job* job;
	
	JobListItem* nextJob;
	JobListItem* prevJob;
	
	JobListItem(Job* jobc);
};

class JobList {
public:
	JobListItem* start;
	JobListItem* end;
	int inList;
	
	/* Sets start and end to NULL
	 * and inList to 0
	 */
	JobList();
	/* Inserts a new job.
	 */
	void insertAtStart(Job* job);
	/* Removes an old job.
	 * Returns NULL for empty.
	 */
	Job* removeFromEnd();
	/* Removes the first job with the requested id
	 * Returns NULL if it doesn't exist
	 */
	Job* popJobWithID(int id);
	/* Removes the first job with the requested PID
	 * Returns NULL if it doesn't exist
	 */
	Job* popJobWithPID(pid_t pid);
	/* Deletes all jobs in the queue,
	 * freeing up their memory.
	 */
	void deleteAll();
	/* This only deletes the JobListItem objects.
	 * NOT the Job objects.
	 */
	~JobList();
};

#endif