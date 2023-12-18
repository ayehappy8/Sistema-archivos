#include <stdio.h>

int main(){
    int menu;
    printf("Bienvenido al gestor de archivos");

    while(menu!=10){

        printf("\n1) Crear un archivo");
        printf("\n2) Renombrar un archivo");
        printf("\n3) Escribir en un archivo");
        printf("\n4) Re-escribir en un archivo");
        printf("\n5) Borrar un archivo");
        printf("\n6) Ayuda \n10) Salir");
        printf("\nEscoja una opcion: ");
        scanf("%d",&menu);

        switch (menu)
        {
            //Crear un file
        case 1:
            printf("Ingrese un nombre para el archivo:");
            
            break;

            //Renombrar un file
        case 2:
            printf("Ingrese el nombre del archivo al que quiere cambiar el nombre:");
            printf("\nIngrese el nuevo nombre para el archivo:");

            break;

            //Escribir en un file
        case 3:
            printf("Ingrese el archivo en el que quiere escribir:");
            printf("\nIngrese lo que quiere escribir:");

            break;

            //Re escribir en un file
        case 4:
            printf("Ingrese el archivo que quiere re-escribir:");
            printf("\nIngrese lo que quiere escribir:");

            break;
        
            //Borrar un file
        case 5:
            printf("Ingrese el nombre del archivo que quiere eliminar:");
            break;

            //Mostrar los detalles al usuario
        case 6:
            printf("\n1) Crear un archivo: Crea un archivo txt en blanco con el nombre que se le quiera asignar \n2) Renombrar un archivo: Permite cambiarle el nombre a un archivo");
            printf("\n3) Escribir en un archivo: Añade el texto que se quiera a un archivo \n4) Re-escribir en un archivo: Borra lo que tenga el archivo y le escribe texto nuevo");
            printf("\n5) Borrar un archivo: Elimina un archivo con su nombre \n6) Ayuda: Muestra informacion de que hacen los comandos \n10) Salir: Para salir del programa\n");

            break;

            //Salir del programa
        case 10:
            printf("Saliendo...");
            break;


        default:
            printf("Opcion invalida");
            break;
        }        
    }
    
    return 0;
}