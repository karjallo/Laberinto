#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 101
// 0 es muro, 1 es espacio vacio,  2 es camino correcto, 3 es entrada, 4 salida
void imprimir_laberinto(int tamano, int matriz[][tamano]){

    for (int i = 0; i < tamano; i++){
        for (int j = 0; j < tamano; j++){
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
            default:
                printf("&");
                break;
        }
        printf("\n");

    }
}

int main(int argc, char *argv[]) {
    
    int laberinto[MAX][MAX];
    int dimension = 21;
    laberinto[dimension][dimension] = {0};
    laberinto[1][0] = 

    if (argc > 1){
        dimension = atoi(argv[1]);
        if (dimension > MAX || dimension <= 0 || dimension % 2 != 0){
            printf("opcion invalida %s \n Ingrese un numero impar hasta %d", argv[1], MAX);
        }
    }

    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            laberinto[i][j] = 0;
        }
    }

    imprimir_laberinto(dimension, laberinto);
    return 0;
}

