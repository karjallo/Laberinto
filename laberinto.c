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
                printf("[#]");
                break;
            case 1:
                printf("[ ]");
                break;
            case 2:
                printf("[*]");
                break;
            case 3:
                printf("[O]");
                break;
            case 4:
                printf("[X]");
                break;
            // solo para[ ]debug, no deberia suceder
            default:
                printf("[&]");
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
bool validar_coordenada(int dimension, int fila, int columna, int **matriz){
    if (fila <= 0 || fila >= dimension - 1){
       return false;
    }
    if (columna <= 0 || columna >= dimension - 1){
        return false;
    }
    if (matriz[fila][columna] != 0)
        return false;

    return true;
}


void fisher_yates(int *direcciones, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = direcciones[i];
        direcciones[i] = direcciones[j];
        direcciones[j] = tmp;
    }
}


// podemos modificar la matriz porque usamos de argumento pointers
void dfs(int fila, int columna, int dimension, int **matriz){
    matriz[fila][columna] = 1;
    int direcciones[] = {0, 1, 2, 3};
    fisher_yates(direcciones, 4);

    for (int i = 0; i < 4; i++){
        int fila_destino = fila + MOV_FILA[direcciones[i]];
        int columna_destino = columna + MOV_COL[direcciones[i]];

        // si el movimiento es valido, rompemos solo pared intermedia
        // porque la funcion dfs ya rompe la pared de destino
        if (validar_coordenada(dimension, fila_destino, columna_destino, matriz)){
            int intermedio_x = (fila + fila_destino) / 2;
            int intermedio_y = (columna + columna_destino) / 2;

            matriz[intermedio_x][intermedio_y] = 1;

            dfs(fila_destino, columna_destino, dimension, matriz);
        }
    }
}


int main(int argc, char *argv[]) {

    clock_t inicio = clock();
    srand(time(NULL));
    int dimension = 11;

    // defino que el minimo de dimension sea 3, porque mas adelante colocamos (1,0)
    // y (n-1, n-2) como entrada y salida, para que eso sea viable el menor valor tiene
    // que ser 3
    if (argc > 1){
        dimension = atoi(argv[1]);
        if (dimension < 3 || dimension % 2 == 0){
            printf("opcion invalida %s \n Ingrese un numero impar mayor a 2", argv[1]);
            return 1;
        }
    }
    // creamos un puntero que apunta a una array de punteros
    // cada puntero apuntaria a un array de enteros, que representan los valores de una fila de la matriz
    // inicializamos el laberinto con todos los valores = 0
    int **laberinto = inicializar_laberinto(dimension);
    // si nuestra funcion de inicializar_laberinto nos brinda un pointer NULL, print error y return
    if (!laberinto){
        printf("Error al asignar memoria\n");
        return 1;
    }

    // definimos entrada y salida
    laberinto[1][0] = 3;
    laberinto[dimension - 2][dimension - 1] = 4;

    // generamos el laberinto, colocamos la posicion 1,1 como partida
    // ya que se encuentra al lado de la entrada
    dfs(1, 1, dimension, laberinto);

    // resolver laberinto
    // TODO


    // calculamos el tiempo
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    // Imprimir mensaje de salida
    printf("el programa se ejecuto: %.4f segundos \n", tiempo);
    // debug momentaneo
    imprimir_laberinto(dimension, laberinto);
    // nos aseguramos que la memoria del heap sea liberada
    liberar_laberinto(dimension, laberinto);
    return 0;
}

