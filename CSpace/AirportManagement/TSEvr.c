#include "TSEvr.h"
#include <string.h>

int TSEvr_setKeyValue (keyType *target, keyType source){
	int i;
	for (i=0;i<4-1;i++)
	{
		(*target)[i]=source[i];
	}
	(*target)[i]=source[i];
	if (source[i]!='\0')
	{
		return -1;
	}
	return 0;
}
int TSEvr_readKeyValue (FILE *from,  keyType *Elem){
	int i;
	int c;
	for (i=0;i<4-1;i++)
	{
		c=getc(from);
		if (!((c>='A'&&c<='Z')||(c>='0'&&c<='9')))
		{
			return 1;
		}
		(*Elem)[i]=c;
	}
	(*Elem)[i]='\0';
	c=getc(from);
	ungetc(c,from);
	if ((c>='A'&&c<='Z')||(c>='0'&&c<='9'))//Check for key length
	{
		return 1;
	}
	return 0;
}
int TSEvr_writeKeyValue(FILE *to, keyType Elem){
	int i;
	for (i=0;i<4-1;i++)
	{
		if(fputc(Elem[i],to)!=Elem[i])
		{
			return 1;
		}
	}
	if (Elem[i]!='\0')
	{
		fputc(Elem[i],to);
		return -1;
	}
	return 0;
}


int TSEvr_setValue (TStoixeiouOther *target, TStoixeiouOther source){
	int i;
	for (i=0;i<5-1;i++)
	{
		(target->ICAO)[i]=(source.ICAO[i]);
	}
	(target->ICAO)[i]=(source.ICAO[i]);
	strcpy(target->country,source.country);
	if (source.ICAO[i]!='\0')
	{
		return -1;
	}
	return 0;
}
int TSEvr_readValue (FILE *from,  TStoixeiouOther *Elem){
	int i;
	int c;
	for (i=0;i<5-1;i++)
	{
		c=getc(from);
		if (!((c>='A'&&c<='Z')||(c>='0'&&c<='9')))
		{
			return 1;
		}
		(Elem->ICAO)[i]=c;
	}
	(Elem->ICAO)[i]='\0';
	c=getc(from);
	ungetc(c,from);
	if ((c>='A'&&c<='Z')||(c>='0'&&c<='9'))//Check for code length
	{
		return 1;
	}
	return 0;
}
int TSEvr_writeValue(FILE *to, TStoixeiouOther Elem){
	int i;
	if (fputs(Elem.country,to)<0)
	{
		return 1;
	}
	if(fputc('\t',to)!='\t')
	{
		return 1;
	}
	for (i=0;i<5-1;i++)
	{
		if(fputc((Elem.ICAO)[i],to)!=(Elem.ICAO)[i])
		{
			return 1;
		}
	}
	if ((Elem.ICAO)[i]!='\0')
	{
		fputc((Elem.ICAO)[i],to);
		return -1;
	}
	return 0;
}
