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

    DIR *directorio;
    struct dirent *entrada;
        
    if ((directorio = opendir(ruta)) != NULL) {
        int i = 1;
        while ((entrada = readdir(directorio)) != NULL) {
            if (entrada->d_type == DT_DIR && strcmp(entrada->d_name, ".") != 0 && strcmp(entrada->d_name, "..") != 0) {
                printf("%d) %s\n", i++, entrada->d_name);
            }
        }
        closedir(directorio);
    } else {
        perror("Error al entrar al directorio");
    }
}

void redireccion(struct DirectorioRoot *directorioRoot, const char *nombreDirectorio) {
    char nuevaRuta[256];
    snprintf(nuevaRuta, sizeof(nuevaRuta), "%s/%s", directorioRoot->ruta, nombreDirectorio);

    if (chdir(nuevaRuta) == 0) {
        if (getcwd(directorioRoot->ruta, sizeof(directorioRoot->ruta)) != NULL) {
            printf("Cambiado a: %s\n", directorioRoot->ruta);
        } else {
            perror("Error a la hora de conseguir la ruta");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("chdir() error");
    }
}

void cambiarDirectorio(struct DirectorioRoot *directorioRoot) {
    mostrarDirectorios(directorioRoot->ruta);
    int opcion;
    printf("Seleccione un directorio por su número (0 para retroceder en un directorio): ");
    scanf("%d", &opcion);
    getchar();  // Limpiar el buffer de entrada

    if (opcion == 0) {
        // Evitar retroceder más allá de la carpeta "root"
        if (strcmp(directorioRoot->ruta, "/home/happy8/Documents/Universidad/Sistema-archivos/root") != 0) {
            if (chdir("..") == 0) {
                if (getcwd(directorioRoot->ruta, sizeof(directorioRoot->ruta)) != NULL) {
                    printf("El directorio se cambio a: %s\n", directorioRoot->ruta);
                } else {
                    perror("Error a la hora de conseguir la ruta");
                    exit(EXIT_FAILURE);
                }
            } else {
                perror("Error en el cambio de ruta");
            }
        } else {
            printf("No se puede retroceder más allá de la carpeta root.\n");
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
// Manejo de directorios
void eliminarDirectorioActual(struct DirectorioRoot *directorioRoot) {
    char directorioActual[256];
    if (getcwd(directorioActual, sizeof(directorioActual)) != NULL) {
        // Comprobar si el directorio actual no es el directorio "root"
            if (rmdir(directorioActual) == 0) {
                printf("El Directorio '%s' se elimino correctamente.\n", directorioActual);

                // Mover de nuevo al directorio padre
                if (chdir("..") == 0) {
                    if (getcwd(directorioRoot->ruta, sizeof(directorioRoot->ruta)) != NULL) {
                        printf("Cambiado a: %s\n", directorioRoot->ruta);
                    } else {
                        perror("getcwd() error");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    perror("chdir() error");
                    exit(EXIT_FAILURE);
                }
            } else {
                perror("Error al eliminar el directorio");
            }
    } else {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

void crearNuevoDirectorio(struct DirectorioRoot *directorioRoot) {
    char nuevoDirectorio[100];
    printf("Ingrese el nombre del nuevo directorio: ");
    scanf("%s", nuevoDirectorio);
    getchar();  // Limpiar el buffer de entrada

    char nuevaRuta[256];
    snprintf(nuevaRuta, sizeof(nuevaRuta), "%s/%s", directorioRoot->ruta, nuevoDirectorio);

    if (mkdir(nuevaRuta, 0777) == 0) {
        printf("Directorio '%s' fue creado correctamente.\n", nuevoDirectorio);
    } else {
        perror("Error al crear el directorio");
    }
}
// Manejo de directorios


int main() {
    // Especifica la ruta absoluta al directorio "root"
    const char *rootDirectorio = "/home/happy8/Documents/Universidad/Sistema-archivos/root";
    //Comprueba si el directorio root existe, sino lo crea
    if (mkdir(rootDirectorio, 0777) != 0 && errno != EEXIST) { 
        fprintf(stderr, "Error al crear el directorio '%s'\n", rootDirectorio);
        return 1;
    }
    //incializacion de estructurassnprintf
    struct DirectorioRoot directorioRoot;
    strcpy(directorioRoot.ruta, rootDirectorio);

    //incializacion de estructurassnprintf


    int opcion;
    do {
        printf("===================Sistema de archivos====================\n");
        printf("--------------------Manejo de archivos---------------------\n");
        //printf("1. Crear archivo y guardar en disco\n");
        printf("--------------------Manejo de archivos---------------------\n");
        printf("\n");        
        printf("------------------Manejo de directorios------------------\n");
        printf("4. Cambiar de directorio\n");
        printf("5. Crear nuevo directorio\n");
        printf("6. Eliminar directorio actual\n");
        printf("------------------Manejo de directorios------------------\n");
        printf("7. Salir\n");
        printf("===================Sistema de archivos====================\n");
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
                eliminarDirectorioActual(&directorioRoot);
                break;
            case 7:
                printf("Saliendo del programa.\n");
                break;
                

            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }

    } while (opcion != 7);

    return 0;
}
