#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// declaramos constantes siendo i de ambas, las coordenadas correspondientes a
// 0 = izquierda, 1 = arriba, 2 = derecha, 3 = abajo
const int DIR_FILA[] = {-1, 0, 1, 0};
const int DIR_COL[] =  {0, -1, 0, 1};


typedef struct {
    int fila;
    int columna;
} Coordenada;

// estado 0 pared, 1 camino libre, 2 visitada
typedef struct{
    Coordenada padre;
    int estado;
    bool es_salida;
    bool es_entrada;
    bool es_camino;
} Nodo;


void imprimir_laberinto(int dimension, Nodo **matriz){

    for (int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++){
            if (matriz[i][j].es_entrada){
                printf("O ");
                continue;
            }
            else if (matriz[i][j].es_salida){
                printf("X ");
                continue;
            }
            else if (matriz[i][j].es_camino){
                printf("* ");
                continue;
            }
            else if (matriz[i][j].estado == 0) {
                printf("# ");
                continue;
            }
            else{
                printf(". ");
            }
        }
        printf("\n");
    }
}


void liberar_laberinto(int dimension, Nodo **matriz){
    for (int i = 0; i < dimension; i++){
        free(matriz[i]);
    }
    free(matriz);
}


Nodo **inicializar_laberinto(int dimension){
    Nodo **matriz = malloc(dimension * sizeof(Nodo *));
    if (!matriz) {
        return NULL;
    }
    for (int i = 0; i < dimension; i++){
        matriz[i] = calloc(dimension, sizeof(Nodo));
        if(!matriz[i]){
            liberar_laberinto(i, matriz);
            return NULL;
        }
    }
    return matriz;
}


bool validar_coordenada(int dimension, int fila, int columna, Nodo **matriz){
    if (fila <= 0 || fila >= dimension - 1){
       return false;
    }
    if (columna <= 0 || columna >= dimension - 1){
        return false;
    }
    if (matriz[fila][columna].estado != 0)
        return false;

    return true;
}

bool validar_camino(int dimension, int fila, int columna, Nodo **matriz){
    if (fila < 0 || fila > dimension - 1){
       return false;
    }
    if (columna < 0 || columna > dimension - 1){
        return false;
    }
    if (matriz[fila][columna].estado != 1)
        return false;

    return true;
}

bool resolver_bfs(int dimension, Nodo **laberinto){
    int fila = 1;
    int columna = 0;
    laberinto[fila][columna].estado = 2;

    // alocamos memoria para el arreglo de struct coordenada
    Coordenada *cola = malloc(dimension * dimension * sizeof(Coordenada));
    if (!cola){
        printf("Error al alocar memoria\n");
        return false;
    }

    // utilizamos inicio y fin para que el array cola se comporte como una cola
    int inicio = 0;
    int fin = 0;
    int nueva_fila;
    int nueva_columna;

    while(!laberinto[fila][columna].es_salida){
        for (int i = 0; i < 4; i++){
            nueva_fila = fila + DIR_FILA[i];
            nueva_columna = columna + DIR_COL[i];

            if (validar_camino(dimension, nueva_fila, nueva_columna, laberinto)){

                // marcamos como visitado al incluir en la cola para evitar
                // incluir mas de una vez
                laberinto[nueva_fila][nueva_columna].estado = 2;
                cola[fin].fila = nueva_fila;
                cola[fin].columna = nueva_columna;
                fin++;

                // agregamos cual fue su padre
                laberinto[nueva_fila][nueva_columna].padre.fila = fila;
                laberinto[nueva_fila][nueva_columna].padre.columna = columna;
            }
        }
        if (inicio == fin){
            free(cola);
            return false;
        }

        fila = cola[inicio].fila;
        columna = cola[inicio].columna;
        inicio++;
    }

    free(cola);
    return true;
}

void reconstruir_camino(int dimension, Nodo **laberinto){
    // buscamos la salida para empezar desde ahi
    int fila = dimension - 2;
    int columna = dimension - 1;

    while (!laberinto[fila][columna].es_entrada){
        int nueva_fila = laberinto[fila][columna].padre.fila;
        int nueva_columna = laberinto[fila][columna].padre.columna;
        laberinto[fila][columna].es_camino = true;
        fila = nueva_fila;
        columna = nueva_columna;
    }
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
void generar_dfs(int fila, int columna, int dimension, Nodo **matriz){

    // marcamos como camino libre
    matriz[fila][columna].estado = 1;
    int direcciones[] = {0, 1, 2, 3};
    // solo para evitar pasar valores no hardcodeados
    int cantidad = sizeof(direcciones) / sizeof(int);

    fisher_yates(direcciones, cantidad);

    for (int i = 0; i < cantidad; i++){
        int fila_destino = fila + (2 * DIR_FILA[direcciones[i]]);
        int columna_destino = columna + (2 * DIR_COL[direcciones[i]]);

        // si el movimiento es valido, rompemos solo pared intermedia
        // porque la funcion dfs ya rompe la pared de destino
        if (validar_coordenada(dimension, fila_destino, columna_destino, matriz)){

            int intermedio_x = (fila + fila_destino) / 2;
            int intermedio_y = (columna + columna_destino) / 2;

            // marcamos el intermedio como camino libre, no el destino, porque se hace al inicio de la funcion recursiva
            matriz[intermedio_x][intermedio_y].estado = 1;

            generar_dfs(fila_destino, columna_destino, dimension, matriz);
        }
    }
}


int main(int argc, char *argv[]) {

    clock_t tiempo_inicio = clock();
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
    Nodo **laberinto = inicializar_laberinto(dimension);
    // si nuestra funcion de inicializar_laberinto nos brinda un pointer NULL, print error y return
    if (!laberinto){
        printf("Error al asignar memoria\n");
        return 1;
    }

    // definimos entrada y salida
    laberinto[1][0].es_entrada = true;
    laberinto[dimension - 2][dimension - 1].es_salida = true;

    // generamos el laberinto, colocamos la posicion 1,1 como partida
    // ya que se encuentra al lado de la entrada
    generar_dfs(1, 1, dimension, laberinto);
    printf("se genero el siguiente laberinto\n");
    if(argc != 3){
        imprimir_laberinto(dimension, laberinto);
    }

    // colocamos que la salida tenga el estado de visitado, osino no
    // funcionara nuestra validar_camino
    laberinto[dimension - 2][dimension - 1].estado = 1;
    laberinto[1][0].estado = 1;

    // solucionar laberinto
    bool solucion = resolver_bfs(dimension, laberinto);

    if (!solucion){
        printf("no se encontro solucion\n");
        return 1;
    }
    else{
        //reconstruir camino
        reconstruir_camino(dimension, laberinto);
    }


    // calculamos el tiempo
    clock_t tiempo_fin = clock();
    double tiempo = (double)(tiempo_fin - tiempo_inicio) / CLOCKS_PER_SEC;

    // Imprimir mensaje de salida
    printf(" la solucion del laberinto es:\n");
    if(argc != 3){
        imprimir_laberinto(dimension, laberinto);
    }
    printf("el programa se ejecuto: %.4f segundos \n", tiempo);

    // liberar memoria alocada
    liberar_laberinto(dimension, laberinto);

    return 0;
}

