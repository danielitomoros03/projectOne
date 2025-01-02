#include <iostream>
#include <math.h>
using namespace std;
int validarDimensiones(int N, int M){
    if (N < 0 || N > 9 || M < 0 || M > 9) return 1;
    return 0;
}
int validarElementos(){
    int i = 0;
    if (T == 'P'){
        TotalPortales++;
    }
    /*
    if (T == 'E'){
        i++;
        return (i > 1) ? 1 : 0;
    } 
    if (T == 'S'){
        i++;
        return (i > 1) ? 1 : 0;
    }
    */
    if (T == 'X'){
        TotalTrampas++;
    }
    // if (T == '.'){}
    if (T == '#'){
        TotalMuros++;
    }
    if (T == 'T'){
        TotalTesoros++;
    }
    return 0;
}
int main(){
    int L = 0; // Cantidad de vidas iniciales del jugador
    cin >> L;
    int N = 0, M = 0; // Límites de dimensiones del Laberinto
    cin >> N >> M;
    if (L <= 0) return 1;
    validarDimensiones(N, M);

    int elementosLaberinto = 0; // Cantidad de elementos en el laberinto
    cin >> elementosLaberinto;
    if (elementosLaberinto < 1 || elementosLaberinto > 50) return 1; // Validación de cantidad de elementos en el laberinto 50=NxM
    int jugadorX = 0, jugadorY = 0; // Posición inicial en X,Y del jugador
    char T; int X = 0, Y = 0; // Tipo de elemento en el laberinto, posición en el eje X y eje Y; en el caso de los portales será P Xa Ya Xb Yb

    for (int i = 0; i < elementosLaberinto; i++){
        cin >> T >> X >> Y >> Xb >> Yb; // Lectura de elementos en el laberinto
        int TotalPortales = 0; // Total de portales en el laberinto
        int TotalTrampas = 0; // Total de trampas en el laberinto
        int TotalMuros = 0; // Total de muros en el laberinto
        int TotalTesoros = 0; // Total de tesoros en el laberinto
        validarElementos();
        if (TotalPortales > 10 || TotalTrampas > 10 || TotalMuros > 10 || TotalTesoros > 10) return 1; // Validación de cantidad de elementos en el laberinto

        if (T == 'P'){
            int Xa = X, Ya = Y;
        } // Portales
        if (T == 'E'){
            // Asignar la posición inicial del jugador
            jugadorX = X; 
            jugadorY = Y;
        } // Posición inicial del jugador
        if (T == 'S'){} // Objetivo del jugador
        if (T == 'X'){} // Reduce la vida del jugador si cae en ella
        if (T == '.'){} // Espacio vacío que permite el paso
        if (T == '#'){} // Bloquea el paso del jugador 
        if (T == 'T'){} // Otorga un bonus de vida al jugador
    }

    int cantidadMovimientos = 0; // Cantidad de movimientos a ingresar
    cin >> cantidadMovimientos;
    for (int i = 0; i < cantidadMovimientos; i++){
        char movimiento = ' '; // Lista de cada M movimiento posible (w, a, s, d)
        cin >> cantidadMovimientos;
        if (movimiento == 'w'){ cantidadMovimientos--; } // Mover hacia arriba
        else if (movimiento == 'a'){ cantidadMovimientos--; } // Mover hacia la izquierda
        else if (movimiento == 's'){ cantidadMovimientos--; } // Mover hacia abajo
        else if (movimiento == 'd'){ cantidadMovimientos--; } // Mover hacia la derecha
        else { break; } // Validación de movimiento inválido
        if (cantidadMovimientos == 0){
            cout << "MUERTO" << endl;
            break;
        }
    }

    return 0;
}