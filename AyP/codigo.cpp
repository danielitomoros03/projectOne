#include <iostream>
#include <math.h>
using namespace std;
int L = 100; // Vidas iniciales del jugador
int N = 0, M = 0; // Límites de dimensiones del Laberinto
char T; int X = 0, Y = 0, Xb = 0, Yb = 0; // Tipo de elemento, posición en X y Y; para los portales será P Xa Ya Xb Yb
// int posicion_inicial_X = 0, posicion_inicial_Y = 0;
int destino_jugadorX, destino_jugadorY;
char entrada, tipo; // Entrada de datos, tipo de elemento
string elementos = ""; // Lista de elementos en el laberinto
int TotalPortales = 0; // Total de portales en el laberinto
int TotalEspacios = 0; // Total de espacios vacíos en el laberinto
int TotalTrampas = 0; // Total de trampas en el laberinto
int TotalMuros = 0; // Total de muros en el laberinto
int TotalTesoros = 0; // Total de tesoros en el laberinto
int SalidaX = 0, SalidaY = 0; // Posición de salida del laberinto
int tesoros_encontrados = 0; // Cantidad de tesoros encontrados
int trampas_activadas = 0; // Cantidad de trampas activadas
int validarDimensiones(int N, int M){
    if (N < 0 || N > 9 || M < 0 || M > 9) return 1;
    else return 0;
}
int contarElementos(char T){
    if (T == 'P') TotalPortales++;
    else if (T == 'X') TotalTrampas++;
    else if (T == '.') TotalEspacios++;
    else if (T == '#') TotalMuros++;
    else if (T == 'T') TotalTesoros++;
    else return 0;
    return 1;
}
char movimiento; // Lista de cada M movimiento posible (w, a, s, d)
int jugadorX, jugadorY; // Posición inicial en X,Y del jugador
int contarMovimiento(char movimiento){
    if (movimiento == 'w' || movimiento == 'W') destino_jugadorY++; // Mover hacia arriba
    else if (movimiento == 'a' || movimiento == 'A') destino_jugadorX--; // Mover hacia la izquierda
    else if (movimiento == 's' || movimiento == 'S') destino_jugadorY--; // Mover hacia abajo
    else if (movimiento == 'd' || movimiento == 'D') destino_jugadorX++; // Mover hacia la derecha
    else { return 0; } // Validación de movimiento inválido
    return 1;
}
void tesorosEncontrados(){
    cout << "TESOROS: " << tesoros_encontrados << endl;
} // TERMINAR
void trampasActivadas(){
    cout << "TRAMPAS: " << trampas_activadas << endl;
}
void vidaRestante(int L){
    if (L > 0) cout << "VIDA: " << L << endl;
}
int cantidadMovimientos = 0; // Cantidad de movimientos a ingresar
void estadoFinal(){
    if (cantidadMovimientos < 1 || L < 1){
        cout << "MUERTO" << endl;
    }
    else if (SalidaX == jugadorX && SalidaY == jugadorY && L > 0){
        cout << "LOGRADO" << endl;
    }
    else if (SalidaX == jugadorX && SalidaY == jugadorY && L > 0 && tesoros_encontrados == TotalTesoros){
        cout << "SORPRENDENTE" << endl;
    }
    else if (jugadorX != SalidaX && jugadorY != SalidaY){
        cout << "ATRAPADO" << endl;
    }
}
int validarMovimientos(){
    destino_jugadorX = jugadorX;
    destino_jugadorY = jugadorY;

    cin >> entrada;
    if (contarMovimiento(entrada)) { 
        if ( destino_jugadorX < 0 || destino_jugadorX > N || destino_jugadorY < 0 ||  destino_jugadorY > M) cout << "Movimiento bloqueado" << endl;
        else {
            string aux = to_string(destino_jugadorX)+to_string(destino_jugadorY);
            int elemento = elementos.find(aux);
            if (elemento != -1) {   
                tipo = elementos.at(elemento-1);

                // Casos Portal
                if (tipo != 'P' and tipo != '#' and tipo != '.' and tipo != 'T' and tipo != 'X') {
                    cout << elementos.at(8) << endl;
                    jugadorX = (elementos.at(elemento-2))-'0';
                    jugadorY = (elementos.at(elemento-1))-'0';
                } else if (tipo == 'P'){
                    jugadorX = elementos.at(elemento+2)-'0';
                    jugadorY = elementos.at(elemento+3)-'0';
                // Caso Movimiento Inválido
                } else if (tipo == '#') {
                    cout << "Movimiento Bloquedo";
                } else {
                // Casos Actualización de vida
                    if (tipo == 'T'){
                        L += 20;
                        tesoros_encontrados += 1;
                    }else if (tipo == 'X'){
                        L -= 10;
                        trampas_activadas += 1;
                    }
                    jugadorX = destino_jugadorX;
                    jugadorY = destino_jugadorY;
                }   
            } else {
                jugadorX = destino_jugadorX;
                jugadorY = destino_jugadorY;
            }
        }
    }   else {
    }
    return 0;
}
int main(){
    cin >> L; // Vidas iniciales del jugador
    cin >> N >> M;
    if (L <= 0) return 1;
    validarDimensiones(N, M);

    int elementosLaberinto = 0; // Cantidad de elementos en el laberinto
    cin >> elementosLaberinto;
    int casillas = (N+1) * (M+1); // Comparación de casillas NxM del laberinto
    if (elementosLaberinto < 1 || elementosLaberinto > casillas) cout << "No pueden haber mas elementos que casillas" << endl; // Validación de cantidad de elementos en el laberinto

    for (int i = 0; i < elementosLaberinto; i++){
        cin >> T >> X >> Y;
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
                elementosLaberinto--; // Restar un elemento
                elementos += to_string(Xb) + to_string(Yb);
            }
        }
    }
    
    cin >> cantidadMovimientos;
    if (cantidadMovimientos < 0) return 1; // Validar que la cantidad de movimientos sea mayor a 0
    for (int i = 0; i < cantidadMovimientos; i++){
        validarMovimientos();
    }

    tesorosEncontrados(); // Tesoros encontrados por el jugador
    trampasActivadas(); // Cantidad de trampas activadas TERMINAR !!!
    vidaRestante(L); // Vida restante del jugador
    estadoFinal(); // Estado final del jugador
    
    return 0;
};