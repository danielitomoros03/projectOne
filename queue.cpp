#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

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

int main() {
    
    // ================== EJEMPLO DE COLA ==================
    Queue<int> colaNumeros;
    
    std::cout << "\n=== COLA DE NÚMEROS ===" << std::endl;
    colaNumeros.Enqueue(100);
    colaNumeros.Enqueue(200);
    colaNumeros.Enqueue(300);
    colaNumeros.print(); 
    
    std::cout << "Frente: " << colaNumeros.Head() << std::endl;  // 100
    std::cout << "Tamaño: " << colaNumeros.Size() << std::endl;  // 3
    
    colaNumeros.Dequeue();
    std::cout << "Después de Dequeue - Frente: " << colaNumeros.Head() << std::endl;  // 200
    std::cout << "Tamaño: " << colaNumeros.Size() << std::endl;  // 2
    
    // ========== USO CON DIFERENTES TIPOS ==========
    
    Queue<double> colaDecimales;
    colaDecimales.Enqueue(1.23);
    colaDecimales.Enqueue(4.56);
    std::cout << "Frente cola decimales: " << colaDecimales.Head() << std::endl;  // 1.23
    getchar();
    return 0;
}