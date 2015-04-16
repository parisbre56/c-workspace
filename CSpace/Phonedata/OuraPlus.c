#include "TStoixeiouOuraPlus.h"
#include "OuraPlus.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct typos_komvou *typos_deikti;

typedef struct typos_komvou{
	TStoixeiouOuraPlus dedomena;
	typos_deikti epomenos;
	typos_deikti proigoumenos;
} typos_komvou;

typedef struct GeneralNode{
	typos_deikti Head;
	typos_deikti Last;
	typos_deikti Current;
	int ListSize;
} GeneralNode;

void OuraPlus_dimiourgia(TOuraPlus * const ouraP)
{
	*ouraP=malloc(sizeof(GeneralNode));
	if (*ouraP==NULL)
	{
		return;
	}
	(*ouraP)->Current=NULL;
	(*ouraP)->Head=NULL;
	(*ouraP)->Last=NULL;
	(*ouraP)->ListSize=0;
}

void OuraPlus_katastrofi(TOuraPlus *const ouraP)
{
	if(*ouraP==NULL)
	{
		return;
	}
	while(OuraPlus_megethos(*ouraP))
	{
		OuraPlus_exagogiArxh(*ouraP);
	}
	free(*ouraP);
	*ouraP=NULL;
}

int	OuraPlus_keni(const TOuraPlus ouraP)
{
	return ouraP->ListSize==0;
}

int OuraPlus_megethos(const TOuraPlus ouraP)
{
	return ouraP->ListSize;
}

int OuraPlus_periexomenoTrexon(const TOuraPlus ouraP, TStoixeiouOuraPlus * const value)
{
	if (ouraP==NULL||ouraP->Current==NULL)
	{
		return 1;
	}
	return TSOuraPlus_setValue (*value, (ouraP->Current->dedomena));
}

int OuraPlus_allagiTrexon(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio) /* error if Current invalid */
{
	if (ouraP->Current==NULL||stoixeio==NULL)
	{
		return 1;
	}
	return TSOuraPlus_setValue(ouraP->Current->dedomena,stoixeio);
}

int OuraPlus_allagiTrexon_num(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio) /* error if Current invalid */
{
	if (ouraP->Current==NULL||stoixeio==NULL)
	{
		return 1;
	}
	return TSOuraPlus_setValue_number(ouraP->Current->dedomena,stoixeio);
}

int OuraPlus_eisagogiKeni(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio)
/* Private function used only for inserting the first element.
 * Used for convenience.
 */
{
	if (stoixeio==NULL||!OuraPlus_keni(ouraP))
	{
		return 1;
	}
	if((ouraP->Head=malloc(sizeof(typos_komvou)))==NULL)
	{
		return 1;
	}
	TSOuraPlus_create(&(ouraP->Head->dedomena));
	if(ouraP->Head->dedomena==NULL)
	{
		free(ouraP->Head);
		return 1;
	}
	(ouraP->ListSize)++;
	ouraP->Head->epomenos=ouraP->Head->proigoumenos=ouraP->Head;
	ouraP->Last=ouraP->Head;
	return TSOuraPlus_setValue(ouraP->Head->dedomena,stoixeio);
}

int OuraPlus_eisagogiTelos(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio) /* panta sto telos */
{
	if (stoixeio==NULL)
	{
		return 1;
	}
	if (OuraPlus_keni(ouraP))
	{
		return OuraPlus_eisagogiKeni(ouraP,stoixeio);
	}
	if ((ouraP->Last->epomenos=malloc(sizeof(typos_komvou)))==NULL)
	{
		return 1;
	}
	TSOuraPlus_create(&(ouraP->Last->epomenos->dedomena));	
	if(ouraP->Last->epomenos->dedomena==NULL)
	{
		free(ouraP->Last->epomenos);
		return 1;
	}
	ouraP->Last->epomenos->proigoumenos=ouraP->Last;
	ouraP->Head->proigoumenos=ouraP->Last=ouraP->Last->epomenos;
	ouraP->Last->epomenos=ouraP->Head;
	(ouraP->ListSize)++;
	return TSOuraPlus_setValue(ouraP->Last->dedomena,stoixeio);
}

int OuraPlus_exagogiLastOne(const TOuraPlus ouraP)
/* Private function used only for deleting the last element.
 * Used for convenience.
 */
{
	if (OuraPlus_megethos(ouraP)!=1)
	{
		return 1;
	}
	TSOuraPlus_destroy(&(ouraP->Head->dedomena));
	free(ouraP->Head);
	ouraP->Current==NULL;
	ouraP->Head==NULL;
	ouraP->Last==NULL;
	ouraP->ListSize=0;
	return 0;
}

int OuraPlus_exagogiArxh(const TOuraPlus ouraP) /*  panta apo thn arxh  */ 
{
	if (OuraPlus_keni(ouraP))
	{
		return 1;
	}
	if (ouraP->Current==ouraP->Head)
	{
		ouraP->Current=NULL;
	}
	if (OuraPlus_megethos(ouraP)==1)
	{
		return OuraPlus_exagogiLastOne(ouraP);
	}
	TSOuraPlus_destroy(&(ouraP->Head->dedomena));
	ouraP->Head=ouraP->Head->epomenos;
	free(ouraP->Head->proigoumenos);
	ouraP->Head->proigoumenos=ouraP->Last;
	ouraP->Last->epomenos=ouraP->Head;
	(ouraP->ListSize)--;
	return 0;
}

int OuraPlus_diagrafiTrexon(const TOuraPlus ouraP) /* apo apoydhpote (Current) */
{
	if (OuraPlus_keni(ouraP)||ouraP->Current==NULL)
	{
		return 1;
	}
	if (ouraP->Current==ouraP->Head)
	{
		return OuraPlus_exagogiArxh(ouraP); //Also covers the case <ouraP> -> ListSize == 1
	}
	if (ouraP->Current==ouraP->Last)
	{
		ouraP->Last=ouraP->Last->proigoumenos;
	}
	TSOuraPlus_destroy(&(ouraP->Current->dedomena));
	ouraP->Current->proigoumenos->epomenos=ouraP->Current->epomenos;
	ouraP->Current->epomenos->proigoumenos=ouraP->Current->proigoumenos;
	free(ouraP->Current);
	ouraP->Current=NULL;
	(ouraP->ListSize)--;
	return 0;
}

int OuraPlus_epomenos(const TOuraPlus ouraP) /* if noNext no move + error */
{
	if (OuraPlus_keni(ouraP)||ouraP->Current==NULL)
	{
		return 1;
	}
	/*if (ouraP->Current->epomenos==ouraP->Current)
	{
		return 1;
	}*/
	ouraP->Current=ouraP->Current->epomenos;
	return 0;
}

int OuraPlus_proigoymenos(const TOuraPlus ouraP) /* if noPrev no move + error*/
{
	if (OuraPlus_keni(ouraP)||ouraP->Current==NULL)
	{
		return 1;
	}
	/*if (ouraP->Current->proigoumenos==ouraP->Current)
	{
		return 1;
	}*/
	ouraP->Current=ouraP->Current->proigoumenos;
	return 0;
}

int OuraPlus_prwtos(const TOuraPlus ouraP) /* to First - if empty error */
{
	if (OuraPlus_keni(ouraP))
	{
		return 1;
	}
	ouraP->Current=ouraP->Head;
	return 0;
}

int OuraPlus_teleytaios(const TOuraPlus ouraP) /* to Last - if empty to Current=Last + error */
{
	if (OuraPlus_keni(ouraP))
	{
		return 1;
	}
	ouraP->Current=ouraP->Last;
	return 0;
}

int OuraPlus_anazitisi(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio, int (*iso)(TStoixeiouOuraPlus , TStoixeiouOuraPlus))
{
	if (OuraPlus_keni(ouraP))
	{
		return 1;
	}
	if (ouraP->Current==ouraP->Head)
	{
		ouraP->Current=NULL;
		return 0;
	}
	if (ouraP->Current==NULL)
	{
		OuraPlus_prwtos(ouraP);
	}
	do{
		if ((*iso)(ouraP->Current->dedomena,stoixeio))
		{
			return 0;
		}
		OuraPlus_epomenos(ouraP);
	}while (ouraP->Current!=ouraP->Head);
	ouraP->Current=NULL;
	return 0;
}