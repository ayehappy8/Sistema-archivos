#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

struct Archivo {
    char ruta[256];  // Ruta completa del archivo
    char contenido[1000];
};

struct DirectorioRoot {
    char ruta[256];
};
// Manejo de directorios
void mostrarDirectorios(const char *ruta) {
    printf("Directorios disponibles en '%s':\n", ruta);

    DIR *dir;
    struct dirent *entrada;
        
    if ((dir = opendir(ruta)) != NULL) {
        int i = 1;
        while ((entrada = readdir(dir)) != NULL) {
            if (entrada->d_type == DT_DIR && strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                printf("%d) %s\n", i++, entrada->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio");
    }
}

void redireccion(struct DirectorioRoot *directorioRoot, const char *nombreDirectorio) {
    char nuevaRuta[256];
    snprintf(nuevaRuta, sizeof(nuevaRuta), "%s/%s", directorioRoot->ruta, nombreDirectorio);

    if (chdir(nuevaRuta) == 0) {
        if (getcwd(directorioRoot->ruta, sizeof(directorioRoot->ruta)) != NULL) {
            printf("Cambiado a: %s\n", directorioRoot->ruta);
        } else {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("chdir() error");
    }
}

void cambiarDirectorio(struct DirectorioRoot *directorioRoot) {
    mostrarDirectorios(directorioRoot->ruta);
    int opcion;
    printf("Seleccione un directorio por su número (0 para volver atrás): ");
    scanf("%d", &opcion);
    getchar();  // Limpiar el buffer de entrada

    if (opcion == 0) {
        // Evitar retroceder más allá de la carpeta "root"
        if (strcmp(directorioRoot->ruta, "/home/happy8/Documents/Universidad/Sistema-archivos/root") != 0) {
            if (chdir("..") == 0) {
                if (getcwd(directorioRoot->ruta, sizeof(directorioRoot->ruta)) != NULL) {
                    printf("Cambiado a: %s\n", directorioRoot->ruta);
                } else {
                    perror("getcwd() error");
                    exit(EXIT_FAILURE);
                }
            } else {
                perror("chdir() error");
            }
        } else {
            printf("No se puede retroceder más allá de la carpeta 'root'.\n");
        }
    } else {
        DIR *dir;
        struct dirent *entrada;

        if ((dir = opendir(directorioRoot->ruta)) != NULL) {
            int i = 1;
            while ((entrada = readdir(dir)) != NULL) {
                if (entrada->d_type == DT_DIR && strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                    if (i == opcion) {
                        redireccion(directorioRoot, entrada->d_name);
                        break;
                    }
                    i++;
                }
            }
            closedir(dir);
        } else {
            perror("Error al abrir el directorio");
        }
    }
}

void crearNuevoDirectorio(struct DirectorioRoot *directorioRoot) {
    char nombreDirectorioNuevo[100];
    printf("Ingrese el nombre del nuevo directorio: ");
    scanf("%s", nombreDirectorioNuevo);
    getchar();  // Limpiar el buffer de entrada

    char nuevaRuta[256];
    snprintf(nuevaRuta, sizeof(nuevaRuta), "%s/%s", directorioRoot->ruta, nombreDirectorioNuevo);

    if (mkdir(nuevaRuta, 0777) == 0) {
        printf("Directorio '%s' creado correctamente.\n", nombreDirectorioNuevo);
    } else {
        perror("Error al crear el directorio");
    }
}
// Manejo de directorios


int main() {
    // Especifica la ruta absoluta al directorio "root"
    const char *rootDirectorio = "/home/happy8/Documents/Universidad/Sistema-archivos/root";
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
        printf("4. Cambiar directorio\n");
        printf("5. Crear nuevo directorio\n");
        printf("6. Salir\n");
        printf("-------------------\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 4:
                cambiarDirectorio(&directorioRoot);
                break;

            case 5:
                crearNuevoDirectorio(&directorioRoot);
                break;

            case 6:
                printf("Saliendo del programa.\n");
                break;

            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }

    } while (opcion != 6);

    return 0;
}
