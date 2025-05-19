#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

class Numor {
public:
    int id;
    string nombre;
    string tipo;
    int dmg;
    int life;
};

void llenarArreglo(const string& nombreArchivo, Numor* numoris, int N) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    // Saltar la primera línea (que ya leímos para obtener el tamaño)
    string linea;
    getline(archivo, linea);

    for (int i = 0; i < N; ++i) {
        archivo >> numoris[i].id >> numoris[i].nombre >> numoris[i].tipo >> numoris[i].dmg >> numoris[i].life;
    }
    archivo.close();
}

void leerTorre(string& filename, int **Torre, int pisos) {
    ifstream torre(filename);
    if (!torre.is_open()) {
        cout << "Archivo no encontrado: " << filename << endl;
        return;
    }

    string linea;
    getline(torre, linea); // Leer y descartar la primera línea (número de pisos)

    for (int i = 0; i < pisos; ++i) {
        getline(torre, linea);
        int num_actual = 0;
        string numero_str;
        for (size_t j = 0; j < linea.length() && num_actual < 10; ++j) {
            if (linea[j] >= '0' && linea[j] <= '9') {
                numero_str += linea[j];
            } else if (!numero_str.empty()) {
                Torre[i][num_actual++] = atoi(numero_str.c_str());
                numero_str = "";
            } else if (linea[j] == ' ') {
                // Ignorar espacios entre números
            }
        }
        // Procesar el último número si existe y hay espacio
        if (!numero_str.empty() && num_actual < 10) {
            Torre[i][num_actual++] = atoi(numero_str.c_str());
        }
        // Llenar el resto de la fila con un valor predeterminado (ej: 0)
        while (num_actual < 10) {
            Torre[i][num_actual++] = 0;
        }
    }
    torre.close();
}

Numor** prepararNumorisOscuros(int** Torre, int pisos, int* cantidadPorPiso, Numor* numorisDB, int cantidadNumoris) {
    Numor** numorisOscuros = new Numor*[pisos];
    
    for (int i = 0; i < pisos; i++) {
        // Contar cuántos Numoris hay en este piso (IDs no cero)
        int count = 0;
        for (int j = 0; j < 10; j++) {
            if (Torre[i][j] > 0) {
                count++;
            }
        }
        
        cantidadPorPiso[i] = count;
        numorisOscuros[i] = new Numor[count];
        
        // Llenar el arreglo de Numoris oscuros para este piso
        int idx = 0;
        for (int j = 0; j < 10 && idx < count; j++) {
            if (Torre[i][j] > 0) {
                // Buscar el Numor correspondiente en la base de datos
                for (int k = 0; k < cantidadNumoris; k++) {
                    if (numorisDB[k].id == Torre[i][j]) {
                        numorisOscuros[i][idx] = numorisDB[k];
                        idx++;
                        break;
                    }
                }
            }
        }
    }
    
    return numorisOscuros;
}

void generarPermutacionesUnicas(Numor* arr, int n, int tamPer, Numor* permutacionActual, bool* usado, int numEle) {
    if (tamPer == numEle) {
        for (int i = 0; i < tamPer; ++i) {
            cout << permutacionActual[i].id << " ";
        }
        cout << endl;
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (!usado[i]) {
            usado[i] = true;
            permutacionActual[numEle] = arr[i];
            generarPermutacionesUnicas(arr, n, tamPer, permutacionActual, usado, numEle + 1);
            usado[i] = false; // Backtrack
        }
    }
}

void obtenerPermutacionesUnicasDe6(Numor* arr, int n) {
    int k = 6;
    Numor* permutacionActual = new Numor[k];
    bool* usado = new bool[n];
    for (int i = 0; i < n; ++i) {
        usado[i] = false;
    }

    if (n < k) {
        cout << "El tamaño del arreglo es menor que 6. No se pueden generar permutaciones de tamaño 6." << endl;
        delete[] permutacionActual;
        delete[] usado;
        return;
    }

    generarPermutacionesUnicas(arr, n, k, permutacionActual, usado, 0);

    delete[] permutacionActual;
    delete[] usado;
}



int main() {
    int N, pisos, X;

    // Leer la base de datos de Numoris
    ifstream DB("NumorisDB.in");
    if (!DB.is_open()) {
        cout << "Error al abrir archivo NumorisDB.in" << endl;
        return -1;
    }

    DB >> N;
    DB.close();
    
    Numor* numorisDB = new Numor[N];
    llenarArreglo("NumorisDB.in", numorisDB, N);

    // Leer torre
    cout << "Ingrese el número de la Torre (X): ";
    cin >> X;
    string filename = "Torre" + to_string(X) + ".in";

    ifstream torre_info(filename);
    if (!torre_info.is_open()) {
        cout << "Error al abrir archivo " << filename << endl;
        delete[] numorisDB;
        return -1;
    }

    torre_info >> pisos;
    torre_info.close();
    
    int** Torre = new int*[pisos];
    for (int i = 0; i < pisos; i++) {
        Torre[i] = new int[10];
    }

    leerTorre(filename, Torre, pisos);

    obtenerPermutacionesUnicasDe6(numorisDB, N);

    system("pause");
    return 0;
}
