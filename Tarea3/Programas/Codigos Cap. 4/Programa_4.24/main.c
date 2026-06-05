#include <stdio.h>

 /* Parametros y funciones. */

 int f1(void);
 int f2(void);                                 /* Prototipos de funciones. */
 int f3(void);
 int f4(void);

 int K = 5; // Variable GLOBAL

 int main(void)
 {
    int I;
    for (I = 1; I <= 4; I++)
    {
        printf("\n\nEl resultado de la funcion f1 es: %d", f1());
        printf("\nEl resultado de la funcion f2 es: %d", f2());
        printf("\nEl resultado de la funcion f3 es: %d", f3());
        printf("\nEl resultado de la funcion f4 es: %d", f4());
    }
    return 0;
 }

 int f1(void)
 {
    K *= K; // Se corrigió el punto y coma que faltaba
    return (K);
 }

 int f2(void)
 {
    int K = 3; // Variable local K (oculta a la global de forma segura aquí)
    K++;
    return (K);
 }

 int f3(void)
 {
    static int K = 6; // Variable estática local
    K += 3;
    return (K);
}

 int f4(void)
 {
    int K_local = 4;       // AJUSTE PARA C: Cambiamos el nombre para evitar el conflicto
    K_local = K_local + K; // Ahora 'K' se refiere libremente a la variable GLOBAL
    return (K_local);
 }
