#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

const int VMAX = 9999999;

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

// Función para calcular la ventaja elemental
float calcularVentaja(const string& tipo1, const string& tipo2) {
    if (tipo1 == "Agua" && tipo2 == "Fuego") return 2.0;
    if (tipo1 == "Fuego" && tipo2 == "Tierra") return 2.0;
    if (tipo1 == "Tierra" && tipo2 == "Aire") return 2.0;
    if (tipo1 == "Aire" && tipo2 == "Agua") return 2.0;
    if (tipo2 == "Agua" && tipo1 == "Fuego") return 0.5;
    if (tipo2 == "Fuego" && tipo1 == "Tierra") return 0.5;
    if (tipo2 == "Tierra" && tipo1 == "Aire") return 0.5;
    if (tipo2 == "Aire" && tipo1 == "Agua") return 0.5;
    return 1.0;
}

// Estructura para almacenar los resultados del combate
struct ResultadoCombate {
    int danioTotal;
    int bajas;
    bool victoria;
};

// Función para simular un ataque entre dos Numoris
bool atacar(Numor& atacante, Numor& defensor) {
    float ventaja = calcularVentaja(atacante.tipo, defensor.tipo);
    int danio = (int)(atacante.dmg * ventaja);
    defensor.life -= danio;
    return defensor.life <= 0; // Retorna true si el defensor es derrotado
}

// Función para simular el combate entre un equipo y un piso de la torre
ResultadoCombate simularCombatePiso(Numor (&equipo)[6], Numor oscuros[], int cantidadOscuros) {
    ResultadoCombate resultado = {0, 0, false};
    
    // Copias de trabajo para no modificar los originales
    Numor equipoCopia[6];
    Numor oscurosCopia[10]; // Máximo 10 Numoris por piso
    

    for (int i = 0; i < 6; i++) {
        equipoCopia[i] = equipo[i];
    } cout << endl;

    for (int i = 0; i < cantidadOscuros; i++) {
        oscurosCopia[i] = oscuros[i];
    } cout << endl;
    
    int indiceEquipo = 0;
    int indiceOscuro = 0;
    
    // Mientras haya Numoris vivos en ambos lados
    while (indiceEquipo < 6 && indiceOscuro < cantidadOscuros) {
        // El equipo ataca primero (retador)
        if (atacar(equipoCopia[indiceEquipo], oscurosCopia[indiceOscuro])) {
            // Numor oscuro derrotado, pasar al siguiente
            indiceOscuro++;
            continue;
        }
        
        // El Numor oscuro contraataca
        if (atacar(oscurosCopia[indiceOscuro], equipoCopia[indiceEquipo])) {
            // Numor del equipo derrotado
            resultado.bajas++;
            resultado.danioTotal += equipo[indiceEquipo].life; // Daño total es la vida original
            indiceEquipo++;
            continue;
        }
    }
    
    // Victoria si todos los oscuros fueron derrotados
    resultado.victoria = (indiceOscuro >= cantidadOscuros);
    
    return resultado;
}

// Función para simular el combate contra toda la torre
ResultadoCombate simularCombateTorre(Numor equipo[6], Numor** numorisOscuros, int* cantidadPorPiso, int pisos) {
    ResultadoCombate resultadoFinal = {0, 0, true};

    Numor equipoActual[6];
    for (int j = 0; j < 6; j++) {
        equipoActual[j] = equipo[j];
    }

    for (int i = 0; i < pisos; i++) {        
        
        ResultadoCombate resultadoPiso = simularCombatePiso(equipoActual, numorisOscuros[i], cantidadPorPiso[i]);
        
        resultadoFinal.danioTotal += resultadoPiso.danioTotal;
        resultadoFinal.bajas += resultadoPiso.bajas;
                
        // Si perdimos en algún piso, perdimos toda la torre
        if (!resultadoPiso.victoria) {
            resultadoFinal.victoria = false;
            break;
        }
    }
    
    return resultadoFinal;
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



void encontrarMejorEquipoBT(Numor* numorisDB, int cantidadNumoris, int tamPer, Numor equipoActual[6], bool usado[], int numEle, 
    Numor** numorisOscuros, int* cantidadPorPiso, int pisos, int mejorEquipoIds[6], int idsActuales[6], int equipoSize, 
    int& menorDanio, int& menorBajas, int& menorSumaIds) {


    if (tamPer == numEle) {
        for (int i = 0; i < tamPer; ++i) {
            idsActuales[i] = equipoActual[i].id;
            cout << idsActuales[i] << " ";
        }
        cout << endl;

        ResultadoCombate resultado = simularCombateTorre(equipoActual, numorisOscuros, cantidadPorPiso, pisos);
        if (resultado.victoria) {
            bool esNuevoMejor = false;
            if (resultado.bajas < menorBajas) {
                esNuevoMejor = true;
            } else if (resultado.bajas == menorBajas) {
                if (resultado.danioTotal < menorDanio) {
                    esNuevoMejor = true;
                } else if (resultado.danioTotal == menorDanio) {
                    int sumaIds = 0;
                    for (int i = 0; i < 6; i++) {
                        sumaIds += idsActuales[i];
                    }
                    if (sumaIds < menorSumaIds) {
                        esNuevoMejor = true;
                        menorSumaIds = sumaIds;
                    } else if (sumaIds == menorSumaIds) {
                        bool esNumeroMenor = false;
                        for (int i = 0; i < 6; i++) {
                            if (idsActuales[i] < mejorEquipoIds[i]) {
                                esNumeroMenor = true;
                                break;
                            } else if (idsActuales[i] > mejorEquipoIds[i]) {
                                break;
                            }
                        }
                        if (esNumeroMenor) {
                            esNuevoMejor = true;
                        }
                    }
                }
            }
            if (esNuevoMejor) {
                menorBajas = resultado.bajas;
                menorDanio = resultado.danioTotal;
                for (int i = 0; i < 6; i++) {
                    mejorEquipoIds[i] = idsActuales[i];
                    
                } cout << "Nuevo mejor equipo encontrado (bajas: " << menorBajas << ", danio: " << menorDanio << "): ";
                for (int i = 0; i < 6; i++) {
                    cout << mejorEquipoIds[i] << " ";
                }
            }
        }
    }

    for (int i = 0; i < cantidadNumoris; ++i) {
        if (!usado[i]) {
            usado[i] = true;
            equipoActual[numEle] = numorisDB[i];
            idsActuales[numEle] = numorisDB[i].id;
            encontrarMejorEquipoBT(numorisDB, cantidadNumoris, tamPer, equipoActual, usado, numEle + 1, numorisOscuros, cantidadPorPiso, 
            pisos, mejorEquipoIds, idsActuales, equipoSize+1, menorDanio, menorBajas, menorSumaIds);
            usado[i] = false; // Backtrack
        }
    }
}

void encontrarMejorEquipo(Numor* numorisDB, int cantidadNumoris, Numor** numorisOscuros, int* cantidadPorPiso, int pisos, int mejorEquipoIds[6]) {
    int menorDanio = VMAX;
    int menorBajas = VMAX;
    int menorSumaIds = VMAX;
    int k = 6;
    Numor equipoActual[6];
    bool usado[cantidadNumoris] = {false};
    int idsActuales[6];

    encontrarMejorEquipoBT(numorisDB, cantidadNumoris, k, equipoActual, usado, 0, numorisOscuros, cantidadPorPiso, pisos, mejorEquipoIds, idsActuales, 0, menorDanio, menorBajas, menorSumaIds);
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
    cout << "Ingrese el numero de la Torre (X): ";
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

    int* cantidadPorPiso = new int[pisos];
    Numor** numorisOscuros = prepararNumorisOscuros(Torre, pisos, cantidadPorPiso, numorisDB, N);

    // Encontrar el mejor equipo
    int mejorEquipoIds[6] = {0}; // Inicializar con ceros


    encontrarMejorEquipo(numorisDB, N, numorisOscuros, cantidadPorPiso, pisos, mejorEquipoIds);

    // Mostrar el resultado
    cout << "\nEl mejor equipo para derrotar la torre es:\n";
    for (int i = 0; i < 6; i++) {
        cout << mejorEquipoIds[i];
        if (i < 5) cout << " ";
    }
    cout << endl;

    // Liberar memoria
    for (int i = 0; i < pisos; i++) {
        delete[] Torre[i];
        delete[] numorisOscuros[i];
    }
    delete[] Torre;
    delete[] numorisOscuros;
    delete[] cantidadPorPiso;
    delete[] numorisDB;

    system("pause");
    return 0;
}
