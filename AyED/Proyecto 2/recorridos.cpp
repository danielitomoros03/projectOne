#include<iostream>
using namespace std;


struct Nodo{
    int valor;
    Nodo* izquierdo;
    Nodo* derecho;
    Nodo(int val) : valor(val), izquierdo(nullptr), derecho(nullptr){}
};

void BFS(Nodo* raiz){
    if(!raiz) return; // El arbol esta vacio

    Queue<Nodo*> cola;
    cola.Enqueue(raiz);

    while(!cola.IsEmpty()){
        Nodo* nodoActual = cola.Head();
        cola.Dequeue();

        cout << nodoActual -> valor << " "; //Aqui es donde se hace la operacion
        if(nodoActual -> izquierdo != nullptr){
            cola.Enqueue(nodoActual -> izquierdo);
        }
        if(nodoActual -> derecho){
            cola.Enqueue(nodoActual -> derecho);
        }
    }
    cout << endl;
}

void inordenIte(Nodo* raiz){
    if(!raiz) return;

    Stack<Nodo*> pila;
    Nodo* nodoActual = raiz;

    cout << "Recorrido inorden (con pila) ";

    while(nodoActual != nullptr || !pila.IsEmpty()){
        //Recorre hasta el nodo mas a la izquierda, empujando los nodos encontrados a la pila

        while(nodoActual != nullptr){
            pila.push(nodoActual);
            nodoActual = nodoActual -> izquierdo;
        }

        //Una vez que no haya mas hijos izquierdos, procesa el nodo superior de la pila
        nodoActual = pila.top();
        pila.pop();
        cout << nodoActual -> valor << " ";

        //Ahora se mueve al hijo derecho
        nodoActual = nodoActual -> derecho;

    } cout << endl;
}

void preordenIte(Nodo* raiz){
    if(!raiz) return;

    Stack<Nodo*> pila;
    pila.push(raiz);

    cout << "Recorrido preorden (con pila) ";

    while(!pila.IsEmpty()){
        Nodo* nodoActual = pila.top();
        pila.pop();

        cout << nodoActual -> valor << " ";

        //Empuja el hijo derecho primero, para que el izquierdo se procese antes
        if(nodoActual -> derecho){
            pila.push(nodoActual -> derecho);
        }
        if(nodoActual -> izquierdo){
            pila.push(nodoActual -> izquierdo);
        }
    } cout << endl;
}

void postOrdenIte(Nodo* raiz){
    if(!raiz) return;

    Stack<Nodo*> pila1;
    Stack<Nodo*> pila2;

    pila1.push(raiz);

    while(!pila1.IsEmpty()){
        Nodo* nodoActual = pila1.top();
        pila1.pop();
        pila2.push(nodoActual); //Aqui mueve el nodo

        //Empuja los hijos (izquierdo primero, luego derecho) a pila1
        if(nodoActual -> izquierdo != nullptr){
            pila1.push(nodoActual -> izquierdo);
        }
        if(nodoActual -> derecho != nullptr){
            pila1.push(nodoActual -> derecho);
        }
    }

    cout << "Recorrido postOrden (con pila) ";

    while(!pila2.IsEmpty()){
        cout << pila2.top() -> valor << " "; //Visita el nodo
        pila2.pop();
    }
    cout << endl;
}
