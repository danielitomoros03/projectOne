#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int MAX_NODOS = 20;

struct Nodo {
    char tipo;
    int enlacesMax;
    int enlacesUsados = 0;
};

class Grafo {
private:
    int numVertices;
    Nodo nodos[MAX_NODOS];
    int matriz[MAX_NODOS][MAX_NODOS];

public:
    Grafo(char tipos[], int n) {
        numVertices = n;

        // Inicializamos matriz de adyacencia con ceros
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                matriz[i][j] = 0;
            }
        }

        // Configuramos los nodos con tipo y enlaces máximos
        for (int i = 0; i < n; ++i) {
            nodos[i].tipo = tipos[i];
            nodos[i].enlacesUsados = 0;

            switch (tipos[i]) {
                case 'H': nodos[i].enlacesMax = 1; break;
                case 'E': nodos[i].enlacesMax = 4; break;
                case 'M': nodos[i].enlacesMax = 2; break;
                case 'C': nodos[i].enlacesMax = 2; break;
            }
        }

        // Creamos las conexiones según reglas
        conectarNodos();
    }

    int tipoEnlace(char a, char b) {

        if (a == 'H' || b == 'H') return 0;                      // Estable
        if ((a == 'E' && (b == 'M' || b == 'E')) ||
            (b == 'E' && (a == 'M' || a == 'E'))) return 1;      // Aporte
        if (a == 'M' && b == 'M') return 1;
        if (a == 'C' || b == 'C') return -1;                     // Inestable
        return -1;
    }

    void conectarNodos() {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = i + 1; j < numVertices; ++j) {
                if (nodos[i].enlacesUsados < nodos[i].enlacesMax &&
                    nodos[j].enlacesUsados < nodos[j].enlacesMax) {
                    
                    int tipo = tipoEnlace(nodos[i].tipo, nodos[j].tipo);
                    matriz[i][j] = matriz[j][i] = tipo;

                    if (tipo != 0) { // Aporte o inestable sí consumen enlaces
                        nodos[i].enlacesUsados++;
                        nodos[j].enlacesUsados++;
                    } else if (nodos[i].tipo != 'H' && nodos[j].tipo != 'H') {
                        nodos[i].enlacesUsados++;
                        nodos[j].enlacesUsados++;
                    } else if (nodos[i].tipo == 'H') {
                        nodos[i].enlacesUsados++;
                    } else if (nodos[j].tipo == 'H') {
                        nodos[j].enlacesUsados++;
                    }
                }
            }
        }
    }

    void dfsCamino(int nodo, bool visitado[], int energiaActual, int longitud,
                    int& mejorEnergia, int& mejorLongitud) {
        visitado[nodo] = true;

        for (int i = 0; i < numVertices; ++i) {
            if (matriz[nodo][i] && !visitado[i]) {
                dfsCamino(i, visitado, energiaActual + matriz[nodo][i], longitud + 1,
                            mejorEnergia, mejorLongitud);
            }
        }

        if (energiaActual > mejorEnergia ||
            (energiaActual == mejorEnergia && longitud > mejorLongitud)) {
            mejorEnergia = energiaActual;
            mejorLongitud = longitud;
        }

        visitado[nodo] = false;
    }
};

int expandirEntrada(const string& entrada, char tipos[]) { //Expande la entrada y la coloca en el arreglo
    int idx = 0;
    for (size_t i = 0; i < entrada.length(); ++i) {
        if (std::isalpha(entrada[i])) { //Retorna true si es un numero
            tipos[idx++] = entrada[i];
        } else if (std::isdigit(entrada[i]) && idx > 0) { 
            int rep = entrada[i] - '0' - 1;
            char anterior = tipos[idx - 1];
            for (int r = 0; r < rep; ++r)
                tipos[idx++] = anterior;
        }
    }
    return idx;
}

int main() {
    


    return 0;
}
