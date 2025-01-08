#include <iostream>
#include <math.h>
using namespace std;
int TotalPortales = 0; // Total de portales en el laberinto
int TotalEspacios = 0; // Total de espacios vacíos en el laberinto
int TotalTrampas = 0; // Total de trampas en el laberinto
int TotalMuros = 0; // Total de muros en el laberinto
int TotalTesoros = 0; // Total de tesoros en el laberinto
int SalidaX = 0, SalidaY = 0; // Posición de salida del laberinto
int validarDimensiones(int N, int M){
    if (N < 0 || N > 9 || M < 0 || M > 9) return 1;
    else return 0;
}
void contarElementos(char T){
    if (T == 'P') TotalPortales++;
    if (T == 'X') TotalTrampas++;
    if (T == '.') TotalEspacios++;
    if (T == '#') TotalMuros++;
    if (T == 'T') TotalTesoros++;
}
char movimiento; // Lista de cada M movimiento posible (w, a, s, d)
int jugadorX = 0, jugadorY = 0; // Posición inicial en X,Y del jugador
int validarMovimientos(char movimiento){
    if (movimiento == 'w' || movimiento == 'W') jugadorY++; // Mover hacia arriba
    else if (movimiento == 'a' || movimiento == 'A') jugadorX--; // Mover hacia la izquierda
    else if (movimiento == 's' || movimiento == 'S') jugadorY--; // Mover hacia abajo
    else if (movimiento == 'd' || movimiento == 'D') jugadorX++; // Mover hacia la derecha
    else { return 1; } // Validación de movimiento inválido
    return 0;
}
void tesorosEncontrados(){
    cout << "TESOROS: " << TotalTesoros << endl; // AGREGAR VARIABLE DE TESOROS ENCONTRADOS, CONTADOR 
} // TERMINAR
void trampasActivadas(){
    cout << "TRAMPAS: " << TotalTrampas << endl; // AGREGAR VARIABLE DE TRAMPAS ACTIVADAS, CONTADOR
} // TERMINAR
int L = 0; // Vidas iniciales del jugador
void vidaRestante(int L){
    if (L > 0) cout << "VIDA: " << L << endl;
}
int cantidadMovimientos = 0; // Cantidad de movimientos a ingresar
void estadoFinal(){
    if (cantidadMovimientos < 1){
        cout << "MUERTO" << endl;
    }
    if (SalidaX == jugadorX && SalidaY == jugadorY && L > 0){
        cout << "LOGRADO" << endl;
    }
    if (SalidaX == jugadorX && SalidaY == jugadorY && L > 0 && TotalTesoros > 1){ // CAMBIAR TOTAL POR LA CANTIDAD DE TESOROS ENCONTRADOS
        cout << "SORPRENDENTE" << endl;
    }
    if (jugadorX != SalidaX && jugadorY != SalidaY){
        cout << "ATRAPADO" << endl;
    }
}
int main(){
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
    string elementos = ""; // Lista de elementos en el laberinto
    for (int i = 0; i < elementosLaberinto; i++){
        if (T == 'E'){
            // Asignar la posición inicial del jugador
            jugadorX = X; 
            jugadorY = Y;
        } // Posición inicial del jugador
        else if (T == 'S'){
            SalidaX = X; 
            SalidaY = Y;
        } // Posición de salida del laberinto
        else {
            elementos += T + to_string(X) + to_string(Y);
            if (T == 'P'){
                cin >> Xb >> Yb; // Leer los valores adicionales solo si es un portal
                elementos += to_string(Xb) + to_string(Yb);
            }
            elementos += " ";
        }
        // contarElementos(T);
        /*if (TotalPortales + TotalEspacios + TotalTrampas + TotalMuros + TotalTesoros > casillas) return 1; // Validación de cantidad de elementos en el laberinto
        // if (TotalPortales > 5 || TotalTrampas > 10 || TotalMuros > 10 || TotalTesoros > 10) return 1; // Validación de cantidad de elementos en el laberinto
        if (T == 'X') L -= 10; // Reduce la vida del jugador si cae en ella
        if (T == '.'){} // Espacio vacío que permite el paso
        if (T == '#'){} // Bloquea el paso del jugador 
        if (T == 'T') L += 20; // Otorga un bonus de vida al jugador
        */
        // cout << elementos << endl;
    }
    
    cin >> cantidadMovimientos;
    if (cantidadMovimientos < 0) return 1; // Validar que la cantidad de movimientos sea mayor a 0
    for (int i = 0; i < cantidadMovimientos; i++){
        cin >> movimiento;
        cantidadMovimientos--; // Restar un movimiento
        validarMovimientos(movimiento);
        if (cantidadMovimientos < 0) return 1; // Validar que dispone de movimientos
    }

    /*
    tesorosEncontrados(); // Tesoros encontrados por el jugador TERMINAR !!!
    trampasActivadas(); // Cantidad de trampas activadas TERMINAR !!!
    vidaRestante(L); // Vida restante del jugador
    estadoFinal(); // Estado final del jugador
    */
    return 0;
}
