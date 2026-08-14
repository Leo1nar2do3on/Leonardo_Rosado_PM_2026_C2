#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Códigos de colores ANSI para la interfaz estética
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

typedef struct {
    int F;  // Función
    int S;  // Sustitución
    int P;  // Profundidad / Perturbación
    int E;  // Extensión
    int A;  // Agresión
    int V;  // Vulnerabilidad

    // Variables intermedias y finales del Método Mosler completo
    int I;   // Importancia del suceso (F * S)
    int D;   // Daños ocasionados (P * E)
    int C;   // Carácter del riesgo (I + D)
    int PR;  // Probabilidad (A * V)
    int ER;  // Riesgo Estimado / Clase (C * PR)
} ParametrosMosler;

typedef struct {
    int id;
    char nombre[50];
    ParametrosMosler mosler;
} Riesgo;

// Prototipos
void crearRiesgo(Riesgo **riesgos, int *total);
void modificarRiesgo(Riesgo *riesgos, int total);
void calcularRiesgos(Riesgo *riesgos, int total);
void imprimirRiesgos(Riesgo *riesgos, int total);
void borrarRiesgo(Riesgo **riesgos, int *total);
void guardarArchivo(Riesgo *riesgos, int total, const char *nombreArchivo);
void cargarArchivo(Riesgo **riesgos, int *total, const char *nombreArchivo);
void limpiarPantalla();

int main() {
    Riesgo *riesgos = NULL;
    int total = 0;
    int opcion;
    char nombreArchivo[50] = "riesgos_mosler.csv";

    do {
        limpiarPantalla();
        printf(CYAN BOLD "\n======================================================\n");
        printf("    METODO MOSLER COMPLETO - GESTION DE RIESGOS       \n");
        printf("======================================================\n" RESET);
        printf(YELLOW " [1]" RESET " Registrar Nuevo Riesgo\n");
        printf(YELLOW " [2]" RESET " Modificar Parametros Mosler (F, S, P, E, A, V)\n");
        printf(YELLOW " [3]" RESET " Calcular Gravedad y Riesgo Estimado (ER)\n");
        printf(YELLOW " [4]" RESET " Mostrar Listado y Reporte Completo\n");
        printf(YELLOW " [5]" RESET " Eliminar Riesgo\n");
        printf(YELLOW " [6]" RESET " Configurar Archivo (Actual: " GREEN "%s" YELLOW ")\n", nombreArchivo);
        printf(YELLOW " [7]" RESET " Guardar Datos en Disco\n");
        printf(YELLOW " [8]" RESET " Cargar Datos desde Disco\n");
        printf(RED   " [9]" RESET " Salir del Sistema\n");
        printf(CYAN "------------------------------------------------------\n" RESET);
        printf(BOLD " Seleccione una opcion [1-9]: " RESET);

        if (scanf("%d", &opcion) != 1) {
            printf(RED "Error: Ingrese un numero valido.\n" RESET);
            while(getchar() != '\n');
            continue;
        }

        switch(opcion) {
            case 1:
                crearRiesgo(&riesgos, &total);
                break;
            case 2:
                modificarRiesgo(riesgos, total);
                break;
            case 3:
                calcularRiesgos(riesgos, total);
                break;
            case 4:
                imprimirRiesgos(riesgos, total);
                break;
            case 5:
                borrarRiesgo(&riesgos, &total);
                break;
            case 6:
                printf(CYAN "\nIngrese el nuevo nombre del archivo (ej. datos.csv): " RESET);
                scanf("%s", nombreArchivo);
                printf(GREEN "Archivo configurado exitosamente.\n" RESET);
                break;
            case 7:
                guardarArchivo(riesgos, total, nombreArchivo);
                break;
            case 8:
                cargarArchivo(&riesgos, &total, nombreArchivo);
                break;
            case 9:
                printf(GREEN "\nSaliendo del sistema Mosler. ¡Hasta luego!\n\n" RESET);
                break;
            default:
                printf(RED "Opcion no valida. Intente de nuevo.\n" RESET);
        }
        if (opcion != 9) {
            printf(CYAN "\nPresione ENTER para continuar..." RESET);
            while(getchar() != '\n');
            getchar();
        }
    } while(opcion != 9);

    if (riesgos != NULL) {
        free(riesgos);
    }

    return 0;
}

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void crearRiesgo(Riesgo **riesgos, int *total) {
    *riesgos = realloc(*riesgos, (*total + 1) * sizeof(Riesgo));
    if (*riesgos == NULL) {
        printf(RED "Error de asignacion de memoria dinamica.\n" RESET);
        exit(1);
    }

    Riesgo nuevo;
    nuevo.id = *total + 1;
    printf(CYAN "\n--- REGISTRAR NUEVO RIESGO ---\n" RESET);
    printf("Ingrese la descripcion del riesgo: ");
    scanf(" %[^\n]", nuevo.nombre);

    nuevo.mosler.F = 0; nuevo.mosler.S = 0; nuevo.mosler.P = 0;
    nuevo.mosler.E = 0; nuevo.mosler.A = 0; nuevo.mosler.V = 0;
    nuevo.mosler.I = 0; nuevo.mosler.D = 0; nuevo.mosler.C = 0;
    nuevo.mosler.PR = 0; nuevo.mosler.ER = 0;

    (*riesgos)[*total] = nuevo;
    (*total)++;
    printf(GREEN "Riesgo registrado con exito (ID: %d).\n" RESET, nuevo.id);
}

void modificarRiesgo(Riesgo *riesgos, int total) {
    if (total == 0) {
        printf(RED "No hay riesgos registrados en el sistema.\n" RESET);
        return;
    }

    int id;
    printf(CYAN "\n--- MODIFICAR PARAMETROS MOSLER (6 CRITERIOS) ---\n" RESET);
    printf("Ingrese el ID del riesgo a modificar: ");
    scanf("%d", &id);

    if (id < 1 || id > total) {
        printf(RED "ID fuera de rango.\n" RESET);
        return;
    }

    Riesgo *r = &riesgos[id - 1];
    printf(YELLOW "Modificando: %s\n" RESET, r->nombre);
    printf("Ingrese los valores para cada criterio en la escala de 1 a 5:\n");
    printf("  [F] Funcion (Consecuencias/Daños): "); scanf("%d", &r->mosler.F);
    printf("  [S] Sustitucion (Dificultad de reemplazar): "); scanf("%d", &r->mosler.S);
    printf("  [P] Profundidad / Perturbación (Imagen/Psicológico): "); scanf("%d", &r->mosler.P);
    printf("  [E] Extension (Alcance geográfico/afectación): "); scanf("%d", &r->mosler.E);
    printf("  [A] Agresión (Probabilidad de manifestación): "); scanf("%d", &r->mosler.A);
    printf("  [V] Vulnerabilidad (Exposición a sufrir daños): "); scanf("%d", &r->mosler.V);

    printf(GREEN "Parametros actualizados correctamente.\n" RESET);
}

void calcularRiesgos(Riesgo *riesgos, int total) {
    if (total == 0) {
        printf(RED "No hay riesgos para calcular.\n" RESET);
        return;
    }

    for (int i = 0; i < total; i++) {
        ParametrosMosler *m = &riesgos[i].mosler;
        m->I  = m->F * m->S;       // Importancia del suceso
        m->D  = m->P * m->E;       // Daños ocasionados
        m->C  = m->I + m->D;       // Carácter del riesgo
        m->PR = m->A * m->V;       // Probabilidad
        m->ER = m->C * m->PR;      // Riesgo Estimado (Clase)
    }
    printf(GREEN "\nCalculos matematicos del Metodo Mosler completados con exito.\n" RESET);
}

void imprimirRiesgos(Riesgo *riesgos, int total) {
    if (total == 0) {
        printf(RED "\nNo hay riesgos registrados para mostrar.\n" RESET);
        return;
    }

    printf(CYAN "\n=========================================================================================================\n");
    printf("                                  REPORTE GENERAL - METODO MOSLER COMPLETO                               \n");
    printf("=========================================================================================================\n" RESET);
    printf(BOLD "ID | Descripcion                  | F | S | P | E | A | V |  I  |  D  |  C  | PR  |   ER (Clase)    \n");
    printf("---------------------------------------------------------------------------------------------------------\n" RESET);
    for (int i = 0; i < total; i++) {
        Riesgo r = riesgos[i];
        printf("%-2d | %-28s | %d | %d | %d | %d | %d | %d | %3d | %3d | %3d | %3d | " BOLD "%-9d\n" RESET,
               r.id, r.nombre, r.mosler.F, r.mosler.S, r.mosler.P,
               r.mosler.E, r.mosler.A, r.mosler.V,
               r.mosler.I, r.mosler.D, r.mosler.C, r.mosler.PR, r.mosler.ER);
    }
    printf(CYAN "=========================================================================================================\n" RESET);
}

void borrarRiesgo(Riesgo **riesgos, int *total) {
    if (*total == 0) {
        printf(RED "No hay riesgos para eliminar.\n" RESET);
        return;
    }

    int id;
    printf(RED "\n--- ELIMINAR RIESGO ---\n" RESET);
    printf("Ingrese el ID del riesgo que desea eliminar: ");
    scanf("%d", &id);

    if (id < 1 || id > *total) {
        printf(RED "ID invalido.\n" RESET);
        return;
    }

    for (int i = id - 1; i < *total - 1; i++) {
        (*riesgos)[i] = (*riesgos)[i + 1];
        (*riesgos)[i].id = i + 1;
    }

    (*total)--;
    *riesgos = realloc(*riesgos, (*total) * sizeof(Riesgo));
    printf(GREEN "Riesgo eliminado exitosamente.\n" RESET);
}

void guardarArchivo(Riesgo *riesgos, int total, const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) {
        printf(RED "Error al abrir el archivo para escritura.\n" RESET);
        return;
    }

    fprintf(f, "ID,Nombre,F,S,P,E,A,V,I,D,C,PR,ER\n");
    for (int i = 0; i < total; i++) {
        Riesgo r = riesgos[i];
        fprintf(f, "%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                r.id, r.nombre, r.mosler.F, r.mosler.S, r.mosler.P,
                r.mosler.E, r.mosler.A, r.mosler.V,
                r.mosler.I, r.mosler.D, r.mosler.C, r.mosler.PR, r.mosler.ER);
    }

    fclose(f);
    printf(GREEN "Datos guardados exitosamente en el archivo CSV: %s\n" RESET, nombreArchivo);
}

void cargarArchivo(Riesgo **riesgos, int *total, const char *nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "r");
    if (f == NULL) {
        printf(RED "No se pudo encontrar o abrir el archivo especificado.\n" RESET);
        return;
    }

    char linea[256];
    fgets(linea, sizeof(linea), f); // Omitir cabecera

    int count = 0;
    Riesgo *temp = NULL;

    while (fgets(linea, sizeof(linea), f) != NULL) {
        temp = realloc(*riesgos, (count + 1) * sizeof(Riesgo));
        if (temp == NULL) {
            printf(RED "Error de memoria al cargar el archivo.\n" RESET);
            break;
        }
        *riesgos = temp;

        Riesgo r;
        sscanf(linea, "%d,%49[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
               &r.id, r.nombre, &r.mosler.F, &r.mosler.S, &r.mosler.P,
               &r.mosler.E, &r.mosler.A, &r.mosler.V,
               &r.mosler.I, &r.mosler.D, &r.mosler.C, &r.mosler.PR, &r.mosler.ER);

        r.id = count + 1;
        (*riesgos)[count] = r;
        count++;
    }

    *total = count;
    fclose(f);
    printf(GREEN "Datos cargados con exito desde %s. Total de registros: %d\n" RESET, nombreArchivo, *total);
}
