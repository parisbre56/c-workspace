#ifndef __DB2_RECORD__
#define __DB2_RECORD__

#include <string>

class Record
{
public:
	int id;
	char name[15];
	char surname[20];
	char status[1];
	char dateOfBirth[10];
	int salary;
	char section[1];
	int daysOff;
	int prevYears;
	
	//All the variables are set to zero
	Record();
	
	//Large strings will be truncated
	Record(int Id, std::string Name, std::string Surname, char Status, 
		std::string DateOfBirth, int Salary, char Section, int DaysOff, int PrevYears);
		
	//The variable *<container> now points to the variable with name <varname> (or NULL if the name was wrong).
	//*<type> is set to 'c' for char pointer, 'i' for integer and '\0' for wrong name
	//*<size> is the size of the variable and negative for wrong name
	//<varname> is case sensitive
	void getVariable(void ** container, std::string varname, char* type=NULL, int* size=NULL);
	
	void print(void);
	
	//Changes the record to match the new values. Large strings will be truncated
	void update(int Id, std::string Name, std::string Surname, char Status, 
		std::string DateOfBirth, int Salary, char Section, int DaysOff, int PrevYears);
};

#endif
