#include <stdio.h>       			     	/* Header file for standard I/O library */
#include <math.h>        			             	/* Header file for math library */

main()
{ long i;
  double sum, current, pi;
  short c;
  c=-1;
  i = 3;                        			        	/* Denominator of current term */
  sum = 0.0;                      			  	                     	 /* So far sum */
  do {
    current = c*1/(((double) i-1)*((double) i)*((double) i+1)); 	  			       	   /* Current term */
    sum = sum+current;                 				    	/* Add current term to sum */
    i+=2;                                		                  	  /* Next term now */
	c*=-1;
  } while (((double)(c*(-1)))*current > 1.0e-25);  			 	 /* Stop if current term is very small */
  printf("Summed %8ld terms, (3-pi)/4 is %10.8f\n", (i-2)/2, sum);
  getchar();
}
