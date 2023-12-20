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
// Manejo de archivos
void crearArchivo(struct Archivo *archivo, const char *nombreArchivo,struct DirectorioRoot *directorioRoot){
    snprintf(archivo->ruta, sizeof(archivo->ruta), "%s/%s", directorioRoot->ruta, nombreArchivo);
    //strcpy(archivo->contenido, contenido);

    FILE *file = fopen(archivo->ruta, "w");
    if (file != NULL) {
        //fputs(archivo->contenido, file);
        fclose(file);
        printf("Archivo creado y guardado en disco en la ruta: %s\n", archivo->ruta);
    } else {
        fprintf(stderr, "Error al abrir el archivo '%s' para escritura.\n", archivo->ruta);
    }
}

void escribirArchivo(struct DirectorioRoot *directorioRoot, const char *extension){
    char ruta[256];
    mostrarArchivosEnDirectorio(directorioRoot->ruta, extension);
    int opcion;
    printf("Seleccione el archivo segun su numero:\n");
    scanf("%d", &opcion);
    
    getchar();
    

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directorioRoot->ruta)) != NULL) {
        int i = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                // Verificar si el archivo tiene la extensión deseada
                const char *dot = strrchr(ent->d_name, '.');
                if (dot && strcmp(dot, extension) == 0) {
                    snprintf(ruta, sizeof(ruta), "%s/%s", directorioRoot->ruta, ent->d_name);
                    if (i == opcion)
                    {
                        printf("Archivo Seleccionado: %s\n", ent->d_name);
                        char contenido[1000];
                        printf("Ingrese el texto que dese escribir:\n");
                        scanf("%s", contenido);
                        FILE *file = fopen(ruta, "w");
                        if (file != NULL) {
                            fputs(contenido, file);
                            fclose(file);
                        } else {
                            fprintf(stderr, "Error al abrir el archivo '%s' para escritura.\n", ruta);
                        }
                    }
                    i++;
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio");
    }
}

void leerArchivo(struct DirectorioRoot *directorioRoot, const char *extension){
    char ruta[256];
    mostrarArchivosEnDirectorio(directorioRoot->ruta, extension);
    int opcion;
    printf("Seleccione el archivo segun su numero:\n");
    scanf("%d", &opcion);
    
    getchar();
    

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directorioRoot->ruta)) != NULL) {
        int i = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                // Verificar si el archivo tiene la extensión deseada
                const char *dot = strrchr(ent->d_name, '.');
                if (dot && strcmp(dot, extension) == 0) {
                    snprintf(ruta, sizeof(ruta), "%s/%s", directorioRoot->ruta, ent->d_name);
                    if (i == opcion)
                    {
                        printf("Archivo Seleccionado: %s\n", ent->d_name);
                        char contenido[1000];
                        FILE *file = fopen(ruta, "r");
                        if (file != NULL) {
                            fgets(contenido, sizeof(contenido), file);
                            fclose(file);
                        } else {
                            fprintf(stderr, "Error al abrir el archivo '%s' para escritura.\n", ruta);
                        }
                        printf("El contenido del archivo es: \n%s\n", contenido);
                    }
                    i++;
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio\n");
    }
}

void eliminarArchivo(struct DirectorioRoot *directorioRoot, const char *extension){
    char ruta[256];
    mostrarArchivosEnDirectorio(directorioRoot->ruta, extension);
    int opcion;
    printf("Seleccione el archivo segun su numero:\n");
    scanf("%d", &opcion);
    getchar();
    

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(directorioRoot->ruta)) != NULL) {
        int i = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                // Verificar si el archivo tiene la extensión deseada
                const char *dot = strrchr(ent->d_name, '.');
                if (dot && strcmp(dot, extension) == 0) {
                    snprintf(ruta, sizeof(ruta), "%s/%s", directorioRoot->ruta, ent->d_name);
                    if (i == opcion)
                    {
                        if (remove(ruta)==0)
                        {
                            printf("El archivo fue correctamente eliminado\n");
                        }else
                            printf("El archivo no se pudo eliminar\n");
                    }
                    i++;
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio");
    }
}

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
                    printf("%d. %s\n", i++, ent->d_name);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio\n");
    }
}
// Manejo de archivos

int main() {
    // Especifica la ruta absoluta al directorio "root"
    const char *rootDirectorio = "/home/happy8/Documents/Universidad/Sistema-archivos/root";
    //Comprueba si el directorio root existe, sino lo crea
    if (mkdir(rootDirectorio, 0777) != 0 && errno != EEXIST) { 
        fprintf(stderr, "Error al crear el directorio '%s'\n", rootDirectorio);
        return 1;
    }
    //incializacion de estructuras
    struct DirectorioRoot directorioRoot;
    strcpy(directorioRoot.ruta, rootDirectorio);

    struct Archivo miArchivo;
    //incializacion de estructuras


    int opcion;
    do {
        printf("\n");        
        printf("\n");        
        printf("===================Sistema de archivos====================\n");
        printf("Para cancelar una acción escriba -1\n");
        printf("--------------------Manejo de archivos---------------------\n");
        printf("1. Crear archivo\n");
        printf("2. Escribir en un archivo\n");
        printf("3. Leer un archivo\n");
        printf("4. Eliminar archivo\n");
        printf("5. Mostrar archivos de texto disponible\n");
        printf("--------------------Manejo de archivos---------------------\n");
        printf("\n");        
        printf("------------------Manejo de directorios------------------\n");
        printf("6. Mostrar directorios\n");
        printf("7. Cambiar de directorio\n");
        printf("8. Crear nuevo directorio\n");
        printf("9. Eliminar directorio actual\n");
        printf("------------------Manejo de directorios------------------\n");
        printf("0. Salir\n");
        printf("===================Sistema de archivos====================\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar();  // Limpiar el buffer de entrada

        switch (opcion) {
            case 1:
                printf("Ingrese nombre del Archivo\n");
                char nombreArchivo[100];
                char exten[5] = ".txt";
                scanf("%s", nombreArchivo);
                strcat(nombreArchivo, exten);
                crearArchivo(&miArchivo, nombreArchivo, &directorioRoot);   
                break;

            case 2:
                escribirArchivo(&directorioRoot, ".txt");

                break;

            case 3:
                leerArchivo(&directorioRoot, ".txt");

                break;
            
            case 4:
                eliminarArchivo(&directorioRoot, ".txt");

                break;

            case 5:
                mostrarArchivosEnDirectorio(directorioRoot.ruta, ".txt");
                break;

            case 6:
                printf("\n");
                printf("-------------------------------\n");
                mostrarDirectorios(directorioRoot.ruta);
                printf("-------------------------------\n");        
                break;
            case 7:
                cambiarDirectorio(&directorioRoot);
                break;
            case 8:
                crearNuevoDirectorio(&directorioRoot);
                break;
            case 9:
                eliminarDirectorioActual(&directorioRoot);
                break;
            case 0:
                printf("Saliendo del programa.\n");
                break;
                

            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }

    } while (opcion != 0);

    return 0;
}
