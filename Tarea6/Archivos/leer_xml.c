/* ========================================================================
   leer_xml.c
   Lee un archivo XML con estructuras <Alumno>, cuenta cuantos elementos
   hay (primera pasada), reserva un arreglo dinamico con esa cantidad
   (malloc) y luego carga los datos en memoria (segunda pasada) para
   finalmente imprimirlos.
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

/* Quita espacios y saltos de linea al principio/final de una cadena */
void limpiar(char *s) {
    /* quitar '\n' o '\r' al final */
    int len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || s[len-1] == ' ')) {
        s[--len] = '\0';
    }
    /* quitar espacios al inicio */
    char *inicio = s;
    while (*inicio == ' ' || *inicio == '\t') inicio++;
    if (inicio != s) memmove(s, inicio, strlen(inicio) + 1);
}

/* Extrae el contenido entre <etiqueta> y </etiqueta> de una linea */
int extraerContenido(const char *linea, const char *etiqueta, char *destino) {
    char aperturaTag[MAX_CAMPO];
    sprintf(aperturaTag, "<%s>", etiqueta);

    char *inicio = strstr(linea, aperturaTag);
    if (inicio == NULL) return 0;

    inicio += strlen(aperturaTag);
    char *fin = strstr(inicio, "</");
    if (fin == NULL) return 0;

    int longitud = (int)(fin - inicio);
    strncpy(destino, inicio, longitud);
    destino[longitud] = '\0';
    return 1;
}

int main(void) {
    const char *nombreArchivo = "alumnos.xml";
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return 1;
    }

    /* ---------- PRIMERA PASADA: contar cuantos <Alumno> hay ---------- */
    char linea[MAX_LINEA];
    int n = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "<Alumno>") != NULL) {
            n++;
        }
    }

    if (n == 0) {
        printf("No se encontraron alumnos en el archivo.\n");
        fclose(archivo);
        return 0;
    }

    printf("Cantidad de alumnos detectados en el XML: %d\n\n", n);

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
        if (strstr(linea, "<Alumno>") != NULL) {
            indice++;
            continue;
        }
        if (indice < 0) continue; /* aun no entramos a ningun <Alumno> */

        if (extraerContenido(linea, "Nombre", campo)) {
            limpiar(campo);
            strncpy(alumnos[indice].nombre, campo, MAX_CAMPO - 1);
        } else if (extraerContenido(linea, "Apellido", campo)) {
            limpiar(campo);
            strncpy(alumnos[indice].apellido, campo, MAX_CAMPO - 1);
        } else if (extraerContenido(linea, "Promedio", campo)) {
            limpiar(campo);
            alumnos[indice].promedio = atof(campo);
        } else if (extraerContenido(linea, "Materia", campo)) {
            limpiar(campo);
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
