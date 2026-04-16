#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// declaramos constantes siendo i de ambas, las coordenadas correspondientes a
// 0 = izquierda, 1 = arriba, 2 = derecha, 3 = abajo
const int DIR_FILA[] = {-1, 0, 1, 0};
const int DIR_COL[] =  {0, -1, 0, 1};

typedef struct {
    int fila;
    int columna;
} Coordenada;

typedef struct{
    Coordenada padre;
    bool es_pared;
    bool visitado;
    bool es_salida;
    bool es_entrada;
    bool es_camino;
} Nodo;


void imprimir_uso(char *nombre_programa){
    printf("Uso: %s [N] [opciones]\n\n", nombre_programa);
    printf("Argumentos:\n");
    printf("  N              dimension del laberinto (impar, mayor a 3, por defecto 11)\n\n");
    printf("Opciones:\n");
    printf("  --silent       no muestra el laberinto generado ni la solucion\n");
    printf("  --multiple     genera mas de un posible camino, por defecto false\n");
    printf("  --help         muestra este mensaje\n");
    printf("Ejemplos:\n");
    printf("  %s\n", nombre_programa);
    printf("  %s 21\n", nombre_programa);
    printf("  %s 31 --multiple\n", nombre_programa);
    printf("  %s 13 --silent\n", nombre_programa);
    printf("  %s 17 --silent --multiple\n", nombre_programa);
}


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
            else if (matriz[i][j].es_pared) {
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
    // calloc inicia los valores int = 0, bool = false, *p = NULL
    for (int i = 0; i < dimension; i++){
        matriz[i] = calloc(dimension, sizeof(Nodo));
        if(!matriz[i]){
            liberar_laberinto(i, matriz);
            return NULL;
        }
    }
    // hacemos que todo sea pared
    for (int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            matriz[i][j].es_pared = true;
        }
    }
    return matriz;
}


bool validar_coordenada(int dimension, int fila, int columna, Nodo **matriz){
    if (fila <= 0 || fila >= dimension - 1){
       return false;
    }
    else if (columna <= 0 || columna >= dimension - 1){
        return false;
    }
    else if (!matriz[fila][columna].es_pared){
        return false;
    }
    else {
        return true;
    }
}


bool validar_camino(int dimension, int fila, int columna, Nodo **matriz){
    if (fila < 0 || fila > dimension - 1){
       return false;
    }
    else if (columna < 0 || columna > dimension - 1){
        return false;
    }
    else if (matriz[fila][columna].es_pared){
        return false;
    }
    else if (matriz[fila][columna].visitado){
        return false;
    }
    else {
        return true;
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
    matriz[fila][columna].es_pared = false;
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
            matriz[intermedio_x][intermedio_y].es_pared = false;

            generar_dfs(fila_destino, columna_destino, dimension, matriz);
        }
    }
}


void destruir_paredes(int dimension, Nodo **matriz){
    int numero;
    for (int i = 1; i < dimension - 1; i++){
        for (int j = 1; j < dimension - 1; j++){
            if (!matriz[i][j].es_pared){
                continue;
            }
            numero = rand() % 4;
            if (numero == 0){
                matriz[i][j].es_pared = false;
            }
        }
    }
}


bool resolver_bfs(int dimension, Nodo **matriz){
    int fila = 1;
    int columna = 0;
    matriz[fila][columna].visitado = true;

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

    while(!matriz[fila][columna].es_salida){
        for (int i = 0; i < 4; i++){
            nueva_fila = fila + DIR_FILA[i];
            nueva_columna = columna + DIR_COL[i];

            if (validar_camino(dimension, nueva_fila, nueva_columna, matriz)){

                // marcamos como visitado al incluir en la cola
                matriz[nueva_fila][nueva_columna].visitado = true;
                cola[fin].fila = nueva_fila;
                cola[fin].columna = nueva_columna;
                fin++;

                // agregamos cual fue su padre
                matriz[nueva_fila][nueva_columna].padre.fila = fila;
                matriz[nueva_fila][nueva_columna].padre.columna = columna;
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


void reconstruir_camino(int dimension, Nodo **matriz){
    int fila = dimension - 2;
    int columna = dimension - 1;
    int nueva_fila;
    int nueva_columna;

    while (!matriz[fila][columna].es_entrada){
        nueva_fila = matriz[fila][columna].padre.fila;
        nueva_columna = matriz[fila][columna].padre.columna;
        matriz[fila][columna].es_camino = true;
        fila = nueva_fila;
        columna = nueva_columna;
    }
}


int main(int argc, char *argv[]) {

    //inicializamos calculo de ticks
    clock_t tiempo_inicio = clock();
    srand(time(NULL));
    int dimension = 11;
    bool silent = false;
    bool multiple = false;

    // defino que el minimo de dimension sea 3, porque mas adelante colocamos (1,0)
    // y (n-1, n-2) como entrada y salida, para que eso sea viable el menor valor tiene
    // que ser 3
    for ( int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--silent") == 0){
            silent = true;
        }
        else if (strcmp(argv[i], "--multiple") == 0){
            multiple = true;
        }
        else if (strcmp(argv[i], "--help") == 0){
            imprimir_uso(argv[0]);
            return 1;
        }
        else {
            dimension = atoi(argv[i]);
            if (dimension < 3 || dimension % 2 == 0){
                printf("opcion invalida: %s\n", argv[i]);
                imprimir_uso(argv[0]);
                return 1;
            }
        }
    }
    // creamos el laberinto
    Nodo **laberinto = inicializar_laberinto(dimension);
    if (!laberinto){
        printf("Error al asignar memoria\n");
        return 1;
    }

    // definimos entrada y salida
    laberinto[1][0].es_entrada = true;
    laberinto[dimension - 2][dimension - 1].es_salida = true;

    // generamos el laberinto, colocamos la posicion 1,1 como partida
    generar_dfs(1, 1, dimension, laberinto);
    printf("se genero el siguiente laberinto\n");
    if(!silent){
        imprimir_laberinto(dimension, laberinto);
    }

    // destruimos paredes aleatorias para asegurar posibles caminos
    // con un 25% chance
    if (multiple){
        destruir_paredes(dimension, laberinto);
    }

    // como en la generacion no visitamos la entrada y la salida, destruimos pared
    laberinto[dimension - 2][dimension - 1].es_pared = false;
    laberinto[1][0].es_pared = false;

    // solucionar laberinto
    bool solucion = resolver_bfs(dimension, laberinto);

    if (!solucion){
        printf("no se encontro solucion\n");
        return 1;
    }
    else{
        //reconstruir camino solo si se soluciono
        reconstruir_camino(dimension, laberinto);
    }


    // Imprimir mensaje de salida
    printf(" la solucion del laberinto es:\n");
    if(!silent){
        imprimir_laberinto(dimension, laberinto);
    }
    clock_t tiempo_fin = clock();
    double tiempo = (double)(tiempo_fin - tiempo_inicio) / CLOCKS_PER_SEC;
    printf("el programa se ejecuto: %.4f segundos \n", tiempo);

    // liberar memoria alocada
    liberar_laberinto(dimension, laberinto);

    return 0;
}

