#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Función para obtener la extensión de un archivo (ej: ".png")
const char *obtener_extension(const char *nombre) {
    const char *punto = strrchr(nombre, '.');
    if (!punto || punto == nombre) return "";
    return punto;
}

// --- FUNCIÓN PRINCIPAL DE VALIDACIÓN ---
int validar_archivo(const char *ruta_archivo) {
    // 1. Obtener la extensión declarada en el nombre
    const char *ext = obtener_extension(ruta_archivo);

    // 2. Abrir el archivo en modo de lectura binaria ("rb")
    FILE *archivo = fopen(ruta_archivo, "rb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir o no existe el archivo '%s'.\n", ruta_archivo);
        return -1;
    }

    // Reservamos un arreglo para leer los primeros 8 bytes (firma o magic bytes)
    unsigned char bytes[8];
    size_t leidos = fread(bytes, sizeof(unsigned char), 8, archivo);
    fclose(archivo);

    // Si el archivo está vacío o tiene menos de 2 bytes, no es una imagen válida
    if (leidos < 2) {
        printf("Archivo corrupto o demasiado pequeño.\n");
        return 0;
    }

    // 3. Comparar la extensión con los bytes reales del encabezado

    // VALIDACIÓN PARA BMP
    if (strcasecmp(ext, ".bmp") == 0) {
        // Un archivo BMP real siempre debe empezar con los caracteres 'B' y 'M'
        // 'B' en hexadecimal es 0x42 y 'M' es 0x4D
        if (bytes[0] == 0x42 && bytes[1] == 0x4D) {
            return 1; // Contenido válido
        }
    }

    // VALIDACIÓN PARA PNG
    else if (strcasecmp(ext, ".png") == 0) {
        // Un PNG real siempre empieza con los bytes fijos: 89 50 4E 47 0D 0A 1A 0A
        if (leidos >= 8 &&
            bytes[0] == 0x89 && bytes[1] == 0x50 && bytes[2] == 0x4E && bytes[3] == 0x47 &&
            bytes[4] == 0x0D && bytes[5] == 0x0A && bytes[6] == 0x1A && bytes[7] == 0x0A) {
            return 1; // Contenido válido
        }
    }

    // VALIDACIÓN PARA JPG / JPEG
    else if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0) {
        // Un JPG real siempre empieza con la secuencia de bytes: FF D8 FF
        if (leidos >= 3 && bytes[0] == 0xFF && bytes[1] == 0xD8 && bytes[2] == 0xFF) {
            return 1; // Contenido válido
        }
    }

    // Si la extensión no es ninguna de las anteriores
    else {
        printf("Extensión '%s' no soportada por este validador.\n", ext);
        return -2;
    }

    return 0; // La extensión no coincide con la firma real del contenido
}

int main() {
    char nombre_archivo[256];

    printf("==================================================\n");
    printf("        VALIDADOR DE CONTENIDO REAL          \n");
    printf("==================================================\n");
    printf("Escribe el nombre del archivo a validar (ej: foto.png): ");

    // Leer el nombre del archivo ingresado por el usuario
    if (scanf("%255s", nombre_archivo) != 1) {
        return 1;
    }

    printf("\nAnalizando archivo '%s'...\n", nombre_archivo);
    int resultado = validar_archivo(nombre_archivo);

    if (resultado == 1) {
        printf("¡VALIDACION EXITOSA!\n");
        printf("El archivo tiene la extension correcta y su estructura interna (firma binaria) es VALIDA.\n");
    }
    else if (resultado == 0) {
        printf("¡ALERTA DE SEGURIDAD O ERROR!\n");
        printf("La extension del archivo NO coincide con su contenido real.\n");
        printf("El contenido interno indica que es falso o está corrupto.\n");
    }

    return 0;
}
