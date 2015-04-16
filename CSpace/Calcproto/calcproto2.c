#include <stdio.h>       			     	
#include <math.h>  
#include <stdbool.h>

double getnextvalue(void);
bool isEndofvalue(int,int);
bool firsthaspriority(int,int);
int ch;

int main(void)
{ 
	short int i;
	double values[3];
	for (i=0;i<3;i++)
		values[i]=0.0;
	int op[3];
	for (i=0;i<3;i++)
		op[i]=0;
	i=0;
	
	printf("Calculator prototype.\nWrite what you want to compute and press enter\nUse the end of file character");
	#ifdef __unix__ /* __unix__ is usually defined by compilers targeting Unix systems */
	printf("(CTRL-D)");
	#elif defined _WIN32 /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */
	printf("(CTRL-Z)");
	#endif
	printf(" to exit.\n");
	ch=getchar();
	while (ch!=EOF)
	{
		values[i]=getnextvalue();
		op[i]=ch;
		i++;
		while (ch!='\n')
		{
			values[i]=getnextvalue();
			if (i==2)
			{
				if (firsthaspriority(op[0],op[1]))
				{
					switch (op[1])
					{
						case '+':
						values[0]=values[0]+values[1];
						break;
						
						case '-':
						values[0]=values[0]-values[1];
						break;
						
						case '*':
						values[0]=values[0]*values[1];
						break;
						
						case '/':
						values[0]=values[0]/values[1];
						break;
						
						case '^':
						values[0]=pow(values[0],values[1]);
						break;
					}
					values[1]=values[2];
					values[2]=0.0;
					op[0]=op[1];
					op[1]=0;
					i=
				}
				else
				{
					
				}
			}
			else
			{
				i++;
			}
		}
		printf("%f",values[0]);
		i=0;
		for (i=0;i<3;i++)
			values[i]=0.0;
		for (i=0;i<2;i++)
			op[i]=0;
	}
	printf("End\n");
}

double getnextvalue(void)
{
	short int i;
	char outstring[40];
	for (i=0;i<40;i++)
	{
		outstring[i]=0;
	}
	i=0;
	int prevch='\0';
	ch = getchar();
	while (!isEndofvalue(prevch,ch))
	{
		outstring[i]=ch;
		prevch=ch;
		ch=getchar();
		i++;
	}
	return atof(outstring);
}

bool isEndofvalue(int ch1,int ch2)
{
	if (ch2=='+'||ch2=='-')
	{
		if (ch1!='e'&&ch1!='E')
		{
			return true;
		}
	}
	if (ch2=='*'||ch2=='/'||ch2=='^')
	{
		return true;
	}
	return false;
}


