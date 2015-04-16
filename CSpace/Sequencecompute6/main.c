#include <stdio.h>       			     	/* Header file for standard I/O library */
#include <math.h>        			             	/* Header file for math library */

main()
{ long i;
  double sum, current, pi;
  i = 1;                        			        	/* Denominator of current term */
  sum = 0.0;                      			  	                     	 /* So far sum */
  do {
    current = 1/(((double) i*i)*((double) i*i)); 	  			       	   /* Current term */
    sum = sum+current;                 				    	/* Add current term to sum */
    i++;                                		                  	  /* Next term now */
  } while (current > 1.0e-35);  			 	 /* Stop if current term is very small */
  printf("Summed %8ld terms, (pi^4)/90 is %10.8f\n", i-1, sum);
  getchar();
}
