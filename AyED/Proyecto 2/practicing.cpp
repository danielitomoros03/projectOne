#include <iostream>
#include <string>
using namespace std;
class BST {
private:
    struct Nodo {
        int valor;
        string nombre;
        float montoGastado;
        int umbralMin;
        int unmbralMax;
        Nodo* izquierdo;
        Nodo* derecho;
        Nodo(int v, float m) : valor(v), montoGastado(m), izquierdo(nullptr), derecho(nullptr), nombre(' ') umbralMin(1), umbralMax(50)  {
            if(montoGastado >= umbralMin && umbralMax >= montoGastado) continue;
            else cout << "El monto esta fuera del rango de " << umbralMin << " y " << umbralMax << endl; 
        }
    };
    Nodo* raiz;
    Nodo* insert(Nodo* nodo, int valor, float montoGastado) {
        if (!nodo) {
            return new Nodo(valor, montoGastado);
        }
        if (valor < nodo->valor) {
            nodo->izquierdo = insert(nodo->izquierdo, valor, montoGastado);
        }
        else {
            nodo->derecho = insert(nodo->derecho, valor, montoGastado);
        }
        return nodo;
    }
    Nodo* eliminar(Nodo* nodo, int valor) {
        if (!nodo) return nullptr;

        if (valor < nodo->valor) {
            nodo->izquierdo = eliminar(nodo->izquierdo, valor);
        }

        else if (valor > nodo->valor) {
            nodo->derecho = eliminar(nodo->derecho, valor);
        }
        else {
            //Caso 1: hoja o 0 hijos
            if (!nodo->izquierdo && !nodo->derecho) {
                delete nodo;
                return nullptr;
            }

            //Caso 2: tiene hijo izquierdo o 1 hijo
            if (!nodo->izquierdo) {
                Nodo* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
             
            //Caso 2b: tiene hijo derecho o 1 hijo
            if (!nodo->derecho) {
                Nodo* temp = nodo->derecho;
                delete nodo;
                return temp;
            }

            //Caso 3 dos hijos
            Nodo* temp = min(nodo->derecho);
            nodo->valor = temp->valor;
            nodo->derecho = eliminar(nodo->derecho, temp->valor);
        }

        return nodo;
    }
    void  inorden(Nodo* nodo) const {
        if (!nodo) return;
        inorden(nodo->izquierdo);
        std::cout << nodo->valor << " ";
        inorden(nodo->derecho);
    }
    void  preorden(Nodo* nodo) const {
        if (!nodo) return;
        cout << nodo->valor << " ";
        preorden(nodo->izquierdo);
        preorden(nodo->derecho);
    }
    void preOrdenInvertido(Nodo* nodo){
        if (!nodo) return;
        cout << nodo->valor << " ";
        preOrdenInvertido(nodo->derecho);
        preOrdenInvertido(nodo->izquierdo);
    }
    void  postorden(Nodo* nodo) const {
        if (!nodo) return;
        postorden(nodo->izquierdo);
        postorden(nodo->derecho);
        cout << nodo->valor << " ";
    }
    void treeMirror(Nodo* nodo){
        if(!nodo) return;
        Nodo* temp = nodo->izquierdo;
        nodo->izquierdo = nodo->derecho;
        nodo->derecho = temp;
        treeMirror(nodo->izquierdo);
        treeMirror(nodo->derecho);
    }
public:
    BST() : raiz(nullptr){}
    ~BST() {}
    bool find(Nodo* nodo, int valor) {
        if (!nodo) return false;
        if (valor == nodo->valor) return true;
        return (valor < nodo->valor) ? find(nodo->izquierdo, valor) : find(nodo->derecho, valor);
    }
    Nodo* min(Nodo* nodo) {
        if (!nodo) return nullptr;
        while (nodo && nodo->izquierdo) nodo = nodo->izquierdo;
        return nodo;

    }
    Nodo* max(Nodo* nodo) {
        if (!nodo) return nullptr;
        while (nodo && nodo->derecho) { nodo = nodo->derecho;}
        return nodo;
    }
    void insertar(int valor) {
        raiz = insert(raiz, valor, montoGastado);
    }
    void pre() const {
        cout << "Recorrido en pre orden" << endl;
        preorden(raiz);
        cout << endl;
    }
    void in() const {
        cout << "Recorrido en in orden" << endl;
        inorden(raiz);
        cout << endl;
    }
    void post() const {
        cout << "Recorrido en post orden" << endl;
        postorden(raiz);
        cout << endl;
    }
    void swap(int &a, int &b){
        cout << "Cambiando " << a << " y " << b << " ";
        int aux = a;
        a = b;
        b = aux;
        cout << "por " << a << " y " << b << endl;
    }
    void pres(){
        preorden(raiz);
        cout << endl;
        preOrdenInvertido(raiz);
    }
    void mirror(){treeMirror(raiz);}
    float porcentajeClientesPorMonto(Nodo* nodo, float total = 0, int contador = 0) const {
        if (nodo->derecho == NULL || nodo->izquierdo == NULL){ 
            cout << "Nodos: " << contador << " | Porcentaje: " << total/(float)(contador) << endl;  
            return total/(float)(contador);
        }
        nodo->montoGastado += total;
        return porcentajeClientesPorMonto(nodo->izquierdo, total, contador+1) + porcentajeClientesPorMonto(nodo->derecho, total, contador+1); 
    }
    void aux(){ porcentajeClientesPorMonto(raiz); }
};
int main() {
    BST tree;
    tree.insertar(50, 5.0); tree.insertar(30, 10.0); tree.insertar(70, 40.0); tree.insertar(20, 35.0); tree.insertar(60, 50.0); tree.insertar(80, 5.0); tree.insertar(10, 9.0);    
    // tree.pre(); cout << endl;
    // tree.mirror();
    tree.aux();
    return 0;
}