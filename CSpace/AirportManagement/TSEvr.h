#ifndef __TSEvr__
#define __TSEvr__

#include <stdio.h>

typedef char keyType[4]; //IATA
typedef struct data {
        char ICAO[5];
        char country[15];
} TStoixeiouOther;

int TSEvr_setKeyValue (keyType *target, keyType source);
int TSEvr_readKeyValue (FILE *from,  keyType *Elem);
int TSEvr_writeKeyValue(FILE *to, keyType Elem);

int TSEvr_setValue (TStoixeiouOther *target, TStoixeiouOther source);
int TSEvr_readValue (FILE *from,  TStoixeiouOther *Elem);
int TSEvr_writeValue(FILE *to, TStoixeiouOther Elem);

#endif
