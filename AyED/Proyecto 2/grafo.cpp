#include <iostream>
using namespace std;

const int MAX_VERTICES = 10;

class Grafo {
private:
    int numVertices;
    int matrizAdyacencia[MAX_VERTICES][MAX_VERTICES];
    bool visitado[MAX_VERTICES];

    int dfsCamino(int nodo, int& nodoFinal, int profundidad) {
        visitado[nodo] = true;
        if (profundidad > nodoFinal) nodoFinal = nodo;

        int maxDistancia = profundidad;
        for (int i = 0; i < numVertices; ++i) {
            if (matrizAdyacencia[nodo][i] == 1 && !visitado[i]) {
                maxDistancia = std::max(maxDistancia, dfsCamino(i, nodoFinal, profundidad + 1));
            }
        }
        return maxDistancia;
    }

    bool dfsCiclo(int v, int padre) {
        visitado[v] = true;
        for (int u = 0; u < numVertices; ++u) {
            if (matrizAdyacencia[v][u]) {
                if (!visitado[u]) {
                    if (dfsCiclo(u, v)) return true;
                } else if (u != padre) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    Grafo(int V) {
        numVertices = V;
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                matrizAdyacencia[i][j] = 0;
            }
        }
    }

    void agregarArista(int u, int v) {
        matrizAdyacencia[u][v] = 1;
        matrizAdyacencia[v][u] = 1;
    }

    int caminoMasLargo() {
        int nodoIntermedio = 0;
        for (int i = 0; i < numVertices; ++i) visitado[i] = false;
        dfsCamino(0, nodoIntermedio, 0);

        int nodoFinal = 0;
        for (int i = 0; i < numVertices; ++i) visitado[i] = false;
        return dfsCamino(nodoIntermedio, nodoFinal, 0);
    }

    bool tieneCiclo() {
        for (int i = 0; i < numVertices; ++i) visitado[i] = false;
        for (int v = 0; v < numVertices; ++v) {
            if (!visitado[v]) {
                if (dfsCiclo(v, -1)) return true;
            }
        }
        return false;
    }
};

int main() {
    Grafo g(5);
    g.agregarArista(0, 1);
    g.agregarArista(1, 2);
    g.agregarArista(1, 3);
    g.agregarArista(3, 4);
    g.agregarArista(4, 0); // Agrega un ciclo

    cout << "Camino más largo: " << g.caminoMasLargo() << endl;
    cout << "¿Tiene ciclo?: " << (g.tieneCiclo() ? "Sí" : "No") << endl;

    return 0;
}
