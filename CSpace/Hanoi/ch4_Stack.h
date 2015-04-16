/***************************************************************************
Arxeio epikefalidas	: ch4_Stack.h
Syggrafeas			: Y. Cotronis
Skopos				: Ylopoihsh me pinaka, ATD Stoiva me Merikh Apokrypsh
Ana8ewrhsh          : X. Georgakopoylos, S. Gkiokas
****************************************************************************/

#ifndef __CH4_STACK__ 
#define __CH4_STACK__

/*dhlwseis typwn*/

#define PLITHOS 100 /*megisto epitrepto plh8os stoixeiwn ana stoiva*/
#include "TStoixeioyStoivas.h"
#include <stdlib.h>
typedef struct TStoivasStruct *TStoivasPt;
/* Opaque pointer used to refer to TStoivas objects.
 */

/*dhlwseis synarthsewn*/

TStoivasPt Stoiva_dimiourgia(TStoivasPt *stoiva);
/* Creates a new TStoivas object, makes <stoiva> point to it and returns a 
 * pointer to the object if successful or NULL if unsuccessful.
 */
void Stoiva_katastrofi(TStoivasPt *stoiva);
/* Destroys TStoivas object <stoiva> and sets the pointer to NULL.
 * If the pointer is alredy NULL nothing happens.
 */
int  Stoiva_keni(TStoivasPt stoiva);
/* Returns 1 if <stoiva> is empty, else returns 0.
 */
//void Stoiva_exagogi(TStoivas *stoiva,TStoixeioyStoivasPt stoixeio,int *ypoxeilisi);
/* The top object of stack <stoiva> is put to <stoixeio>. The top object is then removed
 * from the stack. If <stoiva> is empty <ypoxeilisi> is set to 1 and nothing else happens,
 * else it is set to 0.
 */
void Stoiva_PopValue(TStoivasPt stoiva, TStoixeioyStoivasPt stoixeio, int *ypoxeilisi);
/* The top object of stack <stoiva> is put to <stoixeio>. If <stoiva> is empty <ypoxeilisi>
 * is set to 1 and nothing else happens, else it is set to 0.
 */
void Stoiva_PopState(TStoivasPt stoiva, int *ypoxeilisi);
/* The top object of stack <stoiva> is removed from the stack. If <stoiva> is empty
 * <ypoxeilisi> is set to 1 and nothing else happens, else it is set to 0.
 */
void Stoiva_othisi(TStoivasPt stoiva,TStoixeioyStoivasPt stoixeio,int *yperxeilisi);
/* <stoixeio> is pushed to the top of stack <stoiva>. If <stoiva> is full <yperxeilisi> is 
 * set to 1 and nothing else happens, else it is set to 0.
 */

#endif /*#ifndef __CH4_STACK__ */
