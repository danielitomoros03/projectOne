#include<iostream>
#include<string>

using namespace std;

// Funcion para determinar la cantidad de enlaces qe puede tener un nodo
int tipoNodo(char nodo) {
    if (nodo == 'H') return 1;
    if (nodo == 'E') return 4;
    if (nodo == 'M') return 2;
    if (nodo == 'C') return 2;
    return 0;
}

// Función para determinar la energía de un enlace entre dos tipos de nodos
int energia(char nodo1, char nodo2) {
    if (nodo1 == 'H' || nodo2 == 'H') {
        return 0; // Estable
    }

    if ((nodo1 == 'E' && (nodo2 == 'E' || nodo2 == 'M')) || (nodo2 == 'E' && (nodo1 == 'E' || nodo1 == 'M'))) {
        return 1; // De Aporte
    }
    if ((nodo1 == 'M' && (nodo2 == 'M' || nodo2 == 'E')) || (nodo2 == 'M' && (nodo1 == 'M' || nodo1 == 'E'))) {
        return 1; // De Aporte
    }

    if ((nodo1 == 'C' && (nodo2 == 'E' || nodo2 == 'M' || nodo2 == 'C')) || (nodo2 == 'C' && (nodo1 == 'E' || nodo1 == 'M' || nodo1 == 'C'))) {
        return -1; // Inestable
    }

    return 0; // Caso por defecto
}

bool esDigito(char d){
    return (d >= '2' && d <= '9');
}

struct Info{
    char* arr;
    int tam;
};


Info expandirCadena(const string& cadena) {
    int h = 0, e = 0, m = 0, c = 0;
    
    //Obtener cada tipo de nodo y la cantidad
    for (int i = 0; i < cadena.length(); ++i) {
        char d = cadena[i];
        if (d == 'H' || d == 'E' || d == 'M' || d == 'C') {
            int count = 0;
            if (i + 1 < cadena.length() && esDigito(cadena[i + 1])) {
                string numStr = "";
                while (i + 1 < cadena.length() && esDigito(cadena[i + 1])) {
                    numStr += cadena[i + 1];
                    i++;
                }
                count = stoi(numStr);
            } else {
                count = 1;
            }

            if (d == 'H') {
                h += count;
            } else if (d == 'E') {
                e += count;
            } else if (d == 'M') {
                m += count;
            } else if (d == 'C') {
                c += count;
            }
        }
    }

    int total = h + e + m + c;
    char* nodos = new char[total]; 

    int count = 0;
    for (int i = 0; i < h; ++i) nodos[count++] = 'H';
    for (int i = 0; i < e; ++i) nodos[count++] = 'E';
    for (int i = 0; i < m; ++i) nodos[count++] = 'M';
    for (int i = 0; i < c; ++i) nodos[count++] = 'C';

    Info info;
    info.arr = nodos;
    info.tam = total;
    return info;
}

void swap(char& a, char& b){
    int aux = a;
    a = b; b = aux;
}

void permutar(char arr[], int k, int tam){

    if (k == tam) {
        for (int i = 0; i < tam; ++i) {
            cout << arr[i];
        }
        cout << " ";
    } else {
        for (int i = k; i < tam; ++i) {
            swap(arr[k], arr[i]);
            permutar(arr, k + 1, tam);
            swap(arr[k], arr[i]); // Backtrack
        }
    }
}


// Estructura para representar un Nodo en el grafo
struct NodoArcano {
    char tipo;
    int id; // Identificador único 
    int maxEnlaces; // Máximo de enlaces que puede formar
    int usados; // Conteo de enlaces ya formados
    
    NodoArcano(char t = ' ', int i = 0) : tipo(t), id(i), usados(0) {
        maxEnlaces = tipoNodo(tipo);
    }
};

int main(){

    string cadena;
    cin >> cadena;
    Info info = expandirCadena(cadena);
    char* arr = new char[info.tam];

    for(int i = 0; i < info.tam ; i++){
        arr[i] = info.arr[i];
    }
    permutar(arr, 0, info.tam);

    return 0;
}