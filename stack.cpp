#include <iostream>
#include <stdexcept>
#include <exception>
#include <string>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n = nullptr) : data(d), next(n) {}
    };
    
    Node* topNode;  // Puntero al tope
    int count;      // Contador de elementos

public:
    // Constructor
    Stack() : topNode(nullptr), count(0) {}

    // Destructor
    ~Stack() {
        Clear();
    }

    // Vacía la pila
    void Clear() {
        while (!IsEmpty()) {
            Pop();
        }
    }

    // Verifica si está vacía
    bool IsEmpty() const {
        return count == 0;
    }

    // Agrega elemento al tope
    void Push(const T& x) {
        topNode = new Node(x, topNode);
        count++;
    }

    // Elimina el elemento del tope
    void Pop() {
        if (IsEmpty()) {
            throw std::out_of_range("Pila vacía");
        }
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        count--;
    }

    // Tamaño de la pila
    int Size() const {
        return count;
    }

    // Acceso al elemento tope
    T Top() const {
        if (IsEmpty()) {
            throw std::out_of_range("Pila vacía");
        }
        return topNode->data;
    }
};

int main() {
    // ================== EJEMPLO DE PILA ==================
    Stack<std::string> pilaLibros;
    
    std::cout << "=== PILA DE LIBROS ===" << std::endl;
    pilaLibros.Push("El Principito");
    pilaLibros.Push("Cien años de soledad");
    pilaLibros.Push("1984");
    
    std::cout << "Tope: " << pilaLibros.Top() << std::endl;  // 1984
    std::cout << "Tamaño: " << pilaLibros.Size() << std::endl;  // 3
    
    pilaLibros.Pop();
    std::cout << "Después de Pop - Tope: " << pilaLibros.Top() << std::endl;  // Cien años...
    std::cout << "Tamaño: " << pilaLibros.Size() << std::endl;  // 2
    
   
    // ========== USO CON DIFERENTES TIPOS ==========
    Stack<char> pilaLetras;
    pilaLetras.Push('A');
    pilaLetras.Push('B');
    pilaLetras.Push('C');
    std::cout << "\nTope pila letras: " << pilaLetras.Top() << std::endl;  // C
    
    // Prueba de vaciado
    Stack<int> pilaTemporal;
    pilaTemporal.Push(42);
    std::cout << "\nPila temporal vacía? " << std::boolalpha << pilaTemporal.IsEmpty() << std::endl;
    pilaTemporal.Pop();
    std::cout << "Pila temporal vacía después de Pop? " << pilaTemporal.IsEmpty() << std::endl;
    
    return 0;
}