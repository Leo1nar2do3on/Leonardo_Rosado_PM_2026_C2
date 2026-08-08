#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nombre[50];
    float costo_compra;
    float valor_residual;
    float vida_util_anios;
    float km_vida_util;
    float consumo_ciudad;       // Rendimiento: Km / Galón
    float consumo_autopista;    // Rendimiento: Km / Galón
    float seguro_anual;
    float impuestos_anuales;
    float mantenimiento_total;
    float neumaticos_total;
    float margen_imprevistos;   // Porcentaje (ej. 5 para 5%)
} Vehiculo;

const char *ARCHIVO_VEHICULOS = "vehiculos.txt";
const char *ARCHIVO_CONFIG = "config.txt";

float obtener_precio_combustible() {
    FILE *f = fopen(ARCHIVO_CONFIG, "r");
    float precio = 290.00; // Valor por defecto en RD$
    if (f != NULL) {
        fscanf(f, "%f", &precio);
        fclose(f);
    }
    return precio;
}

void guardar_precio_combustible(float precio) {
    FILE *f = fopen(ARCHIVO_CONFIG, "w");
    if (f != NULL) {
        fprintf(f, "%.2f\n", precio);
        fclose(f);
    }
}

// Opción 2: Listar todos los vehículos
void listar_vehiculos() {
    FILE *f = fopen(ARCHIVO_VEHICULOS, "r");
    if (!f) {
        printf("\nNo hay vehiculos registrados o el archivo no existe.\n");
        return;
    }

    Vehiculo v;
    int contador = 0;

    printf("\n=================================================================================\n");
    printf("                           LISTA DE VEHICULOS REGISTRADOS                         \n");
    printf("=================================================================================\n");
    printf("%-20s | %-14s | %-12s | %-10s | %-10s\n", "Nombre", "Costo (RD$)", "Km Vida U.", "Rend. Ciu.", "Rend. Aut.");
    printf("---------------------------------------------------------------------------------\n");

    // Se usa %[^\n] para leer nombres con espacios guardados en el archivo
    while (fscanf(f, " %[^\n,],%f %f %f %f %f %f %f %f %f %f %f",
                  v.nombre, &v.costo_compra, &v.valor_residual, &v.vida_util_anios,
                  &v.km_vida_util, &v.consumo_ciudad, &v.consumo_autopista,
                  &v.seguro_anual, &v.impuestos_anuales, &v.mantenimiento_total,
                  &v.neumaticos_total, &v.margen_imprevistos) != EOF) {

        printf("%-20s | RD$%-11.2f | %-12.0f | %-7.1f Km/G | %-7.1f Km/G\n",
               v.nombre, v.costo_compra, v.km_vida_util, v.consumo_ciudad, v.consumo_autopista);
        contador++;
    }

    fclose(f);

    if (contador == 0) {
        printf("El archivo esta vacio. Registre un vehiculo con la opcion 1.\n");
    } else {
        printf("---------------------------------------------------------------------------------\n");
        printf("Total de vehiculos registrados: %d\n", contador);
    }
    printf("=================================================================================\n");
}

// Opción 1: Crear vehículo
void crear_vehiculo() {
    FILE *f = fopen(ARCHIVO_VEHICULOS, "a");
    if (!f) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    Vehiculo v;
    printf("\n--- NUEVO VEHICULO ---\n");
    printf("Nombre/Modelo (se permiten espacios): ");
    scanf(" %[^\n]", v.nombre); // Permite leer espacios hasta presionar Enter

    printf("Costo de compra (RD$): ");
    scanf("%f", &v.costo_compra);

    printf("Valor residual estimado de reventa (RD$): ");
    scanf("%f", &v.valor_residual);

    do {
        printf("Vida util estimada en anios (mayor a 0): ");
        scanf("%f", &v.vida_util_anios);
    } while (v.vida_util_anios <= 0);

    do {
        printf("Km totales esperados en vida util (mayor a 0): ");
        scanf("%f", &v.km_vida_util);
    } while (v.km_vida_util <= 0);

    do {
        printf("Rendimiento en ciudad (Km por Galon, ej. 35): ");
        scanf("%f", &v.consumo_ciudad);
    } while (v.consumo_ciudad <= 0);

    do {
        printf("Rendimiento en autopista (Km por Galon, ej. 50): ");
        scanf("%f", &v.consumo_autopista);
    } while (v.consumo_autopista <= 0);

    printf("Seguro anual (RD$): ");
    scanf("%f", &v.seguro_anual);

    printf("Impuestos/Licencias anuales (RD$): ");
    scanf("%f", &v.impuestos_anuales);

    printf("Gasto estimado de mantenimiento en vida util (RD$): ");
    scanf("%f", &v.mantenimiento_total);

    printf("Gasto estimado en neumaticos en vida util (RD$): ");
    scanf("%f", &v.neumaticos_total);

    do {
        printf("Margen para imprevistos (%%, entre 0 y 100): ");
        scanf("%f", &v.margen_imprevistos);
    } while (v.margen_imprevistos < 0 || v.margen_imprevistos > 100);

    // Guardamos el nombre seguido de una coma para separarlo limpiamente de los números en el archivo
    fprintf(f, "%s,%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
            v.nombre, v.costo_compra, v.valor_residual, v.vida_util_anios,
            v.km_vida_util, v.consumo_ciudad, v.consumo_autopista,
            v.seguro_anual, v.impuestos_anuales, v.mantenimiento_total,
            v.neumaticos_total, v.margen_imprevistos);

    fclose(f);
    printf("\n¡Vehiculo '%s' guardado exitosamente!\n", v.nombre);
}

// Opción 3: Borrar vehículo
void borrar_vehiculo() {
    FILE *f = fopen(ARCHIVO_VEHICULOS, "r");
    if (!f) {
        printf("No hay vehiculos registrados.\n");
        return;
    }

    char nombre_borrar[50];
    printf("Nombre del vehiculo a borrar: ");
    scanf(" %[^\n]", nombre_borrar);

    FILE *temp = fopen("temp.txt", "w");
    Vehiculo v;
    int encontrado = 0;

    while (fscanf(f, " %[^\n,],%f %f %f %f %f %f %f %f %f %f %f",
                  v.nombre, &v.costo_compra, &v.valor_residual, &v.vida_util_anios,
                  &v.km_vida_util, &v.consumo_ciudad, &v.consumo_autopista,
                  &v.seguro_anual, &v.impuestos_anuales, &v.mantenimiento_total,
                  &v.neumaticos_total, &v.margen_imprevistos) != EOF) {

        if (strcasecmp(v.nombre, nombre_borrar) == 0) {
            encontrado = 1;
        } else {
            fprintf(temp, "%s,%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                    v.nombre, v.costo_compra, v.valor_residual, v.vida_util_anios,
                    v.km_vida_util, v.consumo_ciudad, v.consumo_autopista,
                    v.seguro_anual, v.impuestos_anuales, v.mantenimiento_total,
                    v.neumaticos_total, v.margen_imprevistos);
        }
    }

    fclose(f);
    fclose(temp);
    remove(ARCHIVO_VEHICULOS);
    rename("temp.txt", ARCHIVO_VEHICULOS);

    if (encontrado) printf("\nVehiculo eliminado con exito.\n");
    else printf("\nVehiculo no encontrado.\n");
}

// Opción 4: Modificar vehículo
void modificar_vehiculo() {
    FILE *f = fopen(ARCHIVO_VEHICULOS, "r");
    if (!f) {
        printf("\nNo hay vehiculos registrados.\n");
        return;
    }

    char nombre_mod[50];
    printf("\nNombre del vehiculo a modificar: ");
    scanf(" %[^\n]", nombre_mod);

    FILE *temp = fopen("temp.txt", "w");
    Vehiculo v;
    int encontrado = 0;

    while (fscanf(f, " %[^\n,],%f %f %f %f %f %f %f %f %f %f %f",
                  v.nombre, &v.costo_compra, &v.valor_residual, &v.vida_util_anios,
                  &v.km_vida_util, &v.consumo_ciudad, &v.consumo_autopista,
                  &v.seguro_anual, &v.impuestos_anuales, &v.mantenimiento_total,
                  &v.neumaticos_total, &v.margen_imprevistos) != EOF) {

        if (strcasecmp(v.nombre, nombre_mod) == 0) {
            encontrado = 1;
            printf("\n--- INGRESAR NUEVOS PARAMETROS PARA %s ---\n", v.nombre);

            printf("Costo de compra (RD$): ");
            scanf("%f", &v.costo_compra);

            printf("Valor residual estimado de reventa (RD$): ");
            scanf("%f", &v.valor_residual);

            do {
                printf("Vida util estimada en anios (mayor a 0): ");
                scanf("%f", &v.vida_util_anios);
            } while (v.vida_util_anios <= 0);

            do {
                printf("Km totales esperados en vida util (mayor a 0): ");
                scanf("%f", &v.km_vida_util);
            } while (v.km_vida_util <= 0);

            do {
                printf("Rendimiento en ciudad (Km por Galon, ej. 35): ");
                scanf("%f", &v.consumo_ciudad);
            } while (v.consumo_ciudad <= 0);

            do {
                printf("Rendimiento en autopista (Km por Galon, ej. 50): ");
                scanf("%f", &v.consumo_autopista);
            } while (v.consumo_autopista <= 0);

            printf("Seguro anual (RD$): ");
            scanf("%f", &v.seguro_anual);

            printf("Impuestos/Licencias anuales (RD$): ");
            scanf("%f", &v.impuestos_anuales);

            printf("Gasto estimado de mantenimiento en vida util (RD$): ");
            scanf("%f", &v.mantenimiento_total);

            printf("Gasto estimado en neumaticos en vida util (RD$): ");
            scanf("%f", &v.neumaticos_total);

            do {
                printf("Margen para imprevistos (%%, entre 0 y 100): ");
                scanf("%f", &v.margen_imprevistos);
            } while (v.margen_imprevistos < 0 || v.margen_imprevistos > 100);
        }

        fprintf(temp, "%s,%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                v.nombre, v.costo_compra, v.valor_residual, v.vida_util_anios,
                v.km_vida_util, v.consumo_ciudad, v.consumo_autopista,
                v.seguro_anual, v.impuestos_anuales, v.mantenimiento_total,
                v.neumaticos_total, v.margen_imprevistos);
    }

    fclose(f);
    fclose(temp);
    remove(ARCHIVO_VEHICULOS);
    rename("temp.txt", ARCHIVO_VEHICULOS);

    if (encontrado) {
        printf("\n¡Vehiculo '%s' modificado exitosamente!\n", nombre_mod);
    } else {
        printf("\nVehiculo no encontrado.\n");
    }
}

// Opción 5: Calcular viaje
void calcular_viaje() {
    FILE *f = fopen(ARCHIVO_VEHICULOS, "r");
    if (!f) {
        printf("No hay vehiculos registrados.\n");
        return;
    }

    char nombre_busca[50];
    printf("Nombre del vehiculo a utilizar: ");
    scanf(" %[^\n]", nombre_busca);

    Vehiculo v;
    int encontrado = 0;

    while (fscanf(f, " %[^\n,],%f %f %f %f %f %f %f %f %f %f %f",
                  v.nombre, &v.costo_compra, &v.valor_residual, &v.vida_util_anios,
                  &v.km_vida_util, &v.consumo_ciudad, &v.consumo_autopista,
                  &v.seguro_anual, &v.impuestos_anuales, &v.mantenimiento_total,
                  &v.neumaticos_total, &v.margen_imprevistos) != EOF) {

        if (strcasecmp(v.nombre, nombre_busca) == 0) {
            encontrado = 1;
            break;
        }
    }
    fclose(f);

    if (!encontrado) {
        printf("Vehiculo no encontrado.\n");
        return;
    }

    float km_ciudad, km_autopista;
    printf("Kilometros a recorrer en ciudad: ");
    scanf("%f", &km_ciudad);
    printf("Kilometros a recorrer en autopista: ");
    scanf("%f", &km_autopista);

    float km_totales = km_ciudad + km_autopista;
    if (km_totales <= 0) {
        printf("Los kilometros recorridos deben ser mayores a 0.\n");
        return;
    }

    float precio_galon = obtener_precio_combustible();

    // 1. Galones consumidos
    float galones_ciudad = (v.consumo_ciudad > 0) ? (km_ciudad / v.consumo_ciudad) : 0;
    float galones_autopista = (v.consumo_autopista > 0) ? (km_autopista / v.consumo_autopista) : 0;
    float galones_totales = galones_ciudad + galones_autopista;
    float costo_combustible_total = galones_totales * precio_galon;

    // 2. Amortización
    float costo_depreciable = v.costo_compra - v.valor_residual;
    float amortizacion_km = costo_depreciable / v.km_vida_util;

    // 3. Mantenimiento y neumáticos
    float mantenimiento_km = v.mantenimiento_total / v.km_vida_util;
    float neumaticos_km = v.neumaticos_total / v.km_vida_util;

    // 4. Costos fijos
    float km_anuales = v.km_vida_util / v.vida_util_anios;
    float costos_fijos_km = (v.seguro_anual + v.impuestos_anuales) / km_anuales;

    // Desgaste base por km
    float desgaste_base_km = amortizacion_km + mantenimiento_km + neumaticos_km + costos_fijos_km;

    // Aplicar porcentaje de imprevistos
    float desgaste_real_km = desgaste_base_km * (1.0 + (v.margen_imprevistos / 100.0));
    float costo_desgaste_viaje = desgaste_real_km * km_totales;

    // Totales
    float costo_total_viaje = costo_combustible_total + costo_desgaste_viaje;
    float costo_km_ciudad = (v.consumo_ciudad > 0) ? ((precio_galon / v.consumo_ciudad) + desgaste_real_km) : desgaste_real_km;
    float costo_km_autopista = (v.consumo_autopista > 0) ? ((precio_galon / v.consumo_autopista) + desgaste_real_km) : desgaste_real_km;

    printf("\n=============================================\n");
    printf("        RESUMEN DEL VIAJE: %s\n", v.nombre);
    printf("=============================================\n");
    printf(" Combustible consumido:         %.2f Gal. (RD$%.2f)\n", galones_totales, costo_combustible_total);
    printf(" Desgaste, amortizacion e imp.: RD$%.2f\n", costo_desgaste_viaje);
    printf(" COSTO TOTAL REAL DEL VIAJE:    RD$%.2f\n", costo_total_viaje);
    printf("---------------------------------------------\n");
    printf(" Costo real promedio por km:    RD$%.2f/km\n", costo_total_viaje / km_totales);
    printf(" Costo especifico km ciudad:    RD$%.2f/km\n", costo_km_ciudad);
    printf(" Costo especifico km autopista: RD$%.2f/km\n", costo_km_autopista);
    printf("=============================================\n");
}

// Opción 6: Modificar precio de combustible
void modificar_combustible() {
    float actual = obtener_precio_combustible();
    float nuevo;
    printf("Precio actual por galon: RD$%.2f. Nuevo precio por galon (RD$): ", actual);
    scanf("%f", &nuevo);
    guardar_precio_combustible(nuevo);
    printf("Precio del combustible actualizado a RD$%.2f por galon\n", nuevo);
}

int main() {
    int opcion;
    do {
        printf("\n--- GESTOR DE COSTOS DE VEHICULO (C) ---\n");
        printf("1. Crear vehiculo\n");
        printf("2. Listar vehiculos\n");
        printf("3. Borrar vehiculo\n");
        printf("4. Cambiar parametros de vehiculo\n");
        printf("5. Calcular viaje\n");
        printf("6. Modificar precio de combustible\n");
        printf("7. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: crear_vehiculo(); break;
            case 2: listar_vehiculos(); break;
            case 3: borrar_vehiculo(); break;
            case 4: modificar_vehiculo(); break;
            case 5: calcular_viaje(); break;
            case 6: modificar_combustible(); break;
            case 7: printf("Saliendo...\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 7);

    return 0;
}
