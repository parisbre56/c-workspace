/***************************************************************************
Arxeio epikefalidas	: TStoixeioyStoivas.h
Syggrafeas			: Y. Cotronis
Skopos				: Orizei ton Typo Stoixeioy ths Stoivas (int)
Ana8ewrhsh          : X. Georgakopoylos, S. Gkiokas
****************************************************************************/
#ifndef __TStoixeioyStoivas_H__
#define __TStoixeioyStoivas_H__
#include <stdio.h>
#include <stdlib.h>

typedef struct TStoixeioyStoivasStruct *TStoixeioyStoivasPt;
/* Opaque pointer used to refer to TStoixeioyStoivas objects
 */

/*dhlwseis synarthsewn*/

TStoixeioyStoivasPt TSstoiva_create (TStoixeioyStoivasPt *Elem);
/* Creates a new TStoixeioyStoivas object, makes <Elem> point to it and returns a 
 * pointer to the object if successful or NULL if unsuccessful.
 */
void TSstoiva_destroy (TStoixeioyStoivasPt *Elem);
/* Destroys TStoixeioyStoivas object <Elem> and sets the pointer to NULL.
 * If the pointer is alredy NULL nothing happens.
 */
void TSstoiva_setValues (TStoixeioyStoivasPt target, TStoixeioyStoivasPt source);
/* Makes the value of <target> the same as the value of <source>.
 */
void TSstoiva_readValue (FILE *from,  TStoixeioyStoivasPt Elem);
/* Reads two integers from stream <from> and uses them to set the value of
 * <Elem>.
 */
void TSstoiva_writeValue (FILE *to, TStoixeioyStoivasPt Elem);
/* Writes two integers, generated from <Elem>, to stream <to>.
 */
void TSstoiva_readFirstValue (FILE *from,  TStoixeioyStoivasPt Elem);
/* Reads one integer from stream <from> and uses it to set the value of
 * <Elem>
 */
void TSstoiva_writeFirstValue (FILE *to, TStoixeioyStoivasPt Elem);
/* Writes one integer, generated from <Elem>, to stream <to>.
 */
int mikrotero(TStoixeioyStoivasPt left, TStoixeioyStoivasPt right);
/* Returns 1 if <left> is smaller than <right>, else returns 0.
 */
#endif
