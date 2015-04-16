#ifndef __TSDDA__
#define __TSDDA__

#include "TSEvr.h"

typedef struct KeyIndex{
        keyType key;
        int arrayIndex;
} TStoixeiouDDA;

int TSDDA_setValue (TStoixeiouDDA *target, TStoixeiouDDA source);
int TSDDA_readValue (FILE *from,  TStoixeiouDDA *Elem);
int TSDDA_writeValue(FILE *to, TStoixeiouDDA Elem);

/* basikes sygkriseis gia xrhsh sthn eisagcgh-diagarfh-anazhthsh se DDA */
int TSDDA_iso(TStoixeiouDDA, TStoixeiouDDA);
int TSDDA_mikrotero(TStoixeiouDDA, TStoixeiouDDA);
int TSDDA_megalytero(TStoixeiouDDA, TStoixeiouDDA);

/* symplhrvmatikes sygkriseis */
int TSDDA_mikrotero_iso(TStoixeiouDDA, TStoixeiouDDA);
int TSDDA_megalytero_iso(TStoixeiouDDA, TStoixeiouDDA);
int TSDDA_diaforo(TStoixeiouDDA, TStoixeiouDDA);
#endif
