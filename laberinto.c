#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// declaramos constantes siendo i de ambas, las coordenadas correspondientes a
// 0 = izquierda, 1 = arriba, 2 = derecha, 3 = abajo
const int MOV_FILA[] = {-2, 0, 2, 0};
const int MOV_COL[] =  {0, -2, 0 ,2};

const int DIR_FILA[] = {-1, 0, 1, 0};
const int DIR_COL[] =  {0, -1, 0, 1};

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
            // solo para debug, no deberia suceder
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
    printf("se genero el siguiente laberinto\n");
    if(argc != 3){
        imprimir_laberinto(dimension, laberinto);
    }

    // resolver laberinto
    // Creamos una matriz donde 0 es no visitado, 1 es visitado y 2 es salida
    // utilizamos 0 como no visitado asi tambien podemos usar validar_coordenada
    int **visitados = inicializar_laberinto(dimension);

    int fila = 1;
    int columna = 0;
    // definimos que la entrada ya fue visitada porque no hay diferencia
    visitados[fila][columna] = 1;

    // alocamos memoria para la lista de cola_fila y cola_columna
    int *cola_fila = malloc(dimension * dimension * sizeof(int));
    if (!cola_fila){
        printf("Error al alocar memoria");
        return 1;
    }
    int *cola_columna = malloc (dimension * dimension * sizeof(int));
    if (!cola_columna){
        printf("Error al alocar memoria");
        return 1;
    }
    int inicio = 0;
    int fin = 0;

    // al usar como una pila aumento padres, y voy restando a medida que utilizo, siendo padres-1 mi indice
    // si el iterador padres < 0, es porque se llego al ultimo padre
    int *padres_fila = malloc(dimension * dimension * sizeof(int));
    if (!padres_fila){
        printf("Error al alocar memoria");
        return 1;
    }
    int *padres_columna = malloc(dimension * dimension * sizeof(int));
    if (!padres_columna){
        printf("Error al alocar memoria");
        return 1;
    }
    // padres es la cantidad de padres que existen, el indice seria padres - 1
    int padres = 0;



    while(laberinto[fila][columna] != 4){
    // iteramos sobre cada vecino y vemos si no esta visitado y se
    // encuentra en el rango de la matriz,
        for (int i = 0; i < 4; i++){
            int nueva_fila = fila + DIR_FILA[i];
            int nueva_columna  = columna + DIR_COL[i];
            if (nueva_fila < 0 || nueva_fila >= dimension ||
                nueva_columna < 0 || nueva_columna >= dimension ||
                laberinto[nueva_fila][nueva_columna] == 0 ||
                visitados[nueva_fila][nueva_columna] != 0){
                continue;
            }
            else{

                // marcamos como visitado
                visitados[nueva_fila][nueva_columna] = 1;
                // encolamos los caminos posibles
                cola_fila[fin] = nueva_fila;
                cola_columna[fin] = nueva_columna;
                fin++;
                // debug
                // printf("iterando en posicion (%d,%d), con el iterador %d \n", nueva_fila, nueva_columna, i);
            }

            // despues de cada iteracion agregamos como padre
            // por como escribimos el codigo la casilla de salida no ingresara a la lista padres
            padres_fila[padres] = fila;
            padres_columna[padres] = columna;
            padres++;

        }
        if (inicio == fin){
            printf("no tiene solucion\n");
            break;
        }

        fila = cola_fila[inicio];
        columna = cola_columna[inicio];
        inicio++;

    }

    // para reconstruir
    // iteramos sobre la pila de
    // como padres es la cantidad de elementos que tiene la lista, para usarlo como indice, disminuimos un numero
    padres--;

    while ( padres >= 0){
        if( abs(padres_columna[padres] - columna) + abs(padres_fila[padres] - fila) == 1){
            fila = padres_fila[padres];
            columna = padres_columna[padres];
            laberinto[fila][columna] = 2;
            padres--;
        }
        else{
            padres--;
        }
    }
    // como la entrada hacemos que sea camino correcto, volvemos a cambiarlo
    laberinto[1][0] = 3;

    // calculamos el tiempo
    clock_t tiempo_fin = clock();
    // clock nos brinda los ticks del procesador, por esa razon debemos
    // divdir entre CLOCKS_PER_SEC
    double tiempo = (double)(tiempo_fin - tiempo_inicio) / CLOCKS_PER_SEC;
    // Imprimir mensaje de salida
    printf(" la solucion del laberinto es:\n");


    if(argc != 3){
        imprimir_laberinto(dimension, laberinto);
    }

    printf("el programa se ejecuto: %.4f segundos \n", tiempo);
    liberar_laberinto(dimension, laberinto);
    liberar_laberinto(dimension, visitados);
    free(padres_fila);
    free(padres_columna);
    free(cola_fila);
    free(cola_columna);


    return 0;
}

