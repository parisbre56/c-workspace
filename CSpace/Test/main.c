//Header files
#include <stdio.h>

int main(void)
{
	int i;
	for (i=-1;i<=3000;i++)
	{
		printf("%d:",i);
		putchar(i);
		putchar('\n');
	}
	getchar();
}