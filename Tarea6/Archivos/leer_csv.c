/* ========================================================================
   leer_csv.c
   Lee un archivo CSV con estructuras Alumno (Nombre,Apellido,Promedio,
   Materia), cuenta cuantos registros hay (primera pasada, sin contar
   el encabezado), reserva un arreglo dinamico con esa cantidad (malloc)
   y luego carga los datos en memoria (segunda pasada) para finalmente
   imprimirlos.
   ======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA   256
#define MAX_CAMPO   100

typedef struct {
    char nombre[MAX_CAMPO];
    char apellido[MAX_CAMPO];
    float promedio;
    char materia[MAX_CAMPO];
} Alumno;

/* Quita '\n' / '\r' al final de una cadena */
void limpiar(char *s) {
    int len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[--len] = '\0';
    }
}

int main(void) {
    const char *nombreArchivo = "alumnos.csv";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return 1;
    }

    char linea[MAX_LINEA];

    /* ---------- PRIMERA PASADA: contar lineas de datos (sin encabezado) ---------- */
    int n = 0;
    int esEncabezado = 1;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (esEncabezado) {           /* la primera linea es el encabezado */
            esEncabezado = 0;
            continue;
        }
        /* ignorar posibles lineas vacias al final del archivo */
        if (strlen(linea) > 1) {
            n++;
        }
    }

    if (n == 0) {
        printf("No se encontraron alumnos en el archivo.\n");
        fclose(archivo);
        return 0;
    }

    printf("Cantidad de alumnos detectados en el CSV: %d\n\n", n);

    /* ---------- RESERVA DE MEMORIA DINAMICA ---------- */
    Alumno *alumnos = (Alumno *) malloc(n * sizeof(Alumno));
    if (alumnos == NULL) {
        printf("Error al reservar memoria.\n");
        fclose(archivo);
        return 1;
    }

    /* ---------- SEGUNDA PASADA: cargar los datos ---------- */
    rewind(archivo);
    esEncabezado = 1;
    int indice = 0;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (esEncabezado) {
            esEncabezado = 0;
            continue;
        }
        limpiar(linea);
        if (strlen(linea) == 0) continue;

        char *token = strtok(linea, ",");
        if (token != NULL) strncpy(alumnos[indice].nombre, token, MAX_CAMPO - 1);

        token = strtok(NULL, ",");
        if (token != NULL) strncpy(alumnos[indice].apellido, token, MAX_CAMPO - 1);

        token = strtok(NULL, ",");
        if (token != NULL) alumnos[indice].promedio = atof(token);

        token = strtok(NULL, ",");
        if (token != NULL) strncpy(alumnos[indice].materia, token, MAX_CAMPO - 1);

        indice++;
    }
    fclose(archivo);

    /* ---------- IMPRESION ---------- */
    printf("%-4s %-12s %-12s %-10s %-25s\n", "N", "Nombre", "Apellido", "Promedio", "Materia");
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-4d %-12s %-12s %-10.2f %-25s\n",
               i + 1,
               alumnos[i].nombre,
               alumnos[i].apellido,
               alumnos[i].promedio,
               alumnos[i].materia);
    }

    /* ---------- LIBERAR MEMORIA ---------- */
    free(alumnos);

    return 0;
}
