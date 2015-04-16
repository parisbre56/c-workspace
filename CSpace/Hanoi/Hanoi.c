/*****************************************************
Arxeio ylopoihshs	: Hanoi.c
Syggrafeas			: Paris - Ioannis Brentas
Skopos				: program gia Ylopoihsh Algorithmoy Hanoi me Stoives
******************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "ch4_Stack.h"
#include "TStoixeioyStoivas.h"

void Input(FILE * from, TStoivasPt S, int *n, unsigned char volflag);
/* Reads input from stream <from> and puts it in stack <S>. Sets <n> to the
 * number of elements read. Inputs that are larger than the top of the stack
 * <S> are ignored. Inputs stop being read either when an EOF is read or
 * when stack <S> is full. If <volflag> is set to a number other than 0 then
 * input is read in pairs of integers. If <volflag> is set to 0 then input is read
 * in single integers. If there is a memmory allocation problem then <n> is set to -1.
 * If no elements were read <n> is set to 0.
 */
int Move(TStoivasPt from, TStoivasPt to);
/* Moves the top element from stack <from> to stack <to>. Returns -1 if there was a
 * memory allocation failure, -2 if there was an underflow error and -3 if there
 * was an overflow error. Returns 0 if everything was OK.
 */
int recursiveHanoi(int level, TStoivasPt from, TStoivasPt to, TStoivasPt with);
/* Solves the hanoi tower problem recursively with the use of stacks. <level>
 * should be set to the number of elements int the "tower". Returns -1 if there was a
 * memory allocation failure, -2 if there was an underflow error and -3 if there
 * was an overflow error. Overflow and underflow errors should normally never happen.
 * Returns 0 if everything was OK.
 */
int Output(FILE * to, TStoivasPt S,unsigned char volflag);
/* Pops elements from stack <S> and prints them in stream <to>. If <volflag> is
 * set to a number other than 0 then output is printed in pairs of integers seperated
 * by a tab. If volflag is set to 0 then output is printed in single integers.
 * Each element is separated by the next with a newline. Returns -1 if there was a
 * memory allocation failure. Returns 0 if everything was OK.
 */
 
int main(int argc, char** argv){
	
	//**Declarations
	
	FILE * filep;
	TStoivasPt C1;
	TStoivasPt C2;
	TStoivasPt C3;
	int diskcount;
	
	int i;
	int inarg=-1;	//-1 means not used
	int outarg=-1;	//-1 means not used
	unsigned char helpflag=0;
	unsigned char volflag=0;
	
	Stoiva_dimiourgia(&C1);
	Stoiva_dimiourgia(&C2);
	Stoiva_dimiourgia(&C3);
	if (C1==NULL||C2==NULL||C3==NULL)
	{
		fprintf(stderr,"ERROR:Memory allocation failure\n");		
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_FAILURE;
	}
	
	//**Arguements
	
	for (i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		{
			if (argv[i][0]==argv[i][1])
			{
				if(!strcmp(argv[i],"--help"))
				{
					helpflag=1;
				}
				else if(!strcmp(argv[i],"--volume"))
				{
					volflag=1;
				}
				else
				{
					fprintf(stderr,"ERROR:Unknown option \"%s\"\n",argv[i]);
					fprintf(stderr,"Please use \"%s --help\" for a list of availiable commands\n",argv[0]);
					Stoiva_katastrofi(&C1);
					Stoiva_katastrofi(&C2);
					Stoiva_katastrofi(&C3);
					return EXIT_FAILURE;
				}
			}
			else
			{
				if (strchr(argv[i],'h')!=NULL)
				{
					helpflag=1;
				}
				if (strchr(argv[i],'v')!=NULL)
				{
					volflag=1;
				}
			}
		}
		else if(inarg==-1)
		{
			inarg=i;
		}
		else
		{
			outarg=i;
		}
	}
	if (helpflag)
	{
		printf("This program solves the Hanoi tower problem with use of stacks\n");
		printf("Usage:\tprintf ([flags]) [input] ([output])\n");
		printf("\tIf no output file is specified the standard output is used\n");
		printf("Flags:\t-h --help\tdisplay this help text\n");
		printf("\t-v --volume\tdisks are now cylinders and are compared based on volume\n");
		printf("\t\t\tThis means that input is read in pairs instead of signle integers\n");
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_SUCCESS;
	}
	if (inarg==-1)
	{
		fprintf(stderr,"ERROR:No input file specified\n");
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_FAILURE;
	}
	
	//**Input
	
	if ((filep=fopen(argv[inarg],"r"))==NULL)
	{
		fprintf(stderr,"ERROR:Unable to open input file <%s>: %s\n",argv[inarg],strerror(errno));
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_FAILURE;
	}
	Input(filep,C1,&diskcount,volflag);
	if (diskcount==-1)
	{
		fprintf(stderr,"ERROR:Memory allocation failure\n");
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_FAILURE;
	}
	fclose(filep);
	
	//**Main
	
	i=recursiveHanoi(diskcount,C1,C3,C2);
	if (i!=0)
	{
		fprintf(stderr,"ERROR:");
		switch (i)
		{
			case -1:
				fprintf(stderr,"Memory allocation failure\n");
			break;
			case -2:
				fprintf(stderr,"Stack underflow (this should not have happened)\n");
			break;
			case -3:
				fprintf(stderr,"Stack overflow (this should not have happened)\n");
			break;
			default:
				fprintf(stderr,"Unknown error (this should not have happened)\n");
			break;
		}
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_FAILURE;
	}
	
	//**Output
	
	if (outarg!=-1)
	{
		if ((filep=fopen(argv[outarg],"w"))==NULL)
		{
			fprintf(stderr,"ERROR:Unable to open output file <%s>: %s\n", argv[outarg],strerror(errno));
			Stoiva_katastrofi(&C1);
			Stoiva_katastrofi(&C2);
			Stoiva_katastrofi(&C3);
			return EXIT_FAILURE;
		}
		i=Output(filep,C3,volflag);
		fclose(filep);
	}
	else
	{
		i=Output(stdout,C3,volflag);
	}
	if (i!=0)
	{
		fprintf(stderr,"ERROR:");
		switch (i)
		{
			case -1:
				fprintf(stderr,"Memory allocation failure\n");
			break;
			default:
				fprintf(stderr,"Unknown error (this should not have happened)\n");
			break;
		}
		Stoiva_katastrofi(&C1);
		Stoiva_katastrofi(&C2);
		Stoiva_katastrofi(&C3);
		return EXIT_FAILURE;
	}
	
	//**Cleanup
	
	Stoiva_katastrofi(&C1);
	Stoiva_katastrofi(&C2);
	Stoiva_katastrofi(&C3);
	return EXIT_SUCCESS;
}

void Input(FILE *from, TStoivasPt S, int *n, unsigned char volflag) {
/* Diavazei stoixeia apo to from kai ta wthei sthn S. 
   Epistrefei ton arithmo twn stoixeiwn poy wthhse
*/
	TStoixeioyStoivasPt tempread;
	TStoixeioyStoivasPt temps;	
	int yperx=0;
	int ypox=0;
	*n=0;
	if (TSstoiva_create(&temps)==NULL)
	{
		*n=-1;
		return;
	}
	if (TSstoiva_create (&tempread)==NULL)
	{
		TSstoiva_destroy(&temps);
		*n=-1;
		return;
	}
	while (!feof(from))
	{
		if (volflag)
		{
			TSstoiva_readValue (from,tempread);
		}
		else
		{
			TSstoiva_readFirstValue (from,tempread);
		}
		Stoiva_PopValue(S,temps,&ypox);
		if (ypox||mikrotero(tempread,temps))
		{
			Stoiva_othisi(S,tempread,&yperx);
			if (yperx)
			{
				TSstoiva_destroy(&temps);
				TSstoiva_destroy(&tempread);
				return;
			}
			*n+=1;
		}
	}
	TSstoiva_destroy(&temps);
	TSstoiva_destroy(&tempread);	
}

int Move(TStoivasPt Source, TStoivasPt Target) {
       
/* Metaferei ena stoixeio apo thn stoiva Source sthn Target */
	int ypox;
	int yperx;
	TStoixeioyStoivasPt temps;	
	if (TSstoiva_create(&temps)==NULL)
	{
		return -1;
	}
	Stoiva_PopValue(Source,temps,&ypox);
	if(ypox)
	{
		return -2;
		TSstoiva_destroy(&temps);
	}
	Stoiva_othisi(Target,temps,&yperx);
	if (yperx)
	{
		return -3;
		TSstoiva_destroy(&temps);		
	}
	Stoiva_PopState(Source,&ypox);
	TSstoiva_destroy(&temps);	
	return 0;
}

int recursiveHanoi(int level, TStoivasPt from, TStoivasPt to, TStoivasPt with) {
/* Level einai o arithmos twn diskwn kai 
   from o arxikh stoiva , to h telikh kai with h bohuhtikh 
*/
	int i=0;
    if (level >= 1) 
	{
       i=recursiveHanoi(level - 1, from, with, to);
	   if (i!=0)
	   {
			return i;
	   }
       i=Move(from, to);
	   if (i!=0)
	   {
			return i;
	   }
       i=recursiveHanoi(level - 1, with, to, from);
	   if (i!=0)
	   {
			return i;
	   }
    }
	return i;
}

int Output(FILE *to, TStoivasPt S, unsigned char volflag){
/* Exagei stoixeia apo thn S kai ta ektypvnei sto to */
	TStoixeioyStoivasPt temps;
	if (TSstoiva_create(&temps)==NULL)
	{
		return -1;
	}
	int ypox=0;	
	while (!Stoiva_keni(S))
	{
		Stoiva_PopValue(S,temps,&ypox);
		Stoiva_PopState(S,&ypox);	
		if (volflag)
		{
			TSstoiva_writeValue (to, temps);
		}
		else
		{
			TSstoiva_writeFirstValue (to, temps);
		}
		putc('\n',to);
	}
	TSstoiva_destroy(&temps);
	return 0;
}
