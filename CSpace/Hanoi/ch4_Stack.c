/***************************************************************************
Arxeio ylopoihshs	: ch4_Stack.c
Syggrafeas			: N. Misyrlhs , Y. Cotronis
Skopos				: Ylopoihsh me pinaka, ATD Stoiva me Merikh Apokrypsh
Ana8ewrhsh          : X. Georgakopoylos, S. Gkiokas
****************************************************************************/


#include <stdio.h>
#include "ch4_Stack.h"


typedef struct TStoivasStruct {
	int korifi;
	TStoixeioyStoivasPt pinakas[PLITHOS];
} TStoivas;

/*orismos synarthsewn*/

TStoivasPt Stoiva_dimiourgia(TStoivasPt *stoiva)
{
 /*
 *	Pro: 		Kamia
 *	Meta: 		Dhmioyrgia kenhs stoivas
*/
	int i;
	int j;
	*stoiva=malloc(sizeof(TStoivas));
	if (*stoiva==NULL)
	{
		return NULL;
	}
	for (i=0;i<PLITHOS;i++)
	{
		if (TSstoiva_create(&((*stoiva)->pinakas[i]))==NULL)
		{
			for (j=i-1;j>-1;j--)
			{
				TSstoiva_destroy(&((*stoiva)->pinakas[j]));
			}
			free(*stoiva);
			*stoiva=NULL;
			return NULL;
		}
	}
	(*stoiva)->korifi = -1;
	return *stoiva;
}

void Stoiva_katastrofi(TStoivasPt* stoiva)
{
	int i;
	if (*stoiva==NULL)
	{
		return;
	}
	for (i=0;i<PLITHOS;i++)
	{
		TSstoiva_destroy(&((*stoiva)->pinakas[i]));		
	}
	free(*stoiva);
	*stoiva=NULL;
}

int  Stoiva_keni(TStoivasPt stoiva)
{
 /*
 *	Pro: 	    Dhmioyrgia ths stoiva
 *	Meta: 		Epistrefei 1 an h stoiva einai kenh alliws 0
*/
	return (stoiva->korifi == -1);
}

/*void Stoiva_exagogi(TStoivasPt stoiva,TStoixeioyStoivasPt stoixeio,int *ypoxeilisi)
{

//	Pro: 		Mh kenh stoiva
//	Meta: 		Eksagetai to stoixeio apo th stoiva

	if (Stoiva_keni(stoiva))
		*ypoxeilisi = 1;
	else
	{
		*ypoxeilisi = 0;
		TSstoiva_setValues(stoixeio, stoiva->pinakas[stoiva->korifi]);
//debug
//		printf("E|"); TSstoiva_writeValue(stoixeio); printf("|E\n");
//		printf("E|"); TSstoiva_writeValue(stoiva->pinakas[stoiva->korifi]); printf("|E\n");

		stoiva->korifi--;
	}
}*/

void Stoiva_PopValue(TStoivasPt stoiva, TStoixeioyStoivasPt stoixeio, int *ypoxeilisi)
{
	if (Stoiva_keni(stoiva))
	{
		*ypoxeilisi = 1;
		return;
	}
	*ypoxeilisi = 0;
	TSstoiva_setValues(stoixeio, stoiva->pinakas[stoiva->korifi]);
}
void Stoiva_PopState(TStoivasPt stoiva, int *ypoxeilisi)
{
	if (Stoiva_keni(stoiva))
	{
		*ypoxeilisi = 1;
		return;
	}
	*ypoxeilisi = 0;
	stoiva->korifi--;
}

void Stoiva_othisi(TStoivasPt stoiva,TStoixeioyStoivasPt stoixeio,int *yperxeilisi)
{
 /*
 *	Pro: 		Dhmioyrgia ths stoiva
 *	Meta: 		Wtheitai to stoixeio sth stoiva
*/
	if (stoiva->korifi == PLITHOS -1)
		*yperxeilisi = 1;
	else
	{
		*yperxeilisi = 0;
		stoiva->korifi++;
		TSstoiva_setValues(stoiva->pinakas[stoiva->korifi], stoixeio);
/* debug
		printf("O|"); TSstoiva_writeValue(stoixeio); printf("|O\n");
		printf("O|"); TSstoiva_writeValue(stoiva->pinakas[stoiva->korifi]); printf("|O\n");
*/
	}
}

