/* Epektaseis:A,B,C,D
 * 
 * This program is a simple calculator. It calculates the value of an expression
 * given until a newline character or end of file signal. It then prints the result
 * in the following format: Result <result count> : <result>
 * If an error is encountered the type of error is displayed instead of a result.
 * Space and tab characters are ignored. Empty lines are ignored.
 * Each number or parenthesis can be preceded by a single + or - sign.
 * The program continues to compute each line until it receives an end of file signal.
 * You can press (Control-D) in most Unix and (Control-Z) on most Windows platforms
 * to send an end of file signal.
 * Valid characters are: 1 2 3 4 5 6 7 8 9 0 + - * ( )
 * Any other non empty character results in an error.
 */

//Header files
#include <stdio.h>

//Function prototypes
int calculate(int*,int*,int); 
/* The function calculate returns the value of the mathematical expression
 * given from the standard input. If an error is encountered
 * it returns -1. 
 * The first integer pointer is necessery for returning the last character
 * read inside the function. 
 * The second integer pointer is used to inform the calling function that 
 * an error has been encountered and that there is no need for further computations.
 * It also shows the type of error encountered.
 * The integer holds the number of still open parentheses. If a parenthesis opener 
 * is encountered the function calls itself with the integer increased by 1
 * to calculate the value of the parenthesis.
 */
	
int main (void)
{
	//Variable declarations
	int result;				
	int resultcounter=0;	//How many results have been printed
	int ch;					
	int error;		
	/*  Error codes:
	 * -1 -> unknown character 
	 *  0 -> no error
	 *  1 -> missing operator before number
	 *  2 -> missing operator before parenthesis
	 *  3 -> missing number between operators
	 *  4 -> missing number at the beginning
	 *  5 -> missing number at the end
	 *  6 -> missing parenthesis opener
	 *  7 -> missing parenthesis terminator
	 *  8 -> empty parenthesis
	 *  9 -> missing number at the beggining of parenthesis
	 * 10 -> missing number at the end of parenthesis
	 */ 
	
	ch=getchar();
	while (ch=='\n'||ch=='\t'||ch==' ')//Ignore empty input
	{
		ch=getchar();
	}
	while (ch!=EOF)
	{
		error = 0;
		result=calculate(&ch,&error,0);
		resultcounter++;
		printf("Result %d: ",resultcounter);
		switch (error)
		{
			case -1:
			printf("Unknown character\n");
			break;
			case 0:
			printf("%d\n",result);
			break;
			case 1:
			printf("Missing operator before number\n");
			break;
			case 2:
			printf("Missing operator before parenthesis\n");
			break;
			case 3:
			printf("Missing number between operators\n");
			break;
			case 4:
			printf("Missing number at the beginning\n");
			break;
			case 5:
			printf("Missing number at the end\n");
			break;
			case 6:
			printf("Missing parenthesis opener\n");
			break;
			case 7:
			printf("Missing parenthesis terminator\n");
			break;
			case 8:
			printf("Empty parenthesis\n");
			break;
			case 9:
			printf("Missing number at the beginning of parenthesis\n");
			break;
			case 10:
			printf("Missing number at the end of parenthesis\n");
			break;
		}
		while (ch!='\n'&&ch!=EOF)	//Go to the end of line or the end of file. This is done in case an error has 
		{							//forced calculate() to exit before reaching the end of line or the end of file.
			ch=getchar();
		}
		if (ch!=EOF) 
		{
			ch=getchar();
			while (ch=='\n'||ch=='\t'||ch==' ') //Ignore empty input
			{
				ch=getchar();
			}
		}
	}
	return 0;
}
	
int calculate (int* pch,int* perror,int parencount)
{
	//Variable declarations
	int num1;	//This is the value returned by the function if no error is encountered.
	short int num1sgn;
	int num2;
	short int num2sgn;
	int num3;
	short int num3sgn;
	int symbolcount;
	char op1;
	char op2;
	short int isfull;
	/* 0 <- num1, num2 and num3 are empty
	 * 1 <- num2 and num3 are empty
	 * 2 <- num3 is empty
	 * 3 <- num1, num2 and num3 are filled
	 */
	char expecting;
	/* 'n' <- number or sign ('+' or '-')
	 * 'o' <- operator ('+','-' or '*')
	 */
	
	//Variable initialisation
	symbolcount=0;				//How many '+','-' or '*' have been found before a number. Used for error checking
	op1='\0';
	op2='\0';					// '\0' means that op1 and op2 are empty
	isfull = 0;					//Explained where variable is declared
	expecting = 'n';			//Explained where variable is declared
	num1=num2=num3=0;
	num1sgn=num2sgn=num3sgn=1;	//If EXACTLY one of the symbols preceding a number is '-' then this becomes -1.
	
	while (*pch!='\n'&&*pch!=')'&&*pch!=EOF&&*perror==0)	//Continue until the end of line, a parenthesis terminator or an error is encountered
	{
		while (*pch==' '||*pch=='\t') //**Ignore space and tab characters
		{
			*pch=getchar();
		}
		if (*pch=='+'||*pch=='-'||*pch=='*') //**Handle operators and signs (referred to collectively as symbols)
		{
			symbolcount++;
			if (isfull==0&&symbolcount>1) //If no number has been given yet but there have been more than 1 symbol given (eg: - +1 + 1)
			{
				if (parencount==0)	//If we aren't inside a parenthesis
				{
					*perror=4;	//Then a number is missing at the beginning of the input
				}
				else	//If we are inside a parenthesis
				{
					*perror=9;	//Then a number is missing at the beginning of this parenthesis
				}
			}
			else if (symbolcount>2)	//If more then 2 symbols are given (eg: +1 - - -1)
			{
				*perror=3;	//Then there is a number missing between operators.
			}
			else if (expecting=='n' && *pch=='*')	//If a number is expected but instead a multiplication operator is given (eg: *1+1 or 1+*1 or 1**1)
			{
				if (isfull!=0)	//If this isn't the first symbol given
				{
					*perror=3; //Then there is a number missing between operators.
				}
				else	//If this is the first symbol given
				{
					if (parencount==0)	//If we aren't inside a parenthesis
					{
						*perror=4;	//Then a number is missing at the beginning of the input
					}
					else	//If we are inside a parenthesis
					{
						*perror=9;	//Then a number is missing at the beginning of this parenthesis
					}
				}
			}
			else	//If no error has been encountered
			{
				switch (isfull) //Place the sign according to which number is empty
				{
					case 0:
					/* If this is the first number given, then because of the error checking done previously
					 * we can be sure that the symbol is either a '+' or '-' sign. Since all numbers are treated as 
					 * positive by default the '+' sign is ignored.
					 */
						if (*pch=='-')
						{
							num1sgn*=-1; //Give the first number a negative sign
						}
					break;
					case 1:
						if (*pch=='+'&&op1=='\0') //If op1 is not empty then '+' is a sign and it is ignored
						{
								op1='+'; //The first operator is the adittion operator
						}
						else if (*pch=='-')
						{
							num2sgn*=-1;
							if (op1=='\0')	//If op1 is empty then it is treated as an adittion with a negative
							{				//(each subtraction can be treated as the adition of the numbers' opposite)
								op1='+';
							}
						}
						else if (*pch=='*')
						{
							op1='*';
						}
					break;
					case 2:
						if (*pch=='+'&&op2=='\0')
						{
								op2='+';
						}
						else if (*pch=='-')
						{
							num3sgn*=-1;
							if (op2=='\0')
							{
								op2='+';
							}
						}
						else if (*pch=='*')
						{
							op2='*';
						}
					break;
				}
				expecting='n';//After each operator a number is expected (including its sign)			
				*pch=getchar();
			}
		}
		else if (*pch>='0'&&*pch<='9')	//**Handle numbers
		{
			if (expecting=='o')	//If an operator is expected
			{
				*perror=1;	//An operator is missing before this number
			}
			else 
			{
				switch (isfull) //Place the number according to which number is empty
				{
					case 0:
					while (*pch>='0'&&*pch<='9')
					{
						num1=num1*10+(*pch-'0');
						*pch=getchar();
					}
					isfull=1; //The first number has been filled
					break;
					case 1:
					while (*pch>='0'&&*pch<='9')
					{
						num2=num2*10+*pch-'0';
						*pch=getchar();
					}
					isfull=2; //The second number has been filled
					break;
					case 2:
					while (*pch>='0'&&*pch<='9')
					{
						num3=num3*10+*pch-'0';
						*pch=getchar();
					}
					isfull=3; //All numbers have been filled
					break;
				}
				expecting='o';	//After each number an operator is expected
				symbolcount=0;	//And the symbolcount is set to 0 to acommodate for the next number's sign and operator
			}
		}
		else if (*pch=='(') //**Handle parenthesis
		{
			if (expecting=='o') //If an operator is expected
			{
				*perror=2; //An operator is missing before this parenthesis
			}
			else
			{
				*pch=getchar();
				while (*pch=='\t'||*pch==' ') //Skip to first non-empty character in the parenthesis
				{
					*pch=getchar();
				}
				if (*pch==')') //If the first non-empty character in the parenthesis is the parenthesis terminator character
				{
					*perror=8; //Then this parenthesis is empty
				}
				else //If this parenthesis is not empty
				{
					switch (isfull) //Place the value of the parenthesis according to which number is empty
					{
						case 0:
						num1=calculate(pch,perror,parencount+1); //Call calculate() to return the value of the parenthesis
						isfull=1;
						break;
						case 1:
						num2=calculate(pch,perror,parencount+1);
						isfull=2;
						break;
						case 2:
						num3=calculate(pch,perror,parencount+1);
						isfull=3;
						break;
					}
					if (*perror!=7)		//This is necessery in case error 7 has occured because the last character is a newline. 
					{					//If the program tries to read after the newline no error will be printed and it will
						*pch=getchar();	//continue to wait for input.
					}
					expecting='o';
					symbolcount=0;
				}
			}
		}
		else if (*pch!='\n'&&*pch!=')'&&*pch!=EOF) 
		/* Since none of the above are true (the character read is not a number, an 
		 * operator or a parenthesis opener) and the character read is not one of the
		 * other acceptable characters.
		 */
		{
			*perror=-1; //It is an unknown character
		}
		if (isfull==3&&*perror==0) //**If all numbers are full and no error has been encountered
		{
			num1*=num1sgn; //Give all numbers their sign
			num2*=num2sgn;
			num3*=num3sgn;
			if (op1=='+' && op2=='+') //If both operators are the adittion operators 
			{
				num1+=num2; //Add the second number to the first
				
				op1=op2; //Move the the second operator to the first operators' place
				num2=num3; //Move the the third number to the first numbers' place
				
				op2='\0'; //op2 is empty again
				num3=0; //num3 is empty again
				isfull=2; //the third number is empty
			}
			else if (op1=='+' && op2=='*') // If the second operator has priority
			{
				num2*=num3; //Multiply the second number with the third
				
				op2='\0';
				num3=0;
				
				isfull=2;
			}
			else //if (op1=='*' && (op2=='*' || op2=='+')) //not necessary
			{
				num1*=num2;
				
				op1=op2;
				num2=num3;
				
				op2='\0';
				num3=0;
				
				isfull=2;
			}
			num1sgn=num2sgn=num3sgn=1;	//All signs returned to default value. 
		}								//After the first time only num3sgn will be usefull.
	}
	
	
	if (*pch==EOF) 
	/* If the user pressed (Control-D) twice in the middle of an expression put a newline 
	 * character so that the result won't appear on the same line as the expression. This
	 * usually is not a problem when input is redirected from a file as many editors automatically
	 * put a newline character before the end of file.
	 */
	{
		putchar('\n');
	}
	if (*perror==0) //If no error has been found yet
	{
		if ((*pch=='\n'||*pch==EOF)&&parencount!=0) //If this is the end of line but there are still open parentheses
		{
			*perror=7; //There is a missing parenthesis terminator
		}
		else if (expecting=='n'&&isfull!=0) //If a number was expected and this isn't because it's the default value
		{
			if (parencount==0) //If we aren't inside a parenthesis
			{
				*perror=5; //There is a number missing at the end
			}
			else //If we are inside a parenthesis
			{
				*perror=10; //There is a number missing at the end of this parenthesis
			}
		}
		else if (symbolcount!=0&&isfull==0) //If a sign was given but nothing else
		{
			if (parencount==0) //If we aren't inside a parenthesis
			{
				*perror=4; //There is a number missing at the beginning
			}
			else //If we are inside a parenthesis
			{
				*perror=9; //There is a number missing at the beginning of this parenthesis
			}
		}
		else if (*pch==')'&&parencount==0) //If a parenthesis terminator has been encountered before a parenthesis opener
		{
			*perror=6; //There is a missing parentheses opener
		}
	}
	if (*perror==0) //If no error has been found
	{
		num1*=num1sgn; //In case only one number was given and it had a negative sign
		if (isfull==2) //If there are two numbers left non-empty do what's necessary 
		{
			num2*=num2sgn;
			if (op1=='+') 
			{
				num1+=num2;
			}
			else
			{
				num1*=num2;
			}
		}
		return num1; //If there has been no error return num1, which now holds the result
	}
	return -1; //Else return -1
}
