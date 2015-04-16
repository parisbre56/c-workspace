#include <stdio.h>       			     	/* Header file for standard I/O library */
#include <math.h>        			             	/* Header file for math library */

main()
{ long i,c;
  double result, current;
  i = 1;                        			        	/* Denominator of current term */
  c = 2;
  result = 1.0;                      			  	                     	 /* So far sum */
  do {
    current = ((double)c)/(((double) i)); 	  			       	   /* Current term */
    result *=current;                 				    	/* Add current term to sum */
	//printf("%10.8f : %d / %d = %10.8f \n",c,i,result,current);	
    i+=2;                                		                  	  /* Next term now */
	//getchar();
	current = ((double)c)/(((double) i)); 	  			       	   /* Current term */
    result *=current;                 				    	/* Add current term to sum */
	//printf("%10.8f : %d / %d = %10.8f \n",c,i,result,current);	
	c+=2;                                		                  	  /* Next term now */
	//getchar();
  } while ((current < 1-1.0e-7)||(current > 1+1.0e-7));  			 	 /* Stop if current term is very small */
  printf("Summed %8ld terms, pi/2 is %10.8f\n", i-2,result);
  getchar();
}
