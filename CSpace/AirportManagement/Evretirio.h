#ifndef __Evr__
#define __Evr__

#include <stdlib.h>
#include <stdio.h>
#include "TSEvr.h"

typedef struct EvrNode * EvrPtr;

EvrPtr Evr_dimiourgia(int MaxSize);
int Evr_katastrofi(EvrPtr *E);

int Evr_Insert(EvrPtr E, keyType key, TStoixeiouOther Data);
int Evr_ChangeData(EvrPtr E, keyType key, TStoixeiouOther Data);
int Evr_PrintAll(EvrPtr E, FILE *out);

#endif
