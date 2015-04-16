#include "DB2_Record.hpp"

#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;


Record::Record()
{
	id=0;
	name[0]='\0';
	surname[0]='\0';
	status[0]='\0';
	dateOfBirth[0]='\0';
	salary=0;
	section[0]='\0';
	daysOff=0;
	prevYears=0;
}


Record::Record(int Id, std::string Name, std::string Surname, char Status, 
	std::string DateOfBirth, int Salary, char Section, int DaysOff, int PrevYears)
{
	id=Id;
	strncpy(name,Name.c_str(),15);
	strncpy(surname,Surname.c_str(),20);
	status[0]=Status;
	strncpy(dateOfBirth,DateOfBirth.c_str(),10);
	salary=Salary;
	section[0]=Section;
	daysOff=DaysOff;
	prevYears=PrevYears;
}


void Record::getVariable(void ** container, std::string varname, char* type, int* length)
{
	if(length!=NULL)
	{
		*length=sizeof(int);
	}
	
	if(!varname.compare("id"))
	{
		*container=&id;
		if(type!=NULL)
		{
			*type='i';
		}
	}
	else if(!varname.compare("name"))
	{
		*container=name;
		if(type!=NULL)
		{
			*type='c';
		}
		if(length!=NULL)
		{
			*length=15;
		}
	}
	else if(!varname.compare("surname"))
	{
		*container=surname;
		if(type!=NULL)
		{
			*type='c';
		}
		if(length!=NULL)
		{
			*length=20;
		}
	}
	else if(!varname.compare("status"))
	{
		*container=status;
		if(type!=NULL)
		{
			*type='c';
		}
		*length=1;
	}
	else if(!varname.compare("dateOfBirth"))
	{
		*container=dateOfBirth;
		if(type!=NULL)
		{
			*type='c';
		}
		if(length!=NULL)
		{
			*length=10;
		}
	}
	else if(!varname.compare("salary"))
	{
		*container=&salary;
		if(type!=NULL)
		{
			*type='i';
		}
	}
	else if(!varname.compare("section"))
	{
		*container=section;
		if(type!=NULL)
		{
			*type='c';
		}
		if(length!=NULL)
		{
			*length=1;
		}
	}
	else if(!varname.compare("daysOff"))
	{
		*container=&daysOff;
		if(type!=NULL)
		{
			*type='i';
		}
	}
	else if(!varname.compare("prevYears"))
	{
		*container=&prevYears;
		if(type!=NULL)
		{
			*type='i';
		}
	}
	else
	{
		*container=NULL;
		if(type!=NULL)
		{
			*type='\0';
		}
		if(length!=NULL)
		{
			*length=-1;
		}
	}
}

void Record::print(void)
{
	printf("Id: %i\n",id);
	printf("Name: %.15s\n", name);
	printf("Surname: %.20s\n", surname);
	printf("Status: %c\n",status[0]);
	printf("Date of birth: %.10s\n", dateOfBirth);
	printf("Salary: %i\n",salary);
	printf("Section: %c\n",section[0]);
	printf("Days off: %i\n",daysOff);
	printf("Previous years: %i\n",prevYears);
}

void Record::update(int Id, std::string Name, std::string Surname, char Status, 
	std::string DateOfBirth, int Salary, char Section, int DaysOff, int PrevYears)
{
	id=Id;
	strncpy(name,Name.c_str(),15);
	strncpy(surname,Surname.c_str(),20);
	status[0]=Status;
	strncpy(dateOfBirth,DateOfBirth.c_str(),10);
	salary=Salary;
	section[0]=Section;
	daysOff=DaysOff;
	prevYears=PrevYears;
}
