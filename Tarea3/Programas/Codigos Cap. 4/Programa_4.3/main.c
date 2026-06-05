#include <stdio.h>

 /* /* Conflicto de variables con el mismo nombre. */

 void f1(void);            /* Prototipo de función. */
 int K = 5;                /* Variable global. */

 void main (void)
 {
   int I;
   for (I = 1; I <= 3; I++)
       f1();
 }

 void f1(void)                                                                                                                                                   /* La función utiliza tanto la variable local I como la variable
 /* La funcion utiliza tanto la variable local I como la variable global I. */
 {
   int local_K = 2;                /* Variable local. */
   local_K += local_K;
   printf("\n\nEl valor de la variable local es: %d", K);
   K = K + local_K;            /* Uso de ambas variables. */
   printf("\nEl valor de la variable global es: %d", K);
 }
