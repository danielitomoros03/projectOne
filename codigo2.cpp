#include <iostream>
#include <string>
#include <fstream>
#include <climits>

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
ResultadoCombate simularCombatePiso(Numor equipo[6], Numor oscuros[], int cantidadOscuros) {
    ResultadoCombate resultado = {0, 0, false};
    
    // Copias de trabajo para no modificar los originales
    Numor equipoCopia[6];
    Numor oscurosCopia[10]; // Máximo 10 Numoris por piso
    
    for (int i = 0; i < 6; i++) {
        equipoCopia[i] = equipo[i];
    }
    
    for (int i = 0; i < cantidadOscuros; i++) {
        oscurosCopia[i] = oscuros[i];
    }
    
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
    
    for (int i = 0; i < pisos; i++) {
        // Restaurar vida de los Numoris del equipo para cada piso
        Numor equipoActual[6];
        for (int j = 0; j < 6; j++) {
            equipoActual[j] = equipo[j];
        }
        
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

// Función para encontrar el mejor equipo
void encontrarMejorEquipo(Numor* numorisDB, int cantidadNumoris, Numor** numorisOscuros, int* cantidadPorPiso, int pisos, int mejorEquipoIds[6]) {
    int menorDanio = INT_MAX;
    int menorBajas = INT_MAX;
    long long menorSumaIds = LLONG_MAX;
    
    // Arreglo temporal para almacenar el equipo actual
    Numor equipoActual[6];
    int idsActuales[6];
    
    // Generar todas las combinaciones posibles de 6 Numoris
    for (int a = 0; a < cantidadNumoris; a++) {
        for (int b = a + 1; b < cantidadNumoris; b++) {
            for (int c = b + 1; c < cantidadNumoris; c++) {
                for (int d = c + 1; d < cantidadNumoris; d++) {
                    for (int e = d + 1; e < cantidadNumoris; e++) {
                        for (int f = e + 1; f < cantidadNumoris; f++) {
                            // Formar el equipo actual
                            equipoActual[0] = numorisDB[a];
                            equipoActual[1] = numorisDB[b];
                            equipoActual[2] = numorisDB[c];
                            equipoActual[3] = numorisDB[d];
                            equipoActual[4] = numorisDB[e];
                            equipoActual[5] = numorisDB[f];
                            
                            idsActuales[0] = numorisDB[a].id;
                            idsActuales[1] = numorisDB[b].id;
                            idsActuales[2] = numorisDB[c].id;
                            idsActuales[3] = numorisDB[d].id;
                            idsActuales[4] = numorisDB[e].id;
                            idsActuales[5] = numorisDB[f].id;
                            
                            // Simular el combate contra la torre
                            ResultadoCombate resultado = simularCombateTorre(equipoActual, numorisOscuros, cantidadPorPiso, pisos);
                            
                            // Si el equipo ganó y es mejor que el anterior mejor equipo
                            if (resultado.victoria) {
                                bool esNuevoMejor = false;
                                
                                // Primero comparamos por número de bajas
                                if (resultado.bajas < menorBajas) {
                                    esNuevoMejor = true;
                                } else if (resultado.bajas == menorBajas) {
                                    // Si hay empate en bajas, comparamos por daño total
                                    if (resultado.danioTotal < menorDanio) {
                                        esNuevoMejor = true;
                                    } else if (resultado.danioTotal == menorDanio) {
                                        // Si hay empate en daño, comparamos por suma de IDs
                                        long long sumaIds = 0;
                                        for (int i = 0; i < 6; i++) {
                                            sumaIds += idsActuales[i];
                                        }
                                        
                                        if (sumaIds < menorSumaIds) {
                                            esNuevoMejor = true;
                                            menorSumaIds = sumaIds;
                                        } else if (sumaIds == menorSumaIds) {
                                            // Si hay empate en suma de IDs, comparamos por número formado
                                            // (Este criterio ya está implícito en el orden lexicográfico de los IDs)
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
                                
                                // Si es un nuevo mejor equipo, actualizamos
                                if (esNuevoMejor) {
                                    menorBajas = resultado.bajas;
                                    menorDanio = resultado.danioTotal;
                                    
                                    // Guardar los IDs del mejor equipo
                                    for (int i = 0; i < 6; i++) {
                                        mejorEquipoIds[i] = idsActuales[i];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Función para preparar los Numoris oscuros desde la matriz de la torre
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

    // Preparar los Numoris oscuros y las cantidades por piso
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