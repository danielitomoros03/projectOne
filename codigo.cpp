#include <iostream>
#include <math.h>
using namespace std;
void validarDimensiones(int N, int M){
    if (N < 0 || N > 9 || M < 0 || M > 9) return 1;
}
void validarElementos(){
    if (T == 'P') TotalPortales++;
    if (T == 'X') TotalTrampas++;
    if (T == '.') TotalEspacios++;
    if (T == '#') TotalMuros++;
    if (T == 'T') TotalTesoros++;
}
void validarMovimientos(){
    if (movimiento == 'w'){ jugadorY++; cantidadMovimientos--; } // Mover hacia arriba
    else if (movimiento == 'a'){ jugadorX--; cantidadMovimientos--; } // Mover hacia la izquierda
    else if (movimiento == 's'){ jugadorY--; cantidadMovimientos--; } // Mover hacia abajo
    else if (movimiento == 'd'){ jugadorX++; cantidadMovimientos--; } // Mover hacia la derecha
    else { break; } // Validación de movimiento inválido
}
void objetivos(){
    if (cantidadMovimientos < 1){
        cout << "MUERTO" << endl;
        break;
    }
}
int main(){
    int L = 0; // Vidas iniciales del jugador
    cin >> L;
    int N = 0, M = 0; // Límites de dimensiones del Laberinto
    cin >> N >> M;
    if (L <= 0) return 1;
    validarDimensiones(N, M);

    int elementosLaberinto = 0; // Cantidad de elementos en el laberinto
    cin >> elementosLaberinto;
    int casillas = N * M; // Comparación de casillas NxM del laberinto
    if (elementosLaberinto < 1 || elementosLaberinto > casillas) return 1; // Validación de cantidad de elementos en el laberinto

    int jugadorX = 0, jugadorY = 0; // Posición inicial en X,Y del jugador
    char T; int X = 0, Y = 0, Xb = 0, Yb = 0; // Tipo de elemento, posición en X y Y; para los portales será P Xa Ya Xb Yb
    int TotalPortales = 0; // Total de portales en el laberinto
    int TotalEspacios = 0; // Total de espacios vacíos en el laberinto
    int TotalTrampas = 0; // Total de trampas en el laberinto
    int TotalMuros = 0; // Total de muros en el laberinto
    int TotalTesoros = 0; // Total de tesoros en el laberinto

    for (int i = 0; i < elementosLaberinto; i++){
        cin >> T >> X >> Y >> Xb >> Yb; // Lectura de elementos en el laberinto
        validarElementos();
        if (TotalPortales + TotalEspacios + TotalTrampas + TotalMuros + TotalTesoros > casillas) return 1; // Validación de cantidad de elementos en el laberinto
        // if (TotalPortales > 5 || TotalTrampas > 10 || TotalMuros > 10 || TotalTesoros > 10) return 1; // Validación de cantidad de elementos en el laberinto

        if (T == 'P'){
            Xa = X; Ya = Y;
        } // Portales
        if (T == 'E'){
            // Asignar la posición inicial del jugador
            jugadorX = X; 
            jugadorY = Y;
        } // Posición inicial del jugador
        if (T == 'S'){} // Objetivo del jugador
        if (T == 'X') L -= 10; // Reduce la vida del jugador si cae en ella
        if (T == '.'){} // Espacio vacío que permite el paso
        if (T == '#'){} // Bloquea el paso del jugador 
        if (T == 'T') L += 20; // Otorga un bonus de vida al jugador
    }

    int cantidadMovimientos = 0; // Cantidad de movimientos a ingresar
    cin >> cantidadMovimientos;
    for (int i = 0; i < cantidadMovimientos; i++){
        cin >> cantidadMovimientos; // Cantidad de movimientos a realizar
        if (cantidadMovimientos < 0) break; // Validar que la cantidad de movimientos sea mayor a 0
        char movimiento; // Lista de cada M movimiento posible (w, a, s, d)
        validarMovimientos();
    }

    return 0;
}