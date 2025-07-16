#include <iostream>
using namespace std;

void recorrerTablero(int** tablero, int reinas) {
    cout << "Tablero: \n";
    for (int i = 0; i < reinas; i++) {
        for (int j = 0; j < reinas; j++) {
            tablero[i][j] = 0;
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int reinas = 7;
    // Reservar memoria dinámica para el tablero
    int** tablero = new int*[reinas];
    for (int i = 0; i < reinas; i++) {
        tablero[i] = new int[reinas];
    }

    recorrerTablero(tablero, reinas);

    // Liberar memoria
    for (int i = 0; i < reinas; i++) {
        delete[] tablero[i];
    }
    delete[] tablero;

    return 0;
}