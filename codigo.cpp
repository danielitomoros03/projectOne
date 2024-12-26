#include <iostream>
#include <math.h>
using namespace std;
int main(){
    int L = 0; // Cantidad de vidas iniciales del jugador
    cin >> L;
    int N = 0, M = 0; // Límites de dimensiones del Laberinto
    cin >> N >> M;
    int elementosLaberinto = 0; // Cantidad de elementos en el laberinto
    cin >> elementosLaberinto;
    char T; int X = 0, Y = 0; // Tipo de elemento en el laberinto, posición en el eje X y eje Y; en el caso de los portales será P Xa Ya Xb Yb
    for (int i = 0; i < elementosLaberinto; i++){
        cin >> T >> X >> Y;
        if (T == 'P'){
            int Xa = X, Ya = Y, Yb = 0, Xb = 0;
            cin >> Xb >> Yb;
        }
        // EVALUAR ESTE CASO
        if (T == 'E'){} // Posición inicial del jugador
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
            cout << "GAME OVER" << endl;
            break;
        }
    }

    return 0;
}