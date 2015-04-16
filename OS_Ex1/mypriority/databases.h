#ifndef __MYDATABASES__
#define __MYDATABASES__

#include <stdio.h>

typedef struct precentileNode {
	double debt;
	long long afm;
	struct precentileNode* next;
} precentileNode;

typedef struct nomosNode {
	char nomos[3];
	double totalDebt;
	long people;
	struct nomosNode* next;
} nomosNode;

typedef struct otherNode {
	long long afm;
	char lastname[25];
	char firstname[15];
	char town[20];
	char nomos[3];
	double debt;
	struct otherNode* next;
} otherNode;

int PRE_Insert(precentileNode** List, double debt, long long afm);
int PRE_Remove(precentileNode** List, long long afm);
precentileNode* PRE_Get(precentileNode** List, long long afm);
void PRE_Free(precentileNode** List);

int NOM_Insert(nomosNode** List, char* nomos, double debt);
int NOM_Remove(nomosNode** List, char* nomos, double debt);
nomosNode* NOM_Get(nomosNode** List, char* nomos);
void NOM_Free(nomosNode** List);

int OTH_Insert(otherNode** List, long long afm, char* lastname, char* firstname, char* town, char* nomos, double debt);
int OTH_Remove(otherNode** List, long long afm);
otherNode* OTH_Get(otherNode** List, long long afm);
void OTH_Free(otherNode** List);
void OTH_PrintData(otherNode** List, FILE* stream, long long afm);

#endif