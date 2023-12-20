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

void mostrarArchivosEnDirectorio(const char *ruta, const char *extension) {
    printf("Archivos disponibles en '%s' con extensión '%s':\n", ruta, extension);

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(ruta)) != NULL) {
        int i = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                // Verificar si el archivo tiene la extensión deseada
                const char *dot = strrchr(ent->d_name, '.');
                if (dot && strcmp(dot, extension) == 0) {
                    printf("%d. [Archivo] %s\n", i++, ent->d_name);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio");
    }
}

void mostrarDirectoriosDisponibles(const char *ruta) {
    printf("Directorios disponibles en '%s':\n", ruta);

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(ruta)) != NULL) {
        int i = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                printf("%d. %s\n", i++, ent->d_name);
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
    mostrarDirectoriosDisponibles(directorioRoot->ruta);
    int opcion;
    printf("Seleccione un directorio por su número (0 para volver atrás): ");
    scanf("%d", &opcion);
    getchar();  // Limpiar el buffer de entrada

    if (opcion == 0) {
        // Evitar retroceder más allá de la carpeta "root"
        if (strcmp(directorioRoot->ruta, "root") != 0) {
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
        struct dirent *ent;

        if ((dir = opendir(directorioRoot->ruta)) != NULL) {
            int i = 1;
            while ((ent = readdir(dir)) != NULL) {
                if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                    if (i == opcion) {
                        redireccion(directorioRoot, ent->d_name);
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
        printf("4. Mostrar archivos de texto\n");
        printf("5. Cambiar directorio\n");
        printf("6. Salir\n");
        printf("-------------------\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar();  // Limpiar el buffer de entrada

        switch (opcion) {

            case 4:
                mostrarArchivosEnDirectorio(directorioRoot.ruta, "txt");
                break;

            case 5:
                cambiarDirectorio(&directorioRoot);
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
