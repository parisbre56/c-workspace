/***************************************************************************************
Arxeio Epikefalidas	: OuraPlus.h
Syggrafeas			: Y. Cotronis
Skopos			    : 
  O typos ATD OuraPlus symperiferetai ws oura (3 basikes praxeis): 
     a) stoixeia eisagontai sto telos
     b) stoixeia exagontai apo thn arxh
     c) elegxoume gia kenh
   Epipleon omws mporoume
     d) na kini9oume se olh thn oura (prwtos, teleytaios, epomenos, proigoumenos)
     e) na doume kai na allaxoume to periexomeno tou trexontos stoixeiou
     f) na diagrapsoume ton trexonta kombo
     g) na anazhthsoume stoixeio me kapoio krithrio isothtas 
     
   Ylopoieitai me Olikh apokrypsi 
     h) dimiourgia kai katastrofi
     
Ana8ewrhsh:             
****************************************************************************************/

/* Each Oura has the following features:
 * Each element of Oura is linked to both it's previous and its next element.
 * The first element is connected to the last and vice versa.
 * There is a pointer to the first and last element.
 * There is a pointer to the current element.
 * There is a counter for the number of elements present.
 * Each element of Oura contains data of the type described in TStoixeiouOuraPlus.h
 * The current element must be initialised manually.
 */
 
#ifndef __OuraPlus__ 
#define __OuraPlus__
#include "TStoixeiouOuraPlus.h"

/*dhlwseis typou*/
typedef struct GeneralNode *TOuraPlus;

/*dhlwseis synarthsewn*/

/*dhmioyrgia kai katastrofh ouraPs*/
void OuraPlus_dimiourgia(TOuraPlus * const ouraP);

void OuraPlus_katastrofi(TOuraPlus *const ouraP);

/*prakseis elegxoy*/
int	OuraPlus_keni(const TOuraPlus ouraP);

int OuraPlus_megethos(const TOuraPlus ouraP);

/*prakseis prosbashs-allazei to Current*/
int OuraPlus_periexomenoTrexon(const TOuraPlus ouraP, TStoixeiouOuraPlus * const value); /* error if Current invalid */

int OuraPlus_allagiTrexon(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio); /* error if Current invalid */

int OuraPlus_allagiTrexon_num(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio); 
/* Only changes the number of the current element of <ouraP> with the number of <stoixeio>.
 */

/*prakseis eisagwgh-diagrafh Current */
int OuraPlus_eisagogiTelos(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio); /* panta sto telos */

int OuraPlus_exagogiArxh(const TOuraPlus ouraP); /*  panta apo thn arxh  */ 
/*If the current pointer points to the beginning then the current element is set to NULL
 */
int OuraPlus_diagrafiTrexon(const TOuraPlus ouraP); /* apo apoydhpote (Current) */
/*When the current element is deleted the current pointer is set to NULL.
 */
 
/* Epilogh Kombou Current */
int OuraPlus_epomenos(const TOuraPlus ouraP); /* if noNext no move + error */

int OuraPlus_proigoymenos(const TOuraPlus ouraP); /* if noPrev no move + error*/

int OuraPlus_prwtos(const TOuraPlus ouraP); /* to First - if empty error */

int OuraPlus_teleytaios(const TOuraPlus ouraP); /* to Last - if empty to Current=Last + error */

/*prakseis anazhthshs*/
int OuraPlus_anazitisi(const TOuraPlus ouraP, TStoixeiouOuraPlus stoixeio, int (*iso)(TStoixeiouOuraPlus , TStoixeiouOuraPlus));
/* This function begins searching from the current element and keeps moving the current pointer foreword 
 * until it reaches the end of the list or it finds an element that when given with <stoixeio> to function <iso>
 * returns 1. If the end is reached then the current pointer is set to NULL.
 * To begin searching from the start use OuraPlus_prwtos and then use OuraPlus_anazitisi twice (because if started
 * at the head position the current pointer will become NULL; if started with a NULL current pointer it will begin 
 * searching from the start).
 * Remember to use OuraPlus_epomenos before looking for the next matching element, if the current element has been 
 * left unchanged. If not used, the same element will be found.
 */
#endif /*#ifndef __GenList__ */
