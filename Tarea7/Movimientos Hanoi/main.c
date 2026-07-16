#include <stdio.h>
#include <stdlib.h>

// Variable global para llevar el conteo de los movimientos
int contador_movimientos = 0;

// Función recursiva para resolver las Torres de Hanoi y escribir en el archivo
void resolverHanoi(int n, char origen, char destino, char auxiliar, FILE *archivo) {
    // Caso base: Si solo queda un disco, se mueve directamente
    if (n == 1) {
        contador_movimientos++;
        fprintf(archivo, "Movimiento %d: Mover disco 1 desde la torre %c hasta la torre %c\n", contador_movimientos, origen, destino);
        printf("Movimiento %d: Mover disco 1 desde la torre %c hasta la torre %c\n", contador_movimientos, origen, destino);
        return;
    }

    // 1. Mover los n-1 discos superiores de Origen a Auxiliar
    resolverHanoi(n - 1, origen, auxiliar, destino, archivo);

    // 2. Mover el disco grande restante de Origen a Destino
    contador_movimientos++;
    fprintf(archivo, "Movimiento %d: Mover disco %d desde la torre %c hasta la torre %c\n", contador_movimientos, n, origen, destino);
    printf("Movimiento %d: Mover disco %d desde la torre %c hasta la torre %c\n", contador_movimientos, n, origen, destino);

    // 3. Mover los n-1 discos de Auxiliar a Destino
    resolverHanoi(n - 1, auxiliar, destino, origen, archivo);
}

int main() {
    int discos;
    FILE *archivo;

    printf("--- SIMULADOR DE TORRES DE HANOI ---\n");
    printf("Introduce el numero de discos con los que deseas jugar: ");
    if (scanf("%d", &discos) != 1 || discos <= 0) {
        printf("Por favor, introduce un numero entero positivo valido.\n");
        return 1;
    }

    // Abrir (o crear) el archivo de texto en modo escritura ("w")
    archivo = fopen("movimientos_hanoi.txt", "w");
    if (archivo == NULL) {
        printf("¡Error al intentar crear o abrir el archivo de registro!\n");
        return 1;
    }

    // Escribir encabezado en el archivo
    fprintf(archivo, "=== REGISTRO DE MOVIMIENTOS: TORRES DE HANÓI (%d DISCOS) ===\n\n", discos);
    printf("\nIniciando resolucion... Los pasos se guardaran en 'movimientos_hanoi.txt'.\n\n");

    // Ejecutar el algoritmo
    // Columnas representadas por caracteres: A (Origen), C (Destino), B (Auxiliar)
    resolverHanoi(discos, 'A', 'C', 'B', archivo);

    // Escribir el resumen final en el archivo y cerrarlo
    fprintf(archivo, "\nProceso completado con exito en un total de %d movimientos.\n", contador_movimientos);
    fclose(archivo);

    printf("\n¡Simulacion terminada con exito!");
    printf("\nSe han registrado %d movimientos en el archivo 'movimientos_hanoi.txt'.\n", contador_movimientos);

    return 0;
}
