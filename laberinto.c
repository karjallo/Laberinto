#include <stdio.h>
#include <stdlib.h>

void imprimir_laberinto(int tamano, int matriz[][tamano]);

int main(int argc, char *argv[]) {
    
    int dimension = 10;
    if (argc > 1){
        dimension = atoi(argv[1]);
    }

    int laberinto[dimension][dimension];

    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            laberinto[i][j] = 0;
        }
    }

    imprimir_laberinto(dimension, laberinto);
    return 0;
}

void imprimir_laberinto(int tamano, int matriz[][tamano]){

    for (int i = 0; i < tamano; i++){
        for (int j = 0; j < tamano; j++){
            if (matriz[i][j] == 0){
                printf("# ");
            }
        }
        printf("\n");
        
    }
}

