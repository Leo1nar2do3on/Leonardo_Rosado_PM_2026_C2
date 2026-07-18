#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Configuración e inclusión de las librerías STB para procesamiento de imágenes
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX_ARCHIVOS 100
#define MAX_NOMBRE 256

// Función para obtener la extensión de un archivo (ej: ".png")
const char *obtener_extension(const char *nombre_archivo) {
    const char *punto = strrchr(nombre_archivo, '.');
    if (!punto || punto == nombre_archivo) return "";
    return punto;
}

// Función encargada de transformar los píxeles a escala de grises
void procesar_a_blanco_y_negro(const char *entrada, const char *salida, const char *ext) {
    int ancho, alto, canales;

    // 1. Cargar los píxeles de la imagen original en memoria dinámica
    // canales indicará si es RGB (3 bytes por píxel) o RGBA (4 bytes por píxel)
    unsigned char *pixeles_originales = stbi_load(entrada, &ancho, &alto, &canales, 0);

    if (!pixeles_originales) {
        printf("Error: No se pudieron leer los datos de la imagen '%s'.\n", entrada);
        return;
    }

    printf("-> Dimensiones: %dx%d | Canales de color: %d\n", ancho, alto, canales);

    // 2. Asignar memoria dinámica para la nueva imagen (1 canal de color = escala de grises)
    unsigned char *pixeles_gris = (unsigned char *) malloc(ancho * alto * 1);
    if (!pixeles_gris) {
        printf("Error: Memoria RAM insuficiente para el procesamiento.\n");
        stbi_image_free(pixeles_originales);
        return;
    }

    // 3. Conversión píxel por píxel usando la fórmula de luminancia ITU-R BT.601
    // Y = 0.299*R + 0.587*G + 0.114*B
    for (int i = 0; i < ancho * alto; i++) {
        int indice_rgb = i * canales;
        unsigned char r = pixeles_originales[indice_rgb];
        unsigned char g = pixeles_originales[indice_rgb + 1];
        unsigned char b = pixeles_originales[indice_rgb + 2];

        // Calculamos el valor gris promedio ponderado
        pixeles_gris[i] = (unsigned char)(0.299f * r + 0.587f * g + 0.114f * b);
    }

    // 4. Guardar el nuevo bloque de píxeles en el disco con su formato correspondiente
    int resultado = 0;
    if (strcasecmp(ext, ".png") == 0) {
        resultado = stbi_write_png(salida, ancho, alto, 1, pixeles_gris, ancho);
    } else if (strcasecmp(ext, ".bmp") == 0) {
        resultado = stbi_write_bmp(salida, ancho, alto, 1, pixeles_gris);
    } else if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0) {
        resultado = stbi_write_jpg(salida, ancho, alto, 1, pixeles_gris, 90); // Calidad al 90%
    }

    if (resultado) {
        printf("¡Éxito! Imagen convertida y guardada como: %s\n", salida);
    } else {
        printf("Error: No se pudo escribir el archivo de salida en el disco.\n");
    }

    // 5. Liberación estricta de la memoria dinámica utilizada
    stbi_image_free(pixeles_originales);
    free(pixeles_gris);
}

int main() {
    DIR *dir;
    struct dirent *entrada_dir;
    char lista_archivos[MAX_ARCHIVOS][MAX_NOMBRE];
    int total_imagenes = 0;

    // Abrir el directorio actual "." donde corre el ejecutable
    dir = opendir(".");
    if (dir == NULL) {
        printf("Error: No se pudo abrir la carpeta actual.\n");
        return 1;
    }

    // 1. Escanear la carpeta y listar los formatos compatibles
    while ((entrada_dir = readdir(dir)) != NULL) {
        const char *ext = obtener_extension(entrada_dir->d_name);

        if (strcasecmp(ext, ".png") == 0 ||
            strcasecmp(ext, ".jpg") == 0 ||
            strcasecmp(ext, ".jpeg") == 0 ||
            strcasecmp(ext, ".bmp") == 0) {

            strncpy(lista_archivos[total_imagenes], entrada_dir->d_name, MAX_NOMBRE);
            total_imagenes++;

            if (total_imagenes >= MAX_ARCHIVOS) break;
        }
    }
    closedir(dir);

    // Validar si existen imágenes en el directorio
    if (total_imagenes == 0) {
        printf("No se encontraron archivos .png, .jpg o .bmp en la carpeta actual.\n");
        printf("Por favor, coloca imágenes en este directorio y vuelve a iniciar.\n");
        return 0;
    }

    // 2. Desplegar el menú interactivo en consola
    printf("====================================================\n");
    printf("        CONVERSOR DE IMÁGENES A BLANCO Y NEGRO      \n");
    printf("====================================================\n");
    printf("Selecciona el archivo que deseas procesar:\n\n");

    for (int i = 0; i < total_imagenes; i++) {
        printf("[%d] %s\n", i + 1, lista_archivos[i]);
    }
    printf("[%d] Salir del programa\n", total_imagenes + 1);
    printf("\nOpción seleccionada: ");

    int opcion;
    if (scanf("%d", &opcion) != 1) {
        printf("Entrada inválida.\n");
        return 1;
    }

    // Evaluar la opción ingresada
    if (opcion == total_imagenes + 1) {
        printf("Cerrando el programa.\n");
        return 0;
    }

    if (opcion < 1 || opcion > total_imagenes) {
        printf("Opción inválida fuera de rango.\n");
        return 1;
    }

    // 3. Preparar nombres de archivos y ejecutar conversión
    int indice = opcion - 1;
    char archivo_entrada[MAX_NOMBRE];
    char archivo_salida[MAX_NOMBRE + 10];

    strncpy(archivo_entrada, lista_archivos[indice], MAX_NOMBRE);

    // El archivo resultante se guardará con el prefijo "gris_"
    sprintf(archivo_salida, "gris_%s", archivo_entrada);
    const char *ext_seleccionada = obtener_extension(archivo_entrada);

    printf("\nProcesando archivo: %s...\n", archivo_entrada);
    procesar_a_blanco_y_negro(archivo_entrada, archivo_salida, ext_seleccionada);

    return 0;
}
