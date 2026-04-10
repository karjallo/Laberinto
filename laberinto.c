#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// declaramos constantes siendo i de ambas, las coordenadas correspondientes a
// 0 = izquierda, 1 = arriba, 2 = derecha, 3 = abajo
const int MOV_FILA[] = {-2, 0, 2, 0};
const int MOV_COL[] =  {0, -2, 0 ,2};

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

// int direccion representa un valor de 0 al 3, que representaria en que direccion
// se mueve
bool validar_coordenada(int dimension, int fila, int columna, int direccion){
    if (fila + MOV_FILA[direccion] < 0 || fila + MOV_FILA[direccion] >= dimension){
       return false;
    }
    if (columna + MOV_COL[direccion] < 0 || columna + MOV_COL[direccion] >= dimension){
        return false;
    }
    return true;
}


int **generar_laberinto(int dimension, int **matriz){
    // TODO
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

