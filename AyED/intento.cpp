#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib> // Para atoi

using namespace std;

class Numor {
public:
    int id;
    string nombre;
    string tipo;
    int dmg;
    int life;
};

// Función para llenar un arreglo con datos de un archivo
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

int main() {
    int N, pisos, X, numTorre;

    ifstream DB("NumorisDB.in");

    if (!DB.is_open()) {
        cout << "Error al abrir archivo NumorisDB.in" << endl;
        return -1;
    }

    if (DB >> N) {
        DB.close(); // Cerrar el archivo despues de leer el N
        Numor *numoris = new Numor[N];
        llenarArreglo("NumorisDB.in", numoris, N);
        // Aquí podrías usar el arreglo numoris si lo necesitas para algo más
        delete[] numoris;
        numoris = nullptr;
    } else {
        cout << "Error al leer el tamaño de NumorisDB." << endl;
        DB.close();
        return -1;
    }

    // Leer torre
    cout << "Ingrese el número de la Torre (X): ";
    cin >> X;
    string filename = "Torre";
    filename += to_string(X); // Agrega el número de torre al nombre del archivo
    filename += ".in";       // Agrega la extensión .in

    ifstream torre_info(filename);
    if (!torre_info.is_open()) {
        cout << "Error al abrir archivo " << filename << endl;
        return -1;
    }

    if (torre_info >> pisos) {
        torre_info.close(); // Cerrar después de leer el número de pisos
        int **Torre;
        Torre = new int*[pisos];

        for (int i = 0; i < pisos; i++) {
            Torre[i] = new int[10];
        }

        leerTorre(filename, Torre, pisos);

        // Imprimir la matriz Torre para verificar
        cout << "\nMatriz de la Torre (" << pisos << "x10):\n";
        for (int i = 0; i < pisos; ++i) {
            cout << "Piso " << i + 1 << ": ";
            for (int j = 0; j < 10; ++j) {
                cout << Torre[i][j] << " ";
            }
            cout << endl;
        }

        // Liberar la memoria dinámica de la matriz Torre
        for (int i = 0; i < pisos; i++) {
            delete[] Torre[i];
        }
        delete[] Torre;
        Torre = nullptr;

    } else {
        cout << "Error al leer el número de pisos de la torre." << endl;
        torre_info.close();
        return -1;
    }
    system("pause");
    return 0;
}
