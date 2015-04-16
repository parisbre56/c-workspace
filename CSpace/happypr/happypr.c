/*	Author:			Paris-Ioannis Brentas
 *	Mail:			sdi1100070@di.uoa.gr 
 *  AM:				1115201100070
 * 	Last modified:	25 November 2011
 * 	Description:	This program uses the systems' calendar time as a seed
 *					to generate pseudorandom numbers. If one of them is a
 *					happy prime it is given as output. Finally the percentage
 * 					of the numbers that were checked which were happy primes
 * 					is given.
 */	

//Header files
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Definitions
#define SEED starttime	//Default: starttime
#define COMPUTATIONS 100000	//Default: 100000
#define true	1
#define false	0
typedef short unsigned int bool;

//Function prototypes
inline bool isPrime(unsigned int);	//Returns true if the number is prime else returns false. 1 and 0 are not primes.
inline bool isHappy(unsigned int);	//Returns true if the number is happy else false. 0 is unhappy.

int main(void)
{
	//Variable declerations
	int starttime;
	unsigned int i;
	unsigned int happycount;
	unsigned int checknum;
	
	//Variable initialisation
	happycount = 0;
	starttime = time(NULL);	//Storing the starttime ensures that the results for a given seed are independent of system load.
							//(i.e. if the time displayed during an execution is sefined as SEED the same results will be displayed on the same computer.)
	
	//Input check and error handling
	if (starttime==-1)
	{
		fprintf(stderr,"Error:Could not retrieve the calendar time. Stopping.\n"); 
		return EXIT_FAILURE;
	}
	
	srand((unsigned int)SEED);
	printf("Current time is %d\n\n",SEED);
	for (i=1;i<=COMPUTATIONS;i++)
	{
		checknum=((rand()%32768)+1)*((rand()%32768)+1)+1;
		//Checking whether or not a number is happy before checking if it's prime makes the program about 8 times faster than the reverse. 
		//(If you don't believe me, try it out yourself ;) )
		if (isHappy(checknum))
		{
			if (isPrime(checknum))
			{
				happycount++;
				printf("%u is happy prime\n",checknum);
			}
		}
	}
	printf("\nFound %5.2f%% happy prime numbers\n",((float)happycount/COMPUTATIONS)*100);
	return EXIT_SUCCESS;
}

inline bool isPrime(unsigned int checkint)
/*	This function returns true if the number given as checkint when called
 * 	is prime otherwise it returns false.
 * 	It is written seperately for convenience and portability. To use this 
 * 	function in another program, simply uncomment the commented code below
 * 	as necessary.
 * 	The program must also support bool type variables, either by definition,
 * 	as in this program, or with the inclusion of <stdbool.h>.
 */
{
	/*
	#include <math.h>
	if (checkint==0||checkint==1)
	{
		return false;
	}
	*/
	int div;
	unsigned int limit;
	div = 2;
	limit = sqrt(checkint);
	while (div<=limit)
	{
		if (checkint%div==0)
		{
			return false;
		}		
		div++;
		if (div%2==0)
		{
			div++;
			if (div%3==0)
			{
				div+=2;
			}
		}
	}	
	return true;
}

inline bool isHappy(unsigned int checkint)
/*	This function returns true if the number given as checkint when called
 * 	is happy otherwise it returns false. 
 * 	It is written seperately for convenience and portability. To use this 
 * 	function in another program, simply uncomment the commented code below
 * 	as necessary.
 * 	The program must also support bool type variables, either by definition,
 * 	as in this program, or with the inclusion of <stdbool.h>.
 */ 
{
	/*
	if (checkint==0)
	{
		return false;
	}
	*/
	unsigned int tempint;	
	while(true)	//A condition is unnecessery, as all numbers are either happy or unhappy.
	{
		tempint=0;	//The next number in the sequence will be stored here
		while (checkint!=0) 
		{
			tempint+=(checkint%10)*(checkint%10);	//Add the square of the last digit
			checkint/=10;	//Remove last digit
		} 

			/*	If the next number in the sequence is 1, then all the numbers in the 
			*	sequence (including the number given when isHappy is called) are happy.
			*/
		if (tempint==1)
		{			
			return true;
		}
			/*	If the next number in the sequence is one of the following, then all
			 *	the numbers in the sequence (including the number given when isHappy 
			 * 	is called) are not happy.
			 */
		if (tempint==4||tempint==16||tempint==37||tempint==58||tempint==89||tempint==145||tempint==42||tempint==20)
		{
			return false;
		}
		
	checkint=tempint;	//Check the next number in the sequence
	}
}
