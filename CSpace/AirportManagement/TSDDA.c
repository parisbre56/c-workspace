#include "TSDDA.h"
#include <string.h>

int TSDDA_setValue (TStoixeiouDDA *target, TStoixeiouDDA source){
	target->arrayIndex=source.arrayIndex;
	return TSEvr_setKeyValue(&(target->key),source.key);
}
int TSDDA_readValue (FILE *from,  TStoixeiouDDA *Elem){
	return TSEvr_readKeyValue(from,&(Elem->key));
}
int TSDDA_writeValue(FILE *to, TStoixeiouDDA Elem){
	return TSEvr_writeKeyValue(to,Elem.key);
}


int TSDDA_iso(TStoixeiouDDA s1, TStoixeiouDDA s2){
	return (strcmp(s1.key,s2.key)==0);
}

int TSDDA_mikrotero(TStoixeiouDDA s1, TStoixeiouDDA s2){
	return (strcmp(s1.key,s2.key)>0);
}

int TSDDA_megalytero(TStoixeiouDDA s1, TStoixeiouDDA s2){
	return (strcmp(s1.key,s2.key)<0);
}

/* The Following are already Implemented using Above */

int TSDDA_mikrotero_iso(TStoixeiouDDA s1, TStoixeiouDDA s2)
{ 
    return TSDDA_mikrotero(s1,s2)||TSDDA_iso(s1,s2);
}

int TSDDA_megalytero_iso(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
    return TSDDA_megalytero(s1,s2)||TSDDA_iso(s1,s2);
}

int TSDDA_diaforo(TStoixeiouDDA s1, TStoixeiouDDA s2)
{
    return !TSDDA_iso(s1,s2);
}
