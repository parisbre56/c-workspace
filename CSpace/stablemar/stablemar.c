#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//#include "arrayfun.h"
//#include "gsstabmar.h"
//#include "recstabmar.h"

//**arrayfun.h
void printintarray(int** array,unsigned int sizex,unsigned int sizey);
 /* Prints an integer array with <sizex> rows and <sizey> collums.
  */
void printuintarray(unsigned int** array,unsigned int sizex,unsigned int sizey);
 /* Prints an unsigned integer array with <sizex> rows and <sizey> collums.
  */
void* create2Dintarray(unsigned int sizex,unsigned int sizey);
 /* Returns a pointer to the beginning of an array
  * with <sizex> rows and <sizey> collums.
  * If an error occurs all memmory allocated so far is freed
  * and a null pointer is returned
  */

//**gsstabmar.h
void printpreferences(unsigned int** malegroup,unsigned int** femalegroup,unsigned int size);
 /* Prints the preferences of first the male and then the female group
  * with a <size> number of members (rows). <malegroup> and <femalegroup>
  * should be square arrays, each row with <size> numbers (collumns). It is assumed 
  * that each number points to a row in the other array. The first number in each
  * row is considered the most prefered by that person.
  */
void galeshapley(unsigned int** proposers,unsigned int** others,unsigned int size,unsigned int* result); 
 /* Returns the proposer optimal solution in <result>.
  * <proposers> and <others> hold the groups preference in a square array.
  * If proposer i is married to other j then result[i-1]=j.
  * <size> should be the number of couples to be married.
  */
  
//Probably hidden
int aremarried(int* check,unsigned int size);
 /* Returns 1 if all cells of 1D array <check> are not 0
  * else returns 0. <size> should be the size of array <check>
  */
void marryprop(unsigned int** proposers,unsigned int** others,unsigned int size,unsigned int* result,unsigned int tomarry);
/* Tries to marry proposer <tomarry> with the group <others> according to 
 * the proposers preference. <size> should be the number of couples to be married.
 */
int accepts(unsigned int** others,unsigned int* result,unsigned int size,unsigned int proposer,unsigned int other);
/* Returns 1 if <other> accepts to marry <proposer>, else returns 0.
 * <size> should be the number of couples to be married.
 */

//**recstabmar.h
void recstablemar(unsigned int** men,unsigned int** women,unsigned int* married,unsigned int size,unsigned int* solutioncount,unsigned int currentman);
/* Tries to marry each member of the <men> group to each member of the <women> group 
 * by order of male preference. The current marrieage state is held in <married>.
 * <size> should be the number of couples to be married. <solutioncount> holds
 * the number of solutions found so far with the recursive algorithm. It should be
 * started with a value of 0.<currentman> is used to tell the function for which man it is called.
 * <currentman> should be 1 when the function is called.
 * Each time a solution is found it is printed in the standard output.
 */

//Probably hidden 
int marstable(unsigned int** men,unsigned int** women,unsigned int* married,unsigned int couples);
/* Checks if the marriages are stable so far.
 * Returns 1 for stable and 0 for unstable.
 * <couples> is the number marriages so far.
 */

int single(unsigned int* married,unsigned int couples,unsigned int woman);
/* Checks if woman <woman> is married.
 * Returns 0 for married and 1 for single
 * <couples> is the number marriages so far.
 */

int main(void)
{
	unsigned int i;				//Used for "for" loops
	unsigned int j;				//Used for "for" loops
	int ch;						//Used for input
	unsigned int solutioncount;	//Used to keep track of solutions in the recursive solution
	unsigned int** men;					//Male preferences
	unsigned int** women;				//Female preferences
	unsigned int* married;				//Current marriege state
	long unsigned int number;	//The number of couples to be married
	
	//**Get input
	
	number=0;
	ch=getchar();
	while (ch!='\n') //Get the number of couples. Assumes normal input.
	{
		if (ch>='0'&&ch<='9')
		{
			number=number*10+(ch-'0');
			if (number>=UINT_MAX)
			{
				fprintf(stderr,"\n\nERROR: Input too high. Please use a number lower than %u\n\n",UINT_MAX);	
				return EXIT_FAILURE;			
			}
		}
		ch=getchar();
	}

	men=create2Dintarray(number,number);//2D array for storing male preference
	women=create2Dintarray(number,number);//2D array for storing female preference
	married=malloc(number*sizeof(int));
	/* 1D array for storing the marriage state.
	 * If person i is married to j then married[i-1]=j.
	 * If person i is not married then married[i-1]=0.
	 */
	if (men==NULL||women==NULL||married==NULL)
	{
		fprintf(stderr,"\n\nERROR: Memmory allocation failure.\n\n");
		free(men);
		free(women);
		free(married);
		return EXIT_FAILURE;
	}
	
	for (i=0;i<number;i++)
	{
		for (j=0;j<number;j++)
		{
			men[i][j]=0;
			while (ch==' '||ch=='\t'||ch=='\n')//Ignore empty. Assumes normal input
			{
				ch=getchar();
			}
			while (ch>='0'&&ch<='9')//Get male preference.
			{
				men[i][j]=men[i][j]*10+ch-'0';
				ch=getchar();
			}
		}
	}
	for (i=0;i<number;i++)
	{
		for (j=0;j<number;j++)
		{
			women[i][j]=0;
			while (ch==' '||ch=='\t'||ch=='\n')
			{
				ch=getchar();
			}
			while (ch>='0'&&ch<='9')//Get female preference.
			{
				women[i][j]=women[i][j]*10+ch-'0';
				ch=getchar();
			}
		}
	}
	
	//**Display input
	
	/*printf("%u\n",number);
	printuintarray(men,number,number);
	printuintarray(women,number,number);
	putchar('\n');*/
	
	printpreferences(men,women,number);
	putchar('\n');
	
	//**Gale-Shapley solution
	
	printf("Finding the male optimal solution with the Gale-Shapley algorithm...\n\n");
	galeshapley(men,women,number,married);
	printf("Male optimal solution:\n");
	for (i=0;i<number;i++)
	{
		printf("m%03u - w%03u\t",i+1,married[i]);
	}
	putchar('\n');
	putchar('\n');
	
	printf("Finding the female optimal solution with the Gale-Shapley algorithm...\n\n");
	galeshapley(women,men,number,married);
	printf("Female optimal solution:\n");
	for (j=1;j<=number;j++)
	{
		for(i=0;i<number;i++)
		{
			if (married[i]==j)
			{
				printf("m%03u - w%03u\t",j,i+1);
			}
		}
	}
	putchar('\n');
	putchar('\n');
	
	//**Recursive solution
	
	for (i=0;i<number;i++)
	{
		married[i]=0;
	}
	solutioncount=0;
	printf("Finding all solutions with backtracking via recursion...\n");
	recstablemar(men,women,married,number,&solutioncount,1);
	if (solutioncount!=1)
	{
		printf("\nFound %u solutions\n",solutioncount);
	}
	else
	{
		printf("\nFound 1 solution\n");
	}
	
	//**Free memmory
	for (i=0;i<number;i++)
	{
		free(men[i]);
		free(women[i]);
	}
	free(men);
	free(women);
	free(married);
	return EXIT_SUCCESS;
}

//**arrayfun.c
//#include <stdlib.h>
//#include <stdio.h>

void printintarray(int** array,unsigned int sizex,unsigned int sizey)
{
	int i,j;
	for (i=0;i<sizex;i++)
	{
		for (j=0;j<sizey;j++)
		{
			printf("  %d",array[i][j]);
		}
		putchar('\n');
	}
}
void printuintarray(unsigned int** array,unsigned int sizex,unsigned int sizey)
{
	int i,j;
	for (i=0;i<sizex;i++)
	{
		for (j=0;j<sizey;j++)
		{
			printf("  %u",array[i][j]);
		}
		putchar('\n');
	}
}
void* create2Dintarray(unsigned int rows,unsigned int collumns)
{
	int i;
	int j;
	int **ptr;
	ptr = malloc(rows * sizeof(int *));
	if (ptr==NULL)
	{
		return NULL;
	}
	for(i = 0; i < rows; i++)
	{
		ptr[i] = malloc(collumns * sizeof(int));
		if (ptr[i]==NULL)
		{
			for(j=0;j<i;j++)
			{
				free(ptr[j]);
			}
			free(ptr);
			return NULL;
		}
	}
	return ptr;
}

//**gsstabmar.c
//#include <stdio.h>

void printpreferences(unsigned int** malegroup,unsigned int** femalegroup,unsigned int size)
{
	int i,j;
	for (i=0;i<size;i++)
	{
		printf("m%03u order of preferences:",i+1);
		for(j=0;j<size;j++)
		{
			printf(" w%03u",malegroup[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
	for (i=0;i<size;i++)
	{
		printf("w%03u order of preferences:",i+1);
		for(j=0;j<size;j++)
		{
			printf(" m%03u",femalegroup[i][j]);
		}
		putchar('\n');
	}
}
void galeshapley(unsigned int** proposers,unsigned int** others,unsigned int size,unsigned int* result)
{
	int i;
	for (i=0;i<size;i++)
	{
		result[i]=0;
	}
	while (!aremarried(result,size))
	{
		for(i=0;i<size;i++)//For all proposers
		{
			if (result[i]==0)//If they are not married
			{
				marryprop(proposers,others,size,result,i+1);//Then marry him/her
			}
		}
	}
}
int aremarried(int* check,unsigned int size)
{
	int i;
	for (i=0;i<size;i++)
	{
		if (check[i]==0)
		{
			return 0;
		}
	}
	return 1;	
}
void marryprop(unsigned int** proposers,unsigned int** others,unsigned int size,unsigned int* result,unsigned int tomarry)
{
	int i;
	for (i=0;result[tomarry-1]==0;i++)//While man <tomarry> is not married
	{
		//printf("m%03d proposes to w%03d, ",tomarry,proposers[tomarry-1][i]);
		if (accepts(others,result,size,tomarry,proposers[tomarry-1][i]))
		{
			//printf("m%03d gets married to w%03d.\n",tomarry,proposers[tomarry-1][i]);
			result[tomarry-1]=proposers[tomarry-1][i];
		}
	}
}
int accepts(unsigned int** others,unsigned int* result,unsigned int size,unsigned int proposer,unsigned int other)
{
	int i;
	int curmarried=0;
	for (i=0;i<size;i++)//For all men
	{
		if (result[i]==other)//If they are married to <other>
		{
			curmarried=i+1;//Store this
		}
	}
	if (curmarried==0)//If she is not married
	{
		//printf("she accepts.\n");
		return 1;
	}
	for (i=0;1;i++)//For all men
	{
		if(others[other-1][i]==curmarried)//If she prefers the one she is currently married to
		{
			//printf("she refuses.\n");
			//printf("w%03d stays married to m%03d.\n",other,curmarried);
			return 0;
		}
		if(others[other-1][i]==proposer)//If she prefers the proposer
		{
			//printf("she accepts.\n");
			//printf("w%03d divorces m%03d.\n",other,curmarried);
			result[curmarried-1]=0;
			return 1;
		}
	}
}

//**rectabmar.c
//#include <stdio.h>

void recstablemar(unsigned int** men,unsigned int** women,unsigned int* married,unsigned int size,unsigned int* solutioncount,unsigned int currentman)
{
	//int j;
	int i;
	int res;
	if (currentman==size+1)//If all have been married
	{
		printf("\nSolution %u:\n",++(*solutioncount));//Print the solution
		for (i=0;i<size;i++)
		{
			printf("m%03u - w%03u\t",i+1,married[i]);
		}
		putchar('\n');
		putchar('\n');
		return;
	}
	for (i=0;i<size;i++)//For all women
	{
		//printf("Trying to marry m%03d to w%03d\n",currentman,men[currentman-1][i]);
		if (single(married,currentman,men[currentman-1][i]))
		{
			married[currentman-1]=men[currentman-1][i];//Marry currentman to <men>[<currentman>-1][<i>]
			/*for (j=0;j<size;j++)
			{
				printf("m%03d - w%03d\t",j+1,married[j]);
			}
			putchar('\n');*/
			if (marstable(men,women,married,currentman))//If the marriages so far are stable
			{
				//printf("Success\n");
				recstablemar(men,women,married,size,solutioncount,currentman+1);//Check the next man
			}
			married[currentman-1]=0;//Divorce currentman
		}
	}
}
int marstable(unsigned int** men,unsigned int** women,unsigned int* married,unsigned int couples)
{
	int i;
	int j;
	int k;
	int curmarried;
	for(i=0;i<couples;i++)//For all married men
	{
		for(j=0;men[i][j]!=married[i];j++)//For all women man <i>+1 likes more than the one he is currently married
		{
			curmarried=0; //The man woman <men>[<i>][<j>] is married to
			for(k=0;k<couples;k++)//For all men
			{
				if (married[k]==men[i][j])//If man <k> is married to woman <men>[<i>][<j>]
				{
					curmarried=k+1;//Store this
				}
			}
			if (curmarried!=0)//If she isn't married, ignore her
			{
				for(k=0;women[men[i][j]-1][k]!=curmarried;k++)//For all men woman <men>[<i>][<j>] likes more than the one she is currently married
				{
					if(women[men[i][j]-1][k]==i+1)//If one of those men is man <i>+1 (who also likes her more than the one he is currently married)
					{
						//printf("Failed, unstable\n");
						//printf("Blocking pair:m%03d - w%03d\n",i+1,men[i][j]);
						return 0;//Then the marriages are unstable
					}
				}
			}
		}
	}
	return 1;
}
int single(unsigned int* married,unsigned int couples,unsigned int woman)
{
	int i;
	int curmarried=0;
	for (i=0;i<couples;i++)//For all married men
	{
		if (married[i]==woman)//If they are married to <woman>
		{
			//printf("Failed, married.\n");
			return 0;//<woman> is not single
		}
	}
	return 1;//<woman> is single
}