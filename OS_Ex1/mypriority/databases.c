#include "databases.h"
#include <stdlib.h>
#include <string.h>

int PRE_Insert(precentileNode** List, double debt, long long afm)
{
	//Check for empty list
	if((*List)==NULL)
	{
		(*List)=malloc(sizeof(precentileNode));
		if((*List)==NULL)
		{
			return -1;
		}
		(*List)->next=NULL;
		(*List)->afm=afm;
		(*List)->debt=debt;
		return 0;
	}
	//Check to see if debt greater than first
	if((*List)->debt<debt)
	{
		precentileNode* temp = (*List);
		(*List)=malloc(sizeof(precentileNode));
		if((*List)==NULL)
		{
			return -1;
		}
		(*List)->next=temp;
		(*List)->debt=debt;
		(*List)->afm=afm;
		return 0;
	}
	//Find either the end or the correct position
	precentileNode* Pointer=(*List);
	while(Pointer->next!=NULL&&Pointer->next->debt>debt)
	{
		Pointer=Pointer->next;
	}
	//Correct position found
	precentileNode* temp = Pointer->next;
	Pointer->next=malloc(sizeof(precentileNode));
	Pointer=Pointer->next;
	if(Pointer==NULL)
	{
		return -1;
	}
	Pointer->next=temp;
	Pointer->debt=debt;
	Pointer->afm=afm;
	return 0;
}
int PRE_Remove(precentileNode** List, long long afm)
{
	//If empty
	if((*List)==NULL)
	{
		return -1;
	}
	//If first node is target
	if((*List)->afm==afm)
	{
		precentileNode* temp = (*List);
		(*List)=(*List)->next;
		free(temp);
		return 0;
	}
	//Find target
	precentileNode* Pointer=(*List);
	while(Pointer->next!=NULL&&Pointer->next->afm!=afm)
	{
		Pointer=Pointer->next;
	}
	//If not found
	if(Pointer->next==NULL)
	{
		return -1;
	}
	//If found
	precentileNode* temp = Pointer->next;
	Pointer->next=Pointer->next->next;
	free(temp);
	return 0;
}
precentileNode* PRE_Get(precentileNode** List, long long afm)
{
	//If empty
	if((*List)==NULL)
	{
		return NULL;
	}
	//If first node is target
	if((*List)->afm==afm)
	{
		return (*List);
	}
	//Find target
	precentileNode* Pointer=(*List);
	while(Pointer->next!=NULL&&Pointer->next->afm!=afm)
	{
		Pointer=Pointer->next;
	}
	//Target found (NULL counts)
	return Pointer->next;
}
void PRE_Free(precentileNode** List)
{
	precentileNode* temp;
	precentileNode* Pointer = (*List);
	while(Pointer!=NULL)
	{
		temp=Pointer;
		Pointer=Pointer->next;
		free(temp);
	}
}

int NOM_Insert(nomosNode** List, char* nomos, double debt)
{
	//If empty
	if((*List)==NULL)
	{
		(*List)=malloc(sizeof(nomosNode));
		if((*List)==NULL)
		{
			return -1;
		}
		(*List)->next=NULL;
		memcpy((*List)->nomos,nomos,sizeof((*List)->nomos));
		(*List)->people=1;
		(*List)->totalDebt=debt;
		return 0;
	}
	//If first is target
	if(!strncmp((*List)->nomos,nomos,3))
	{
		(*List)->people=(*List)->people+1;
		(*List)->totalDebt=(*List)->totalDebt+debt;
		return 0;
	}
	//Find target
	nomosNode* Pointer = (*List);
	while(Pointer->next!=NULL&&strncmp(Pointer->next->nomos,nomos,3))
	{
		Pointer=Pointer->next;
	}
	//If target not found, create
	if(Pointer->next==NULL)
	{
		Pointer->next=malloc(sizeof(nomosNode));
		Pointer=Pointer->next;
		if(Pointer==NULL)
		{
			return -1;
		}
		Pointer->next=NULL;
		memcpy(Pointer->nomos,nomos,sizeof(Pointer->nomos));
		Pointer->people=1;
		Pointer->totalDebt=debt;
		return 0;
	}
	//If target found
	Pointer=Pointer->next;
	Pointer->people=Pointer->people+1;
	Pointer->totalDebt=Pointer->totalDebt+debt;
	return 0;
}
int NOM_Remove(nomosNode** List, char* nomos, double debt)
{
	//If empty
	if((*List)==NULL)
	{
		return -1;
	}
	//If first is target
	if(!strncmp((*List)->nomos,nomos,3))
	{
		(*List)->people=(*List)->people-1;
		(*List)->totalDebt=(*List)->totalDebt-debt;
		return 0;
	}
	//Find target
	nomosNode* Pointer = (*List);
	while(Pointer->next!=NULL&&strncmp(Pointer->next->nomos,nomos,3))
	{
		Pointer=Pointer->next;
	}
	//If target not found, error
	if(Pointer->next==NULL)
	{
		return -1;
	}
	//If target found
	Pointer=Pointer->next;
	Pointer->people=Pointer->people-1;
	Pointer->totalDebt=Pointer->totalDebt-debt;
	return 0;
}
nomosNode* NOM_Get(nomosNode** List, char* nomos)
{
	//If empty
	if((*List)==NULL)
	{
		return NULL;
	}
	//If first node is target
	if(!strncmp((*List)->nomos,nomos,3))
	{
		return (*List);
	}
	//Find target
	nomosNode* Pointer = (*List);
	while(Pointer->next!=NULL&&strncmp(Pointer->next->nomos,nomos,3))
	{
		Pointer=Pointer->next;
	}
	//Target found (NULL counts)
	return Pointer->next;
}
void NOM_Free(nomosNode** List)
{
	nomosNode* temp;
	nomosNode* Pointer = (*List);
	while(Pointer!=NULL)
	{
		temp=Pointer;
		Pointer=Pointer->next;
		free(temp);
	}
}

int OTH_Insert(otherNode** List, long long afm, char* lastname, char* firstname, char* town, char* nomos, double debt)
{
	if((*List)==NULL)
	{
		(*List)=malloc(sizeof(otherNode));
		if((*List)==NULL)
		{
			return -1;
		}
		(*List)->next=NULL;
		(*List)->afm=afm;
		(*List)->debt=debt;
		memcpy((*List)->firstname,firstname,sizeof((*List)->firstname));
		memcpy((*List)->lastname,lastname,sizeof((*List)->lastname));
		memcpy((*List)->nomos,nomos,sizeof((*List)->nomos));
		memcpy((*List)->town,town,sizeof((*List)->town));
		return 0;
	}
	otherNode * Pointer = (*List);
	while(Pointer->next!=NULL)
	{
		Pointer=Pointer->next;
	}
	Pointer->next=malloc(sizeof(otherNode));
	if(Pointer->next==NULL)
	{
		return -1;
	}
	Pointer=Pointer->next;
	Pointer->next=NULL;
	Pointer->afm=afm;
	Pointer->debt=debt;
	memcpy(Pointer->firstname,firstname,sizeof(Pointer->firstname));
	memcpy(Pointer->lastname,lastname,sizeof(Pointer->lastname));
	memcpy(Pointer->nomos,nomos,sizeof(Pointer->nomos));
	memcpy(Pointer->town,town,sizeof(Pointer->town));
	return 0;
}
int OTH_Remove(otherNode** List, long long afm)
{
	otherNode* temp;
	//If empty
	if((*List)==NULL)
	{
		return -1;
	}
	//If first is target
	if((*List)->afm==afm)
	{
		temp=(*List);
		(*List)=(*List)->next;
		free(temp);
		return 0;
	}
	//Find target
	otherNode* Pointer = (*List);
	while(Pointer->next!=NULL&&Pointer->next->afm!=afm)
	{
		Pointer=Pointer->next;
	}
	//If target not found, error
	if(Pointer->next==NULL)
	{
		return -1;
	}
	//If target found
	temp=Pointer->next;
	Pointer->next=Pointer->next->next;
	free(temp);
	return 0;
}
otherNode* OTH_Get(otherNode** List, long long afm)
{
	//If empty
	if((*List)==NULL)
	{
		return NULL;
	}
	//If first node is target
	if((*List)->afm==afm)
	{
		return (*List);
	}
	//Find target
	otherNode* Pointer = (*List);
	while(Pointer->next!=NULL&&Pointer->next->afm!=afm)
	{
		Pointer=Pointer->next;
	}
	//Target found (NULL counts)
	return Pointer->next;
}
void OTH_Free(otherNode** List)
{
	otherNode* temp;
	otherNode* Pointer = (*List);
	while(Pointer!=NULL)
	{
		temp=Pointer;
		Pointer=Pointer->next;
		free(temp);
	}
}
void OTH_PrintData(otherNode** List, FILE* stream, long long afm)
{
	otherNode* Pointer=*List;
	while(Pointer!=NULL&&Pointer->afm!=afm)
	{
		Pointer=Pointer->next;
	}
	if(Pointer==NULL)
	{
		fprintf(stream,"AFM %lld does not exist.",afm);
		return;
	}
	fprintf(stream,"AFM: %.10lld\nDebt: %.2lf\nFirst Name: %.15s\nLast Name: %.25s\nTown: %.20s\nNomos: %.3s\n",Pointer->afm,Pointer->debt,Pointer->firstname,Pointer->lastname,Pointer->town,Pointer->nomos);
}