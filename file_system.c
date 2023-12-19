#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct Archivo {
    char ruta[256];  // Ruta completa del archivo
    char contenido[1000];
};

struct DirectorioRoot {
    char ruta[256];
};

void crearArchivoYGuardarEnDisco(struct Archivo *archivo, const char *nombreArchivo, const char *contenido) {
    snprintf(archivo->ruta, sizeof(archivo->ruta), "%s/%s", "root", nombreArchivo);
    strcpy(archivo->contenido, contenido);

    FILE *file = fopen(archivo->ruta, "w");
    if (file != NULL) {
        fputs(archivo->contenido, file);
        fclose(file);
        printf("Archivo creado y guardado en disco en la ruta: %s\n", archivo->ruta);
    } else {
        fprintf(stderr, "Error al abrir el archivo '%s' para escritura.\n", archivo->ruta);
    }
}

void cargarArchivoDesdeDisco(struct Archivo *archivo) {
    FILE *file = fopen(archivo->ruta, "r");
    if (file != NULL) {
        fgets(archivo->contenido, sizeof(archivo->contenido), file);
        fclose(file);
        printf("Contenido del archivo cargado desde disco: %s\n", archivo->contenido);
    } else {
        fprintf(stderr, "Error al abrir el archivo '%s' para lectura.\n", archivo->ruta);
    }
}

void eliminarArchivo(const char *ruta) {
    if (remove(ruta) != 0) {
        fprintf(stderr, "Error al eliminar el archivo '%s'.\n", ruta);
    } else {
        printf("Archivo eliminado.\n");
    }
}

int main() {
    // Crear el directorio "root" si no existe
    // La carpeta se ubica en el output
    const char *rootDirectorio = "root";
    if (mkdir(rootDirectorio, 0777) != 0 && errno != EEXIST) {
        fprintf(stderr, "Error al crear el directorio '%s' o ya existe.\n", rootDirectorio);
        return 1;
    }

    struct DirectorioRoot directorioRoot;
    strcpy(directorioRoot.ruta, rootDirectorio);

    struct Archivo miArchivo;

    int opcion;
    do {
        printf("------ Menu ------\n");
        printf("1. Crear archivo y guardar en disco\n");
        printf("2. Cargar archivo desde disco\n");
        printf("3. Eliminar archivo\n");
        printf("4. Salir\n");
        printf("-------------------\n");
        printf("Seleccione una opción: ");
        
        scanf("%d", &opcion);
        getchar();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                printf("Ingrese el nombre del archivo: ");
                char nombreArchivo[100];
                scanf("%s", nombreArchivo);
                printf("Ingrese el contenido del archivo: ");
                scanf("%s", miArchivo.contenido);
                crearArchivoYGuardarEnDisco(&miArchivo, nombreArchivo, miArchivo.contenido);
                break;

            case 2:
                printf("Ingrese el nombre del archivo a cargar: ");
                char nombreArchivoCargar[100];
                scanf("%s", nombreArchivoCargar);
                snprintf(miArchivo.ruta, sizeof(miArchivo.ruta), "%s/%s", "root", nombreArchivoCargar);
                cargarArchivoDesdeDisco(&miArchivo);
                break;

            case 3:
                printf("Ingrese el nombre del archivo a eliminar: ");
                char nombreArchivoEliminar[100];
                scanf("%s", nombreArchivoEliminar);
                snprintf(miArchivo.ruta, sizeof(miArchivo.ruta), "%s/%s", "root", nombreArchivoEliminar);
                eliminarArchivo(miArchivo.ruta);
                break;

            case 4:
                printf("Saliendo del programa.\n");
                break;

            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }

    } while (opcion != 4);

    return 0;
}
