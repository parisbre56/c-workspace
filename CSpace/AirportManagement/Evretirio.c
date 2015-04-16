#include "Evretirio.h"
#define Simple 0 /* 0 for AVL - any other integer for Simple  */

#if (Simple)
   #include "BST/ch8_BSTpointer.h" /* gia Simple Ylopoihsh */
#else
   #include "AVL/ch8_AVLpointer.h" /* gia AVL Ylopoihsh */
#endif


/* Oi diafores tvn ylopoihsevn Simple kai AVL einai mikres - mporeite na xrhsimopoihsete 
   thn parapanv domh #if ... #else ...#endif gia na diaforopihsete tis dyo ylopoihseis  */

typedef struct EvrNode{
       TStoixeiouOther *DataArray; /* array of size MaxSize */
       int LastElem;              /* index of last element in array */
       typos_deikti TreeRoot;     /* Root of Tree */
} EvrNode;

int PrivatePrint(EvrPtr E,typos_deikti CurRoot, FILE* out);

EvrPtr Evr_dimiourgia(int MaxSize){
	EvrPtr tempptr;
	if((tempptr=malloc(sizeof(EvrNode)))==NULL)
	{
		return NULL;
	}
	if((tempptr->DataArray=malloc(sizeof(TStoixeiouOther)*MaxSize))==NULL)
	{
		free(tempptr);
		return NULL;
	}
	#if (Simple)
		Tree_dimiourgia(&(tempptr->TreeRoot));
	#else
		AVLTree_dimiourgia(&(tempptr->TreeRoot));
	#endif
	tempptr->LastElem=0;
	return tempptr;
}

int Evr_Insert(EvrPtr E, keyType key, TStoixeiouOther Data){
	int error=0;
	#if !(Simple)
		int unused=0;
	#endif
	TStoixeiouDDA temp;
	temp.arrayIndex=E->LastElem;
	if(TSEvr_setKeyValue(&(temp.key),key))
	{
		return 1;
	}
	if(TSEvr_setValue(&((E->DataArray)[E->LastElem]),Data))
	{
		return 1;
	}
	E->LastElem=(E->LastElem)+1;
	#if (Simple)
		Tree_eisagogi(&(E->TreeRoot),temp,&error);
	#else
		AVLTree_eisagogi(&(E->TreeRoot),temp,&unused,&error);
	#endif
	return error;
}

int Evr_ChangeData(EvrPtr E, keyType key, TStoixeiouOther Data){
	int found;
	TStoixeiouDDA temp;
	typos_deikti foundpos;
	if(TSEvr_setKeyValue(&(temp.key),key))
	{
		return 1;
	}
	#if (Simple)
		Tree_anazitisi(E->TreeRoot,temp,&foundpos,&found);
	#else
		AVLTree_anazitisi(E->TreeRoot,temp,&foundpos,&found);
	#endif
	if (!found)
	{
		return -1;
	}
	#if (Simple)
		Tree_periexomeno(foundpos,&temp);
	#else
		AVLTree_periexomeno(foundpos,&temp);
	#endif
	if(TSEvr_setValue(&((E->DataArray)[temp.arrayIndex]),Data))
	{
		return 1;
	}
	return 0;
}

int Evr_PrintAll(EvrPtr E, FILE *out){
	return PrivatePrint(E,E->TreeRoot,out);
}

int Evr_katastrofi(EvrPtr *E){
	#if (Simple)
		Tree_katastrofi(&((*E)->TreeRoot));
	#else
		AVLTree_katastrofi(&((*E)->TreeRoot));
	#endif
	free((*E)->DataArray);
	free(*E);
	*E=NULL;
	return 0;
}

int PrivatePrint(EvrPtr E, typos_deikti CurRoot, FILE* out)
{
	/* There is a problem with the two dpaidi functions. AVLTree_dpaidi returns error==1 if
	 * (father==NULL || rightchild==NULL) while Tree_dpaidi returns error==1 only if father==NULL.
	 * As such it is better to check if the returned child is equal to NULL.
	 */
	int error;
	TStoixeiouDDA temp;
	typos_deikti temproot;
	#if (Simple)
		Tree_dpaidi(CurRoot,&temproot,&error);
	#else
		AVLTree_dpaidi(CurRoot,&temproot,&error);
	#endif
	if (temproot!=NULL)
	{
		if(PrivatePrint(E,temproot,out))
		{
			return 1;
		}
	}
	#if (Simple)
		Tree_periexomeno(CurRoot,&temp);
	#else
		AVLTree_periexomeno(CurRoot,&temp);
	#endif
	if(TSEvr_writeKeyValue(out,temp.key))
	{
		return 1;
	}
	putc(' ',out);
	if (TSEvr_writeValue(out,(E->DataArray)[temp.arrayIndex]))
	{
		return 1;
	}
	putc('\n',out);
	#if (Simple)
		Tree_apaidi(CurRoot,&temproot,&error);
	#else
		AVLTree_apaidi(CurRoot,&temproot,&error);
	#endif
	if (temproot!=NULL)
	{
		if(PrivatePrint(E,temproot,out))
		{
			return 1;
		}
	}
	return 0;
}