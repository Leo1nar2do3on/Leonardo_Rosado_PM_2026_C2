/* ========================================================================
   leer_json.c
   Lee un archivo JSON (arreglo de objetos Alumno), cuenta cuantos
   elementos hay (primera pasada, contando la clave "Nombre"), reserva
   un arreglo dinamico con esa cantidad (malloc) y luego carga los
   datos en memoria (segunda pasada) para finalmente imprimirlos.

   Nota: se hace un parseo simple linea por linea, asumiendo el formato
   "clave": valor  (un campo por linea), tal como lo genera el archivo
   alumnos.json de este ejercicio. No es un parser JSON general.
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

/* Extrae el valor asociado a una clave "Clave": valor  dentro de una linea */
int extraerValor(const char *linea, const char *clave, char *destino) {
    char buscado[MAX_CAMPO];
    sprintf(buscado, "\"%s\"", clave);

    char *pos = strstr(linea, buscado);
    if (pos == NULL) return 0;

    /* avanzar hasta despues de los ':' */
    pos = strchr(pos, ':');
    if (pos == NULL) return 0;
    pos++;

    /* saltar espacios */
    while (*pos == ' ' || *pos == '\t') pos++;

    if (*pos == '"') {
        /* valor tipo texto: "valor" */
        pos++;
        char *fin = strchr(pos, '"');
        if (fin == NULL) return 0;
        int longitud = (int)(fin - pos);
        strncpy(destino, pos, longitud);
        destino[longitud] = '\0';
    } else {
        /* valor numerico: puede terminar en ',' o '\n' */
        int i = 0;
        while (*pos != ',' && *pos != '\n' && *pos != '\r' && *pos != '\0') {
            destino[i++] = *pos++;
        }
        destino[i] = '\0';
    }
    return 1;
}

int main(void) {
    const char *nombreArchivo = "alumnos.json";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return 1;
    }

    /* ---------- PRIMERA PASADA: contar cuantas veces aparece "Nombre" ---------- */
    char linea[MAX_LINEA];
    int n = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "\"Nombre\"") != NULL) {
            n++;
        }
    }

    if (n == 0) {
        printf("No se encontraron alumnos en el archivo.\n");
        fclose(archivo);
        return 0;
    }

    printf("Cantidad de alumnos detectados en el JSON: %d\n\n", n);

    /* ---------- RESERVA DE MEMORIA DINAMICA ---------- */
    Alumno *alumnos = (Alumno *) malloc(n * sizeof(Alumno));
    if (alumnos == NULL) {
        printf("Error al reservar memoria.\n");
        fclose(archivo);
        return 1;
    }

    /* ---------- SEGUNDA PASADA: cargar los datos ---------- */
    rewind(archivo);
    int indice = -1;
    char campo[MAX_CAMPO];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (extraerValor(linea, "Nombre", campo)) {
            indice++;
            strncpy(alumnos[indice].nombre, campo, MAX_CAMPO - 1);
        } else if (extraerValor(linea, "Apellido", campo)) {
            strncpy(alumnos[indice].apellido, campo, MAX_CAMPO - 1);
        } else if (extraerValor(linea, "Promedio", campo)) {
            alumnos[indice].promedio = atof(campo);
        } else if (extraerValor(linea, "Materia", campo)) {
            strncpy(alumnos[indice].materia, campo, MAX_CAMPO - 1);
        }
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
