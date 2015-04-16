/* Each Element is composed of three parts: the date, the duration and the phone number.
 * Date and Phone number are strings with length DATESIZE and NUMBERSIZE respectively.
 * Duration is an integer.
 */

#ifndef __TStoixeiouOuraPlus_H__
#define __TStoixeiouOuraPlus_H__

#include <stdlib.h>
#include <stdio.h>

#define DATESIZE 14 //The number of characters necessary to depict the date
#define NUMBERSIZE 14 //The number of characters necessary to depict the phone number

typedef struct GeneralElement *TStoixeiouOuraPlus;

void TSOuraPlus_create (TStoixeiouOuraPlus* Elem);
/* Allocates memory for <Elem> setting al its values to be either blank or zero.
 */
void TSOuraPlus_destroy (TStoixeiouOuraPlus* Elem);
/* Destroyes <Elem> freeing up any memory it might have used.
 * It also sets the pointer to NULL to avoid problems with dangling pointers
 * and double frees.
 * This function does nothing if given a NULL pointer.
 */
int TSOuraPlus_setValue (TStoixeiouOuraPlus target, TStoixeiouOuraPlus source);
/* Sets <target> to the same values as <source>
 */
int TSOuraPlus_setValue_number (TStoixeiouOuraPlus target, TStoixeiouOuraPlus source);
/* Set the phone number of <target> to the phone number of <source>, leaving the rest unchanged.
 */
int TSOuraPlus_readValue (FILE *from,  TStoixeiouOuraPlus Elem);
/* Sets the value of <Elem> according to data gained from stream <from>.
 * Specificaly this function expects date, duration and number parameters
 * in that order. Date and number can be any string smaller than DATESIZE
 * and NUMBERSIZE respectively. Duration must be an integer.
 */
int TSOuraPlus_readValue_number (FILE *from, TStoixeiouOuraPlus Elem);
/* Reads just the phone number, leaving the rest blank.
 */
int TSOuraPlus_readValue_date (FILE *from, TStoixeiouOuraPlus Elem);
/* Reads just the date, leaving the rest blank.
 */
int TSOuraPlus_writeValue(FILE *to, TStoixeiouOuraPlus Elem);
/* Writes the date, duration and phone number of <Elem> in stream <to>.
 */
int TSOuraPlus_writeValue_date(FILE *to, TStoixeiouOuraPlus Elem);
/* Writes the date of <Elem> in stream <to>.
 */
int TSOuraPlus_iso(TStoixeiouOuraPlus Elem1, TStoixeiouOuraPlus Elem2);
/* Returns 1 if the dates of the elements are equal, else returns 0.
 */
int TSOuraPlus_isoNum(TStoixeiouOuraPlus Elem1, TStoixeiouOuraPlus Elem2);
/* Returns 1 if the numbers of the elements are equal, else returns 0.
 */
int TSOuraPlus_isoAll(TStoixeiouOuraPlus Elem1, TStoixeiouOuraPlus Elem2);
/* Returns 1 if the elements are equal, else returns 0.
 */

#endif
