#include <iostream>
using namespace std;
// Ejercicio de N Reinas de todas las soluciones posibles
const int N = 4;
int contador = 0;
struct Soluciones {
    char tableroS[N][N];
};
Soluciones solution[1000];
void inicializarTablero(char tablero[N][N], int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            tablero[i][j] = '-';
}
void recorrerTablero(char tablero[N][N], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}
bool esSeguro(char tablero[N][N], int fila, int columna, int N) {
    // Verifica fila a la izquierda
    for (int i = 0; i < columna; i++)
        if (tablero[fila][i] == 'R') return false;
    // Verifica diagonal superior izquierda
    for (int i = fila, j = columna; i >= 0 && j >= 0; i--, j--)
        if (tablero[i][j] == 'R') return false;
    // Verifica diagonal inferior izquierda
    for (int i = fila, j = columna; i < N && j >= 0; i++, j--)
        if (tablero[i][j] == 'R') return false;
    return true;
}
void resolverNReinas(char tablero[N][N], int columna, int N) {
    if (columna >= N) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                solution[contador].tableroS[i][j] = tablero[i][j];
        contador++;
        return;
    }
    for (int i = 0; i < N; i++) {
        if (esSeguro(tablero, i, columna, N)) {
            tablero[i][columna] = 'R';
            resolverNReinas(tablero, columna + 1, N);
            tablero[i][columna] = '-'; // Backtracking
        }
    }
}
int main() {
    char tablero[N][N];
    inicializarTablero(tablero, N);

    resolverNReinas(tablero, 0, N);

    if(contador == 0) {
        cout << "No hay soluciones posibles." << endl;
    } else
        {
            cout << "Soluciones encontradas: " << contador << endl;
            for (int i = 0; i < contador; i++) {
                cout << "Solución " << i + 1 << ":" << endl;
                recorrerTablero(solution[i].tableroS, N);
            }
        }
    return 0;
}