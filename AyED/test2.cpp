#include <iostream>
using namespace std;

const int MAX_ITEMS = 100;

struct Item {
    int valor;
    int peso;
};

// Variables globales
int mejor_valor = -1;
bool mejor_solucion[MAX_ITEMS] = { false };

void imprimirSolucion(Item items[], int num_items, bool seleccion[], int valor, int peso) {
    cout << "Items: ";
    for (int i = 0; i < num_items; i++) {
        if (seleccion[i]) {
            cout << "Item" << i << " v:" << items[i].valor << ", p:" << items[i].peso << " ";
        }
    }
    cout << "| Valor total: " << valor << ", Peso total: " << peso << endl << endl;
}

// Buscar la mejor solución (mayor valor sin exceder capacidad)
void mochilaMejorSolucion(Item items[], int num_items, int capacidad, int index, int valor_actual, int peso_actual, bool seleccion_actual[]) {
    if (peso_actual > capacidad) return;

    if (index == num_items) {
        if (valor_actual > mejor_valor) {
            mejor_valor = valor_actual;
            for (int i = 0; i < num_items; i++) {
                mejor_solucion[i] = seleccion_actual[i];
            }
        }
        return;
    }

    // No tomar el item
    seleccion_actual[index] = false;
    mochilaMejorSolucion(items, num_items, capacidad, index + 1, valor_actual, peso_actual, seleccion_actual);

    // Tomar el item
    seleccion_actual[index] = true;
    mochilaMejorSolucion(items, num_items, capacidad, index + 1, valor_actual + items[index].valor, peso_actual + items[index].peso, seleccion_actual);
}

int main() {
    Item items[MAX_ITEMS] = {
        {60, 10}, {30, 60}, {25, 24}, {3, 14}
    };
    int num_items = 4;
    int capacidad = 50;
    bool seleccion_actual[MAX_ITEMS] = { false };

    // Buscar la mejor solución
    mochilaMejorSolucion(items, num_items, capacidad, 0, 0, 0, seleccion_actual);

    if (mejor_valor != -1) {
        cout << "MEJOR SOLUCION ENCONTRADA:\n";
        int peso_total = 0;
        for (int i = 0; i < num_items; i++)
            if (mejor_solucion[i]) peso_total += items[i].peso;
        imprimirSolucion(items, num_items, mejor_solucion, mejor_valor, peso_total);
    } else {
        cout << "No hay solucion valida.\n";
    }

    return 0;
}