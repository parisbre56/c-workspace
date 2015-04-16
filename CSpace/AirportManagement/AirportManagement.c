/************************************************************************
Programma pelaths	: AirportManagement.c
Syggrafeas			: 
Skopos				: skeleton main for askhsh 3
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "TSEvr.h"
#include "Evretirio.h"

#define sorted 1

void mypause(void);
void destroyEvr(void);
int mypow(int base, int exp);

EvrPtr E;

int main(int argc, char *argv[])
{
	FILE* filept;
	keyType key;
	TStoixeiouOther Data;
	struct timeval mytime[20];
	struct timeval endof1;
	struct timeval endof2;
	int timecount1=0;
	int timecount2=0;
	int timecounttotal=1;
	int inputs;
	int line;
	int changes;
	int c;
	int i;
	#if sorted
		char * input="./Data Files/IATAcodesSorted.txt";
	#else
		char * input="./Data Files/IATAcodesRandom.txt";
	#endif
	atexit(&mypause);
	
	if((filept=fopen(input,"r"))==NULL)
	{
		fprintf(stderr,"ERROR: Unable to open file %s: %s\n",input,strerror(errno));
		exit(EXIT_FAILURE);
	}
	line=0;
	c=getc(filept);
	while(c!=EOF)
	{
		if (c=='\n')
		{
			++line;
		}
		c=getc(filept);
	}
	if (line==0)
	{
		printf("Empty file, nothing to do.\n");
		exit(EXIT_SUCCESS);
	}
	E=Evr_dimiourgia(line);
	if (E==NULL)
	{
		fprintf(stderr,"ERROR: Unable to create Evretirio structure.\n");
		exit(EXIT_FAILURE);
	}
	atexit(&destroyEvr);
	
	
	strcpy(Data.ICAO,"NONE");
	strcpy(Data.country,"Unknown");
	rewind(filept);
	line=1;
	inputs=0;
	gettimeofday(&(mytime[0]),NULL);
	while (1)
	{
		c=getc(filept);
		while (c!=EOF&&(c<'A'||c>'Z'))
		{
			if (c=='\n')
			{
				++line;
			}
			c=getc(filept);
		}
		if (c==EOF)
		{
			break;
		}
		ungetc(c,filept);
		++inputs;
		if(inputs%(mypow(2,timecount1)*512)==0)
		{
			gettimeofday(&(mytime[timecounttotal]),NULL);
			++timecounttotal;
			++timecount1;
		}
		if(TSEvr_readKeyValue(filept,&key))
		{
			fprintf(stderr,"ERROR: Bad input from %s at line %d.\n",input,line);
			exit(EXIT_FAILURE);
		}
		if(Evr_Insert(E,key,Data))
		{
			fprintf(stderr,"ERROR: Unable to insert data to Evretirio structure at line %d.\n",line);
			exit(EXIT_FAILURE);
		}
	}
	
	gettimeofday(&endof1,NULL);
	fclose(filept);
	
	if((filept=fopen("./Data Files/ICAO-IATAbyCountry.txt","r"))==NULL)
	{
		fprintf(stderr,"ERROR: Unable to open file ./Data Files/ICAO-IATAbyCountry.txt: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	line=1;
	changes=0;
	gettimeofday(&(mytime[timecounttotal]),NULL);
	++timecounttotal;
	while (1)
	{
		c=getc(filept);
		while (c!=EOF&&!((c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='@'||c=='('))
		{
			if (c=='\n')
			{
				++line;
			}
			c=getc(filept);
		}
		if (c==EOF)
		{
			break;
		}
		switch (c)
		{
			case '(':
				++changes; //Changes are counted as the number of calls to Evr_ChangeData and not the actual number of changes made.
				if(changes%(mypow(2,timecount2)*512)==0)
				{
					gettimeofday(&(mytime[timecounttotal]),NULL);
					++timecounttotal;
					++timecount2;
				}
				if(TSEvr_readKeyValue(filept,&key))
				{
					fprintf(stderr,"ERROR: Bad input from ./Data Files/ICAO-IATAbyCountry.txt at line %d.\n",line);
					exit(EXIT_FAILURE);
				}
				switch(Evr_ChangeData(E,key,Data))
				{
					case -1:
						//Ignored nonexistant key.
					break;
					case 0:
						//All good.
					break;
					default: //Error (usually 1)
						fprintf(stderr,"ERROR: Unable to update data at line %d.\n",line);
						exit(EXIT_FAILURE);
					break;
				}
			break;
			case '@':
				i=0;
				c=getc(filept);
				while (c!='\n'&&c!='\t'&&i<15-1)
				{
					(Data.country)[i++]=c;
					c=getc(filept);
				}
				(Data.country)[i]='\0';
				while (c!='\n')
				{
					c=getc(filept);
				}
				line++;
			break;
			default: //A to Z and 0 to 9
				ungetc(c,filept);
				if (TSEvr_readValue(filept,&Data))
				{
					fprintf(stderr,"ERROR: Bad input from ./Data Files/ICAO-IATAbyCountry.txt at line %d.\n",line);
					exit(EXIT_FAILURE);
				}
			break;
		}
	}
	gettimeofday(&endof2,NULL);	
	fclose(filept);
	
	if((filept=fopen("./Data Files/Output.txt","w"))==NULL)
	{
		fprintf(stderr,"ERROR: Unable to open/create file ./Data Files/Output.txt: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(Evr_PrintAll(E,filept))
	{
		fprintf(stderr,"ERROR:Unable to print results to file ./Data Files/Output.txt\n");
		exit(EXIT_FAILURE);		
	}
	
	for(i=0;i<timecount1;i++)
	{
		fprintf(filept,"%d inputs: %f seconds\n",mypow(2,i)*512,mytime[i+1].tv_sec-mytime[0].tv_sec+((float)mytime[i+1].tv_usec-mytime[0].tv_usec)/1000000);
	}
	fprintf(filept,"All inputs (%d): %f seconds\n",inputs,endof1.tv_sec-mytime[0].tv_sec+((float)endof1.tv_usec-mytime[0].tv_usec)/1000000);
	for(i=0;i<timecount2;i++)
	{
		fprintf(filept,"%d changes: %f seconds\n",mypow(2,i)*512,mytime[timecount1+i+2].tv_sec-mytime[timecount1+1].tv_sec+((float)mytime[timecount1+i+2].tv_usec-mytime[timecount1+1].tv_usec)/1000000);
	}
	fprintf(filept,"All changes (%d): %f seconds\n",changes,endof2.tv_sec-mytime[timecount1+1].tv_sec+((float)endof2.tv_usec-mytime[timecount1+1].tv_usec)/1000000);
	fclose(filept);
	
	exit(EXIT_SUCCESS);
}

void mypause(void)
{
	printf("\nPress enter to continue...\n");
	fflush(stdin);
	getchar();
}

void destroyEvr(void)
{
	Evr_katastrofi(&E);
}

int mypow(int base, int exp)
{
	int i;
	int retval=1;
	for (i=0;i<exp;i++)
	{
		retval*=base;
	}
	return retval;
}