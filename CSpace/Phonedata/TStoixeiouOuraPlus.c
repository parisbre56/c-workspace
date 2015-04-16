#include <stdio.h>
#include <string.h>
#include "TStoixeiouOuraPlus.h"

typedef struct GeneralElement{
	char date[DATESIZE+1];
	int sec;
	char number[NUMBERSIZE+1];
}GeneralElement;

void TSOuraPlus_create (TStoixeiouOuraPlus* Elem)
{
	*Elem=malloc(sizeof(GeneralElement));
	(*Elem)->date[0]='\0';
	(*Elem)->number[0]='\0';
	(*Elem)->sec=-1;
}

void TSOuraPlus_destroy (TStoixeiouOuraPlus* Elem)
{
	if (*Elem==NULL)
	{
		return;
	}
	free(*Elem);
	*Elem=NULL;
}

int TSOuraPlus_setValue (TStoixeiouOuraPlus target, TStoixeiouOuraPlus source){
	if (source==NULL||target==NULL)
	{
		return 1;
	}
	strcpy(target->date,source->date);
	strcpy(target->number,source->number);
	target->sec=source->sec;
	return 0;
}

int TSOuraPlus_setValue_number (TStoixeiouOuraPlus target, TStoixeiouOuraPlus source){
	if (source==NULL||target==NULL)
	{
		return 1;
	}
	strcpy(target->number,source->number);
	return 0;
}

int TSOuraPlus_readValue (FILE *from, TStoixeiouOuraPlus Elem){
	if (Elem==NULL)
	{
		return 1;
	}
    if(fscanf(from,"%s",Elem->date)==0)
	{
		return 1;
	}
	if(fscanf(from, "%d", &Elem->sec)==0)
	{
		return 1;
	}
	if(fscanf(from, "%s", Elem->number)==0)
	{
		return 1;
	}
	return 0;
}

int TSOuraPlus_readValue_number(FILE *from, TStoixeiouOuraPlus Elem)
{
	*(Elem->date)='\0';
	Elem->sec=-1;
	if(fscanf(from, "%s", Elem->number)==0)
	{
		return 1;
	}
	return 0;
}

int TSOuraPlus_readValue_date (FILE *from, TStoixeiouOuraPlus Elem){
	if (Elem==NULL)
	{
		return 1;
	}
    if(fscanf(from,"%s",Elem->date)==0)
	{
		return 1;
	}
	return 0;
}

int TSOuraPlus_writeValue(FILE *to, TStoixeiouOuraPlus Elem){
	if (Elem==NULL)
	{
		return 1;
	}
	if(fprintf(to,"%s %d %s", Elem->date, Elem->sec, Elem->number)<0)
	{
		return 1;
	}
	return 0;
}
int TSOuraPlus_writeValue_date(FILE *to, TStoixeiouOuraPlus Elem){
	if (Elem==NULL)
	{
		return 1;
	}
	if(fprintf(to,"%s", Elem->date)<0)
	{
		return 1;
	}
	return 0;
}

int TSOuraPlus_iso(TStoixeiouOuraPlus Elem1, TStoixeiouOuraPlus Elem2){
	if (Elem1==NULL||Elem2==NULL)
	{
		return -1;
	}
	return !strcmp(Elem1->date,Elem2->date);
}

int TSOuraPlus_isoNum(TStoixeiouOuraPlus Elem1, TStoixeiouOuraPlus Elem2){
	if (Elem1==NULL||Elem2==NULL)
	{
		return -1;
	}
	return !strcmp(Elem1->number,Elem2->number);
}

int TSOuraPlus_isoAll(TStoixeiouOuraPlus Elem1, TStoixeiouOuraPlus Elem2){
	if (Elem1==NULL||Elem2==NULL)
	{
		return -1;
	}
	return !strcmp(Elem1->number,Elem2->number)&&!strcmp(Elem1->number,Elem2->number)&&Elem1->sec==Elem2->sec;
}