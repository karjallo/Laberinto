# Laberinto

## Como genere el laberinto?
Utilizando el algoritmo DFS, asegurandome de recorrer todas las casillas dentro de la matriz generada, tambien asi asegurando que exista al menos una solucion

## Algoritmo para resolver?
Utilice el algoritmo BFS debido a que te asegura encontrar el camino mas corto

## Descubrimientos
1. Lo increiblemente rapido que se resuelven los laberintos a pesar de tener dimensiones gigantescas
2. Los limitantes a la hora de utilizar funciones recursivas, por ejemplo ya que utilizo c para resolver, como primeramente tenia la funcion dfs de forma recursiva, tenia un desbordamiento de pila a partir de numeros 1201 aproximadamente

## Que haria diferente la proxima vez?
1. Utilizar structs a la primera, por intentar no complicarme al principio, utilize varias matrices para poder tener un seguimiento de distintos valores, llegando asi a tener 4 matrices:
    - Una para imprimir, donde se registraban paredes, espacios, entrada y salida
    - Otra para llevar el seguimiento de visitados y no visitados
    - Otras dos para guardar las coordenadas de los padres
Haciendo que el codigo sea mucho mas dificil de entender
