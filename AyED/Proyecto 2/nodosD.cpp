#include<iostream>
#include<string>
using namespace std;
int nodeCount = 0; // Contador de nodos creados

// Función para determinar la energía de un enlace entre dos tipos de nodos
int energy(const char &nodeOne, const char &nodeTwo) {
    if (nodeOne == 'H' || nodeTwo == 'H') {
        return 0; // Estable
    }

    if ((nodeOne == 'E' && (nodeTwo == 'E' || nodeTwo == 'M')) || (nodeTwo == 'E' && (nodeOne == 'E' || nodeOne == 'M'))) {
        return 1; // De Aporte
    }
    if ((nodeOne == 'M' && (nodeTwo == 'M' || nodeTwo == 'E')) || (nodeTwo == 'M' && (nodeOne == 'M' || nodeOne == 'E'))) {
        return 1; // De Aporte
    }

    if ((nodeOne == 'C' && (nodeTwo == 'E' || nodeTwo == 'M' || nodeTwo == 'C')) || (nodeTwo == 'C' && (nodeOne == 'E' || nodeOne == 'M' || nodeOne == 'C'))) {
        return -1; // Inestable
    }

    return 0; // Caso por defecto
}

bool isDigit(const char& d){
    return (d >= '2' && d <= '9');
}

/*
Content extendString(const string& stringAux) {
    int h = 0, e = 0, m = 0, c = 0;
    
    //Obtener cada tipo de nodo y la cantidad
    for (int i = 0; i < stringAux.length(); ++i) {
        char d = stringAux[i];
        if (d == 'H' || d == 'E' || d == 'M' || d == 'C') {
            int count = 0;
            if (i + 1 < stringAux.length() && isDigit(stringAux[i + 1])) {
                string numStr = "";
                while (i + 1 < stringAux.length() && isDigit(stringAux[i + 1])) {
                    numStr += stringAux[i + 1];
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

    Content info;
    info.arr = nodos;
    info.size = total;
    return info;
}
*/

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
            swap(arr[k], arr[i]); // Backtraking
        }
    }
}

// Estructura para representar un Nodo en el grafo
struct Node{
    char type; // Tipo de nodo: 'H', 'E', 'M', 'C'
    int id; // Identificador único 
    int maxLinks; // Máximo de enlaces que puede formar
    int used; // Conteo de enlaces ya formados
    Node *next; // Puntero al siguiente nodo
    
    Node(char t = ' ', int i = 0) : type(t), id(i), used(0) {
        if (type == 'H') maxLinks = 1;
        else if (type == 'E') maxLinks = 4;
        else if (type == 'M') maxLinks = 2;
        else if (type == 'C') maxLinks = 2;
        else maxLinks = 0;
    }
};

// void createNodes(const string& input){}

int main(){
    string aux;
    cin >> aux;
    /* Content info = extendString(aux);
    char* arr = new char[info.size];
    for(int i = 0; i < info.size ; i++){
        arr[i] = info.arr[i];
    } */

    for(int i = 0; i < aux.length()-1 ; i++){
        if(!isDigit(aux[i])) continue; // Ignorar si no es un dígito
        // Crear un nodo con el tipo y el identificador
        char d = aux[i];
        Node nodo(d, nodeCount++);
    }
    // permutar(arr, 0, info.size);

    return 0;
}