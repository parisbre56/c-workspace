#include <stdio.h>       			     	/* Header file for standard I/O library */
#include <math.h>        			             	/* Header file for math library */

main()
{ long i;
  double sum, current, pi;
  i = 1;                        			        	/* Denominator of current term */
  sum = 0.0;                      			  	                     	 /* So far sum */
  do {
    current = 1/(((double) i)*((double) i+2)); 	  			       	   /* Current term */
    sum = sum+current;                 				    	/* Add current term to sum */
    i+=2;                                		                  	  /* Next term now */
  } while (current > 1.0e-15);  			 	 /* Stop if current term is very small */
  printf("Summed %8ld terms, result is %10.8f\n", i-1, sum);
  printf("This must be 0.5 ...");
  getchar();
}
