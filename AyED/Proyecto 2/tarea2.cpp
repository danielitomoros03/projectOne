#include<iostream>
#include<string>
#include<chrono>
using namespace std;
using namespace chrono;

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };
    
    Node* front;   // Primer elemento
    Node* rear;    // Último elemento
    int count;     // Contador de elementos

public:
    typedef Node* tPosition;
    // Constructor
    Queue() : front(nullptr), rear(nullptr), count(0) {}

    // Destructor
    ~Queue() {
        Clear();
    }

    // Vacía la cola
    void Clear() {
        while (!IsEmpty()) {
            Dequeue();
        }
    }

    // Verifica si está vacía
    bool IsEmpty() const {
        return count == 0;
    }

    // Agrega elemento al final
    void Enqueue(const T& x) {
        Node* newNode = new Node(x);
        if (IsEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }

    // Elimina el elemento del frente
    void Dequeue() {
        if (IsEmpty()) {
            throw std::out_of_range("Cola vacía");
        }
        Node* temp = front;
        front = front->next;
        delete temp;
        count--;
        
        if (front == nullptr) {
            rear = nullptr;
        }
    }

    // Tamaño de la cola
    int Size() const {
        return count;
    }

    // Acceso al elemento frente
    T Head() const {
        if (IsEmpty()) {
            throw std::out_of_range("Cola vacía");
        }
        return front->data;
    }

    void print(){

        if(IsEmpty()){
            cout << "La cola esta vacia";
            return;
        }

        Node* current = front;

        while(current){
            cout << current -> data << " ";
            current = current -> next;
        }cout << endl;
    }
};

template <typename T>
class Stack{
    private:
        struct Node{
            T data;
            Node* next;
            Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
        };

        Node* topNode;
        int count; //contador de elementos

    public: 

        typedef Node* tPosition;

        Stack() : topNode(nullptr), count(0) {}
        ~Stack(){
            Clear();
        }

         // Constructor de copia (Copia Profunda)
    Stack(const Stack& otra) : topNode(nullptr), count(0) {
        if (!otra.IsEmpty()) { // IsEmpty() ahora es const, así que funciona
            Stack<T> tempStack; // Pila temporal para invertir el orden
            Node* current = otra.topNode;
            while (current != nullptr) {
                tempStack.push(current->data);
                current = current->next;
            }
            // Ahora, se extraen de tempStack y se insertan en 'this' para mantener el orden original
            while (!tempStack.IsEmpty()) {
                this->push(tempStack.Pop());
            }
        }
    }

    // Operador de asignación (Copia Profunda)
    Stack& operator=(const Stack& otra) {
        if (this != &otra) { // Manejar auto-asignación
            Clear(); // Limpiar elementos existentes
            if (!otra.IsEmpty()) { // IsEmpty() ahora es const, así que funciona
                Stack<T> tempStack; // Pila temporal para invertir el orden
                Node* current = otra.topNode;
                while (current != nullptr) {
                    tempStack.push(current->data);
                    current = current->next;
                }
                // Ahora, se extraen de tempStack y se insertan en 'this' para mantener el orden original
                while (!tempStack.IsEmpty()) {
                    this->push(tempStack.Pop());
                }
            }
        }
        return *this;
    }

        bool IsEmpty() const {
            return count == 0;
        }

        T top(){
            if(IsEmpty()){
                throw out_of_range("La pila esta vacia");
            }  return topNode -> data;
        } 

        void next(tPosition& pValue){
            if(pValue != nullptr){
                pValue = pValue -> next;
            }
        }

        void print(){

            tPosition current = topNode;

            cout << "Elementos de la pila " << endl;
            if(IsEmpty()){
                cout << "Pila vacia" << endl;
            } else {
                while(current != nullptr){
                    cout << current -> data << " ";
                    current = current -> next;
                } cout << endl;
            }
        }

        T& get(tPosition v){
            return v -> data;
        }

        void pop(){
            if(IsEmpty()){
                throw std :: out_of_range("La pila esta vacia");
            }
            Node* temp = topNode;
            topNode = topNode -> next;
            delete temp;
            count--;
        }

        T Pop() {
        if (IsEmpty()) {
            throw std::out_of_range("La pila esta vacia");
        }
        Node* temp = topNode;
        T value = temp->data; // Guarda el valor antes de eliminar el nodo
        topNode = topNode->next;
        delete temp; // Libera la memoria
        count--;
        return value; // Devuelve el valor
    }

        void Clear(){
            while(!IsEmpty()){
                pop();
            }
        }

        void push(const T& t){
            topNode = new Node(t, topNode);
            count++;
        }
       
};

class BST{ 
    private:
        struct Nodo{
            int valor;
            Nodo* izquierdo;
            Nodo* derecho;
            Nodo(int v) : valor(v), izquierdo(nullptr), derecho(nullptr){}
        };

    Nodo* raiz;

    Nodo* insert(Nodo* nodo, int valor){
        if(!nodo) return new Nodo(valor);

        if(valor < nodo -> valor){
            nodo -> izquierdo = insert(nodo -> izquierdo, valor);
        } else {
            nodo -> derecho = insert(nodo -> derecho, valor);
        } return nodo;
    }

    public: 

        typedef Nodo* tPosition;
        BST(): raiz(nullptr) {}
        ~BST(){}

        void insertar(int valor) {
            raiz = insert(raiz, valor);
        }

        bool find(Nodo* nodo, int valor){
            if(!nodo) return false;

            if(valor == nodo -> valor) return true;

            return (valor < nodo -> valor) ? find(nodo -> izquierdo, valor): find(nodo -> derecho, valor);
        }

        Nodo* min(Nodo* nodo){
            if(!nodo) return nullptr;

            while(nodo && nodo -> izquierdo){
                nodo = nodo -> izquierdo;
            }
            return nodo;
        }

        Nodo* max(Nodo* nodo){
            if(!nodo) return nullptr;
        
            while(nodo && nodo -> derecho){
                nodo = nodo -> derecho;
            }
            return nodo;
        }

        Nodo* getRaiz(){
            return raiz;
        }

        Nodo* eliminar(Nodo* nodo, int valor){

            if(!nodo) return nullptr;

            if(valor < nodo -> valor){
                nodo -> izquierdo = eliminar(nodo -> izquierdo, valor);
            } else if(valor > nodo -> valor){
                nodo -> derecho = eliminar(nodo -> derecho, valor);
            } else {
                //caso 1: si es hoja o no tiene hijos
                if(!nodo -> izquierdo && !nodo -> derecho){
                    delete nodo;
                    return nullptr;
                }
                //caso 2a: si tiene hijo izquierdo o un hijo
                if(!nodo -> izquierdo){
                    Nodo* temp = nodo -> izquierdo;
                    delete nodo;
                    return temp;
                }
                
                //caso 2b: si tiene hijo derecho o un hijo
                if(!nodo -> derecho){
                    Nodo* temp = nodo -> derecho;
                    delete nodo;
                    return temp;
                }

                //caso 3: si tiene 2 hijos
                Nodo* temp = min(nodo -> derecho);
                nodo -> valor = temp -> valor;
                nodo -> derecho = eliminar(nodo -> derecho, temp -> valor);
            }
            return nodo;
        }

        //Recorridos recursivos
        void inorden(Nodo* nodo, int valor, bool& encontrado, int& count){
            if(!nodo || encontrado) return;
            inorden(nodo -> izquierdo, valor, encontrado, count);
            if(encontrado) return; 

            count++;
            if(nodo -> valor == valor){
                encontrado = true;
                return;
            }
            inorden(nodo -> derecho, valor, encontrado, count);
        }

        void preorden(Nodo* nodo, int valor, bool& encontrado, int& count){
            if(!nodo) return;

            count++;
            if(nodo -> valor == valor) {
                encontrado = true;
                return;
            }

            preorden(nodo -> izquierdo, valor, encontrado, count);
            if(encontrado) return;
            preorden(nodo -> derecho, valor, encontrado, count);
        }

        void postorden(Nodo* nodo, int valor, bool& encontrado, int& count){
            if(!nodo || encontrado) return;

            postorden(nodo -> izquierdo, valor, encontrado, count);
            if(encontrado) return;
            postorden(nodo -> derecho, valor, encontrado, count);
            if(encontrado) return;
            
            count++;
            if(nodo -> valor == valor) {
                encontrado = true;
                return;
            }
        }

        //Funciones para llamar los recorridos e imprimir un mensaje
        void pre(int valor){
            cout << "Preorden recursivo: ";
            bool band = false;
            int count = 0;
            preorden(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados: " << count << endl;
            else cout << " No ha sido encontrado;\n";
        }

        void in(int valor){
            cout << "Inorden recursivo: ";
            bool band = false;
            int count = 0;
            inorden(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados: " << count << endl;
            else cout << " No ha sido encontrado;\n";
        }

        void post(int valor){
            cout << "Postorden recursivo: ";
            bool band = false;
            int count = 0;
            postorden(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados: " << count << endl;
            else cout << "No ha sido encontrado;\n";
        }

        //Recorridos iterativos
        void inordenIte(Nodo* raiz, int valor, bool& band, int& count){
            if(!raiz || band) return;

            Stack<Nodo*> pila;
            Nodo* nodoActual = raiz;

            cout << "Inorden iterativo ";

            while(nodoActual != nullptr || !pila.IsEmpty()){
                //Recorre hasta el nodo mas a la izquierda, empujando los nodos encontrados a la pila

                while(nodoActual){
                    pila.push(nodoActual);
                    nodoActual = nodoActual -> izquierdo;
                }

                //Una vez que no haya mas hijos izquierdos, procesa el nodo superior de la pila
                nodoActual = pila.top();
                pila.pop();
                count++;

                if(nodoActual -> valor == valor){
                    band = true;
                    return;
                }

                //Ahora se mueve al hijo derecho
                nodoActual = nodoActual -> derecho;

            } cout << endl;
        }

        void preordenIte(Nodo* raiz, int valor, bool& band, int& count){
            if(!raiz || band) return;

            Stack<Nodo*> pila;
            pila.push(raiz);

            cout << "Preorden iterativo ";

            while(!pila.IsEmpty()){
                Nodo* nodoActual = pila.top();
                pila.pop();
                count++;

                if(nodoActual -> valor == valor){
                    band = true;
                    return;
                }

                //Empuja el hijo derecho primero, para que el izquierdo se procese antes
                if(nodoActual -> derecho){
                    pila.push(nodoActual -> derecho);
                }
                if(nodoActual -> izquierdo){
                    pila.push(nodoActual -> izquierdo);
                }
            } cout << endl;
        }

        void postOrdenIte(Nodo* raiz, int valor, bool& band, int& count){
            if(!raiz || band) return;

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

            cout << "Postorden iterativo ";

            while(!pila2.IsEmpty()){

                count++;
                if(pila2.top() -> valor == valor){//Visita el nodo
                    band = true;
                    return;
                } 
                pila2.pop();
            }
            cout << endl;
        }
        
        void BFS(Nodo* raiz, int valor, bool& band, int& count){
            if(!raiz || band) return; // El arbol esta vacio

            Queue<Nodo*> cola;
            cola.Enqueue(raiz);

            while(!cola.IsEmpty()){
                Nodo* nodoActual = cola.Head();
                cola.Dequeue();
                count++;

                if(nodoActual -> valor == valor){
                    band = true;
                    return;
                }

                if(nodoActual -> izquierdo != nullptr){
                    cola.Enqueue(nodoActual -> izquierdo);
                }
                if(nodoActual -> derecho){
                    cola.Enqueue(nodoActual -> derecho);
                }
            }
            cout << endl;
        }  

        void printTree() {
            printTreeHelper(raiz, 0);
        }

        void printTreeHelper(Nodo* nodo, int nivel) {
            if (!nodo) return;
            printTreeHelper(nodo->derecho, nivel + 1);
            for (int i = 0; i < nivel; ++i) cout << "   ";
            cout << nodo->valor << endl;
            printTreeHelper(nodo->izquierdo, nivel + 1);
        }


        //Funciones para llamar a los recorridos
        void postIte(int valor){
            bool band = false;
            int count = 0;
            postOrdenIte(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados " << count << endl;
            else cout << "No ha sido encontrado;\n";
        }

        void preIte(int valor){
            bool band = false;
            int count = 0;
            preordenIte(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados " << count << endl;
            else cout << "No ha sido encontrado;\n";
        }

        void inIte(int valor){
            bool band = false;
            int count = 0;
            inordenIte(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados: " << count << endl;
            else cout << "No ha sido encontrado;\n";
        }

        void anchura(int valor){
            cout <<"Recorrido por Anchura: ";
            bool band = false;
            int count = 0;
            BFS(raiz, valor, band, count);
            if(band) cout << valor << " ha sido encontrado. Nodos visitados " << count << endl;
            else cout << "No ha sido encontrado;\n";
        }
};

class Timer {
    chrono::high_resolution_clock::time_point startTime, endTime;
public:
    void start() {
        startTime = chrono::high_resolution_clock::now();
    }
    void stop() {
        endTime = chrono::high_resolution_clock::now();
    }
    void printTime(const string &aux) {
        auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
        cout << "Tiempo transcurrido para " << aux << ": " << duration.count() << " microsegundos." << endl;
        cout << endl;
        cout << endl;
    }
};
int main(){
    BST tree;
    int valores[] = {50, 30, 70, 20, 15, 8, 60};
    int n = sizeof(valores) / sizeof(valores[0]);
    for (int i = 0; i < n; i++) {
        tree.insertar(valores[i]);
    }
    tree.printTree();

    Timer timeOne, timeTwo, timeThree, timeFour, timeFive, timeSix, timeSeven;
    timeOne.start(); tree.post(50); timeOne.stop(); timeOne.printTime("Post-orden recursivo");
    timeTwo.start(); tree.postIte(50);; timeTwo.stop(); timeTwo.printTime("Post-orden Iterativo");
    
    timeThree.start(); tree.pre(15);  timeThree.stop(); timeThree.printTime("Pre-orden recursivo");
    timeFour.start(); tree.preIte(15); timeFour.stop(); timeFour.printTime("Pre-orden iterativo");
    
    timeFive.start(); tree.in(70); timeFive.stop(); timeFive.printTime("In-orden recursivo");
    timeSix.start(); tree.inIte(70); timeSix.stop(); timeSix.printTime("In-orden iterativo");
    
    timeSeven.start(); tree.anchura(30); timeSeven.stop(); timeSeven.printTime("Anchura");

    return 0;
}
