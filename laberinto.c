#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// 0 es muro, 1 es espacio vacio, 2 es camino correcto, 3 es entrada, 4 salida
void imprimir_laberinto(int dimension, int **matriz){

    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            switch (matriz[i][j])
            {
            case 0:
                printf("#");
                break;
            case 1:
                printf(" ");
                break;
            case 2:
                printf("*");
                break;
            case 3:
                printf("O");
                break;
            case 4:
                printf("X");
                break;
            // solo para debug, no deberia suceder
            default:
                printf("&");
                break;
            }
        }
        printf("\n");
    }
}


void liberar_laberinto(int dimension, int **matriz){
    for (int i = 0; i < dimension; i++){
        free(matriz[i]);
    }
    free(matriz);
}


int **inicializar_laberinto(int dimension){
    int **matriz = malloc(dimension * sizeof(int *));
    if (!matriz) {
        return NULL;
    }
    for (int i = 0; i < dimension; i++){
        matriz[i] = calloc(dimension, sizeof(int));
        if(!matriz[i]){
            liberar_laberinto(i, matriz);
            return NULL;
        }
    }
    return matriz;
}



int main(int argc, char *argv[]) {

    srand(time(NULL));
    int dimension = 11;

    if (argc > 1){
        dimension = atoi(argv[1]);
        if (dimension <= 0 || dimension % 2 == 0){
            printf("opcion invalida %s \n Ingrese un numero impar", argv[1]);
            return 1;
        }
    }
    // creamos un puntero que apunta a una lista de punteros
    // cada puntero apuntaria a un array, que representan los valores de una fila de la matriz
    // inicializamos el laberinto con todos los valores = 0
    int **laberinto = inicializar_laberinto(dimension);
    // si nuestra funcion de inicializar_laberinto nos brinda un pointer NULL, print error
    if (!laberinto){
        printf("Error al asignar memoria\n");
        return 1;
    }

    // definimos entrada y salida
    laberinto[1][0] = 3;
    laberinto[dimension - 2][dimension - 1] = 4;

    // laberinto = generar_laberinto(dimesion, laberinto);


    imprimir_laberinto(dimension, laberinto);
    liberar_laberinto(dimension, laberinto);
    return 0;
}

