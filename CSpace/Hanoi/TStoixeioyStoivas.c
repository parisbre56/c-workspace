/***************************************************************************
Arxeio ylopihshs	: TStoixeioyStoivas.h
Syggrafeas			: Y. Cotronis
Skopos				: Ylipoiei praxeis toy Typoy Stoixeioy ths Stoivas (int)
Ana8ewrhsh          : X. Georgakopoylos, S. Gkiokas
****************************************************************************/
#include "TStoixeioyStoivas.h"

typedef struct TStoixeioyStoivasStruct {
	int aktina;
	int ypsos;
} TStoixeioyStoivas;

/*orismos synarthsewn*/

TStoixeioyStoivasPt TSstoiva_create (TStoixeioyStoivasPt *Elem)
{
	return *Elem=malloc(sizeof(TStoixeioyStoivas));
}

void TSstoiva_destroy (TStoixeioyStoivasPt *Elem)
{
	if (*Elem==NULL)
	{
		return;
	}
	free(*Elem);
	*Elem=NULL;
}

void TSstoiva_setValues (TStoixeioyStoivasPt target, TStoixeioyStoivasPt source)
{
	*target=*source;
}

void TSstoiva_readValue (FILE *from, TStoixeioyStoivasPt ElemPtr)
{
     fscanf(from, "%d %d", &(ElemPtr->aktina), &(ElemPtr->ypsos));
}

void TSstoiva_writeValue (FILE *to, TStoixeioyStoivasPt Elem)
{
	fprintf(to, "%d\t%d", Elem->aktina, Elem->ypsos);
}

void TSstoiva_readFirstValue (FILE *from, TStoixeioyStoivasPt ElemPtr)
{
     fscanf(from, "%d", &(ElemPtr->aktina));
	 ElemPtr->ypsos=1;
}

void TSstoiva_writeFirstValue (FILE *to, TStoixeioyStoivasPt Elem)
{
	fprintf(to, "%d", Elem->aktina);
}

int mikrotero(TStoixeioyStoivasPt left, TStoixeioyStoivasPt right)
{
	return left->aktina*left->aktina*left->ypsos<right->aktina*right->aktina*right->ypsos;
	//To compute the actual volume you also need to multiply the above with pi (π=3.1415 etc.)
	//However that is unnecessary.
}
