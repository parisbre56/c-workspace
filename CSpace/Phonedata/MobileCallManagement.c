#include <stdio.h>
#include <stdlib.h>
#include "TStoixeiouOuraPlus.h"
#include "OuraPlus.h"

TOuraPlus Received=NULL, UnAnswered=NULL, Called=NULL;
int InitializeLists(void);

int PopulateLists(FILE *in);
/* Reads call data from in and puts them at the end of the appropriate list. 
   If a list is longer than limitSize delete oldest call. 
   Format of File
   L #number           // #of Calls in list (if list is longer oldest calls are deleted) 
   R dd/mm/yyyy time(sec) telephone         // put into Received Call List 
   U dd/mm/yyyy time(sec) telephone         // put into Unansewred Call List
   C dd/mm/yyyy time(sec) telephone         // put into Called Call List
*/ 

int ShowList(FILE *out, TOuraPlus CallList, int order);
/* Prints to out all data calls, one per line (date, number).
   if order >= 0 oldest to newest, otherwise newest to oldest
*/

int DeleteOrChangeCalls(FILE *in);
/* Reads pairs of telephone numbers from in, finds call with the first and replaces with second 
   Format of File
   A #telephone     #telephone // change
   D #telephone                // delete
*/
int SearchCallList(FILE * in, FILE *out);
/* Reads telephones from in and searches CallList and writes Call Data
   R dd/mm/yyyy     //one per line
   U dd/mm/yyyy     //one per line
   C dd/mm/yyyy     //one per line
*/

void DestroyLists(void);

void mypause(void);

int main(int argc, char *argv[])
{
	FILE* myfile=NULL;
	
	atexit(&mypause);//Pause at exit
	
	if(InitializeLists())
	{
		fprintf(stderr,"ERROR:Unable to initialise lists\n");
		exit(EXIT_FAILURE);
	}
	atexit(&DestroyLists);//Destroy the lists at exit
	if((myfile=fopen("./Input/Calls.txt","r"))==NULL)
	{
		fprintf(stderr,"ERROR:Unable to open file Calls.txt\n");
		exit(EXIT_FAILURE);
	}
    if(PopulateLists(myfile))
	{
		fprintf(stderr,"ERROR:Unable to populate lists\n");
		fclose(myfile);
		exit(EXIT_FAILURE);
	}
	fclose(myfile);
    if(ShowList(stdout, Received, 1))
	{
		fprintf(stderr,"ERROR:Unable to show list Received\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, Called, 1))
	{
		fprintf(stderr,"ERROR:Unable to show list Called\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, UnAnswered, 1))
	{
		fprintf(stderr,"ERROR:Unable to show list UnAnswered\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, Received, -1))
	{
		fprintf(stderr,"ERROR:Unable to show list Received\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, Called, -1))
	{
		fprintf(stderr,"ERROR:Unable to show list Called\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, UnAnswered, -1))
	{
		fprintf(stderr,"ERROR:Unable to show list UnAnswered\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
	if((myfile=fopen("./Input/DelOrChNumber.txt","r"))==NULL)
	{
		fprintf(stderr,"ERROR:Unable to open file DelOrChNumber.txt\n");
		exit(EXIT_FAILURE);
	}
    if(DeleteOrChangeCalls(myfile))
	{
		fprintf(stderr,"ERROR:Unable to delete or change calls\n");
		fclose(myfile);
		exit(EXIT_FAILURE);
	}
	fclose(myfile);
	putchar('\n');
	putchar('\n');
    if(ShowList(stdout, Received, 1))
	{
		fprintf(stderr,"ERROR:Unable to show list Received\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, Called, 1))
	{
		fprintf(stderr,"ERROR:Unable to show list Called\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, UnAnswered, 1))
	{
		fprintf(stderr,"ERROR:Unable to show list UnAnswered\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, Received, -1))
	{
		fprintf(stderr,"ERROR:Unable to show list Received\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, Called, -1))
	{
		fprintf(stderr,"ERROR:Unable to show list Called\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
    if(ShowList(stdout, UnAnswered, -1))
	{
		fprintf(stderr,"ERROR:Unable to show list UnAnswered\n");
		exit(EXIT_FAILURE);
	}
	putchar('\n');
	if((myfile=fopen("./Input/SearchCalls.txt","r"))==NULL)
	{
		fprintf(stderr,"ERROR:Unable to open file SearchCalls.txt\n");
		exit(EXIT_FAILURE);
	}
    if(SearchCallList(myfile, stdout))
	{
		fprintf(stderr,"ERROR:Unable to search call list\n");		
	}
	fclose(myfile);
	exit(EXIT_SUCCESS);
}

int InitializeLists(void){
	OuraPlus_dimiourgia(&Received);
	if(Received==NULL)
	{
		return 1;
	}
	OuraPlus_dimiourgia(&UnAnswered);
	if(UnAnswered==NULL)
	{
		OuraPlus_katastrofi(&Received);
		return 1;
	}
	OuraPlus_dimiourgia(&Called);
	if(Called==NULL)
	{
		OuraPlus_katastrofi(&Received);
		OuraPlus_katastrofi(&UnAnswered);		
		return 1;
	}
	return 0;
}

int PopulateLists(FILE *in){
	int tempchar;
	int i=0;
	TStoixeiouOuraPlus tempElem;
	TSOuraPlus_create(&tempElem);
	if(tempElem==NULL)
	{
		return 1;
	}
	while(!feof(in))
	{
		tempchar=getc(in);
		while(tempchar==' '||tempchar=='\t'||tempchar=='\n'||tempchar=='\r'/*Carriege return for windows*/)
		{
			tempchar=getc(in);
		}
		if (tempchar==EOF)
		{
			break;
		}
		else if (tempchar=='M')
		{
			if(!fscanf(in,"%d",&i))
			{
				TSOuraPlus_destroy(&tempElem);
				return 1;
			}
		}
		else
		{
			if(TSOuraPlus_readValue(in,tempElem))
			{
				TSOuraPlus_destroy(&tempElem);
				return 1;
			}
			switch (tempchar)
			{
				case 'R':
				if(OuraPlus_eisagogiTelos(Received,tempElem))
				{
					TSOuraPlus_destroy(&tempElem);
					return 1;
				}
				break;
				case 'U':
				if(OuraPlus_eisagogiTelos(UnAnswered,tempElem))
				{
					TSOuraPlus_destroy(&tempElem);
					return 1;
				}
				break;
				case 'C':
				if(OuraPlus_eisagogiTelos(Called,tempElem))
				{
					TSOuraPlus_destroy(&tempElem);
					return 1;
				}
				break;
				default:
					TSOuraPlus_destroy(&tempElem);			
					return 1;
				break;
			}
		}
		while(OuraPlus_megethos(Received)>i)
		{
			if(OuraPlus_exagogiArxh(Received))
			{
				TSOuraPlus_destroy(&tempElem);			
				return 1;
			}
		}
		while(OuraPlus_megethos(UnAnswered)>i)
		{
			if(OuraPlus_exagogiArxh(UnAnswered))
			{
				TSOuraPlus_destroy(&tempElem);			
				return 1;
			}
		}
		while(OuraPlus_megethos(Called)>i)
		{
			if(OuraPlus_exagogiArxh(Called))
			{
				TSOuraPlus_destroy(&tempElem);			
				return 1;
			}
		}
	}
	TSOuraPlus_destroy(&tempElem);
	return 0;
}

int ShowList(FILE *out, TOuraPlus CallList, int order){
	int i;
	int (*listmove)(const TOuraPlus)=NULL;
	TStoixeiouOuraPlus tempElem;
	if(order==1)
	{
		if(OuraPlus_prwtos(CallList))
		{
			return 1;
		}
		listmove=&OuraPlus_epomenos;
	}
	else if(order==-1)
	{
		if(OuraPlus_teleytaios(CallList))
		{
			return 1;
		}
		listmove=&OuraPlus_proigoymenos;
	}
	else
	{
		return 1;
	}
	TSOuraPlus_create(&tempElem);
	if(tempElem==NULL)
	{
		return 1;
	}
	for(i=OuraPlus_megethos(CallList);i>0;i--)
	{
		if(OuraPlus_periexomenoTrexon(CallList,&tempElem))
		{
			TSOuraPlus_destroy(&tempElem);
			return 1;
		}
		if(TSOuraPlus_writeValue(out,tempElem))
		{
			TSOuraPlus_destroy(&tempElem);
			return 1;
		}
		putc('\n',out);
		if((*listmove)(CallList))
		{
			TSOuraPlus_destroy(&tempElem);
			return 1;
		}
	}
	TSOuraPlus_destroy(&tempElem);
	return 0;
}
int DeleteOrChangeCalls(FILE *in){
	int tempchar;
	TStoixeiouOuraPlus toFind;
	TSOuraPlus_create(&toFind);
	if(toFind==NULL)
	{
		return 1;
	}
	TStoixeiouOuraPlus ChangeTo;
	TSOuraPlus_create(&ChangeTo);
	if(ChangeTo==NULL)
	{
		TSOuraPlus_destroy(&toFind);
		return 1;
	}
	while(!feof(in))
	{
		tempchar=getc(in);
		while(tempchar==' '||tempchar=='\t'||tempchar=='\n'||tempchar=='\r'/*Carriege return for windows*/)
		{
			tempchar=getc(in);
		}
		if (tempchar==EOF)
		{
			break;
		}
		switch (tempchar)
		{
			case 'D':
				if(TSOuraPlus_readValue_number(in,toFind))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				if(OuraPlus_prwtos(Called)||OuraPlus_prwtos(UnAnswered)||OuraPlus_prwtos(Received))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				if(OuraPlus_anazitisi(Called,toFind,&TSOuraPlus_isoNum)||OuraPlus_anazitisi(UnAnswered,toFind,&TSOuraPlus_isoNum)||OuraPlus_anazitisi(Received,toFind,&TSOuraPlus_isoNum))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				while(1)
				{
					if(OuraPlus_anazitisi(Called,toFind,&TSOuraPlus_isoNum))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_periexomenoTrexon(Called,&toFind))
					{
						break;
					}
					OuraPlus_diagrafiTrexon(Called);
				}
				while(1)
				{
					if(OuraPlus_anazitisi(UnAnswered,toFind,&TSOuraPlus_isoNum))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_periexomenoTrexon(UnAnswered,&toFind))
					{
						break;
					}
					OuraPlus_diagrafiTrexon(UnAnswered);
				}
				while(1)
				{
					if(OuraPlus_anazitisi(Received,toFind,&TSOuraPlus_isoNum))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_periexomenoTrexon(Received,&toFind))
					{
						break;
					}
					OuraPlus_diagrafiTrexon(Received);
				}
			break;
			case 'A':
				if(TSOuraPlus_readValue_number(in,toFind))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				if(TSOuraPlus_readValue_number(in,ChangeTo))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				if(OuraPlus_prwtos(Called)||OuraPlus_prwtos(UnAnswered)||OuraPlus_prwtos(Received))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				if(OuraPlus_anazitisi(Called,toFind,&TSOuraPlus_isoNum)||OuraPlus_anazitisi(UnAnswered,toFind,&TSOuraPlus_isoNum)||OuraPlus_anazitisi(Received,toFind,&TSOuraPlus_isoNum))
				{
					TSOuraPlus_destroy(&toFind);
					TSOuraPlus_destroy(&ChangeTo);
					return 1;
				}
				while(1)
				{
					if(OuraPlus_anazitisi(Called,toFind,&TSOuraPlus_isoNum))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_periexomenoTrexon(Called,&toFind))
					{
						break;
					}
					if(OuraPlus_allagiTrexon_num(Called,ChangeTo))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_epomenos(Called))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
				}
				while(1)
				{
					if(OuraPlus_anazitisi(UnAnswered,toFind,&TSOuraPlus_isoNum))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_periexomenoTrexon(UnAnswered,&toFind))
					{
						break;
					}
					if(OuraPlus_allagiTrexon_num(UnAnswered,ChangeTo))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_epomenos(UnAnswered))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
				}
				while(1)
				{
					if(OuraPlus_anazitisi(Received,toFind,&TSOuraPlus_isoNum))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_periexomenoTrexon(Received,&toFind))
					{
						break;
					}
					if(OuraPlus_allagiTrexon_num(Received,ChangeTo))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
					if(OuraPlus_epomenos(Received))
					{
						TSOuraPlus_destroy(&toFind);
						TSOuraPlus_destroy(&ChangeTo);
						return 1;
					}
				}
			break;
			default:
				TSOuraPlus_destroy(&ChangeTo);
				TSOuraPlus_destroy(&toFind);
				return 1;
			break;
		}
	}
	TSOuraPlus_destroy(&ChangeTo);
	TSOuraPlus_destroy(&toFind);
	return 0;
}
int SearchCallList(FILE *in, FILE *out){
	int tempchar;
	TOuraPlus* target;
	TStoixeiouOuraPlus toFind;
	TSOuraPlus_create(&toFind);
	if(toFind==NULL)
	{
		return 1;
	}
	while(!feof(in))
	{
		tempchar=getc(in);
		while(tempchar==' '||tempchar=='\t'||tempchar=='\n'||tempchar=='\r')
		{
			tempchar=getc(in);
		}
		if (tempchar==EOF)
		{
			break;
		}
		switch (tempchar)
		{
			case 'R':
				target=&Received;
			break;
			case 'U':
				target=&UnAnswered;
			break;
			case 'C':
				target=&Called;
			break;
			default:
				TSOuraPlus_destroy(&toFind);
				return 1;
			break;
		}
		if(OuraPlus_prwtos(*target))
		{
			TSOuraPlus_destroy(&toFind);
			return 1;
		}
		if(TSOuraPlus_readValue_date(in,toFind))
		{
			TSOuraPlus_destroy(&toFind);
			return 1;
		}
		if(OuraPlus_anazitisi(*target,toFind,&TSOuraPlus_iso))
		{
			TSOuraPlus_destroy(&toFind);
			return 1;
		}
		if(OuraPlus_anazitisi(*target,toFind,&TSOuraPlus_iso))
		{
			TSOuraPlus_destroy(&toFind);
			return 1;
		}
		if(!OuraPlus_periexomenoTrexon(*target,&toFind))
		{
			putc(tempchar,out);
			putc(' ',out);
			if(TSOuraPlus_writeValue(out,toFind))
			{
				TSOuraPlus_destroy(&toFind);
				return 1;
			}
			putc('\n',out);
		}
		else
		{
			putc(tempchar,out);
			putc(' ',out);
			if(TSOuraPlus_writeValue_date(out,toFind))
			{
				TSOuraPlus_destroy(&toFind);
				return 1;
			}
			fprintf(out," : No calls found\n");
		}
	}
	TSOuraPlus_destroy(&toFind);
	return 0;
}

void DestroyLists(void){
	OuraPlus_katastrofi(&Received);
	OuraPlus_katastrofi(&UnAnswered);	
	OuraPlus_katastrofi(&Called);	
}


void mypause(void)
{
	printf("\nPress enter to continue...\n");
	fflush(stdin);
	getchar();
}