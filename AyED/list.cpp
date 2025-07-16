#include <iostream>
using namespace std;
struct Node {
    int id;
    Node* next;
    Node* back;

    Node(int val) : id(val), next(nullptr), back(nullptr) {}
};
class LinkedList {
    private:
        Node* head;
    public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }
        head = nullptr;
    }
    void insertFront(int val) {
        Node* newNode = new Node(val); // Crea un nuevo nodo
        newNode->next = head;         // El nuevo nodo apunta a lo que head apuntaba
        head = newNode;               // head ahora apunta al nuevo nodo
    }
    void insertBack(int val) {
        Node* newNode = new Node(val);
        if (head == nullptr) { // Si la lista está vacía, el nuevo nodo es la cabeza
            head = newNode;
            return;
        }
        Node* current = head;
        while (current->next != nullptr) { // Recorre hasta el último nodo
            current = current->next;
        }
        current->next = newNode; // El último nodo ahora apunta al nuevo nodo
    }
    void deleteFront() {
        if (head == nullptr) { // Lista vacía
            cout << "La lista esta vacia. No se puede eliminar." << endl;
            return;
        }
        Node* temp = head;     // Guarda el nodo actual de la cabeza
        head = head->next;     // La cabeza se mueve al siguiente nodo
        delete temp;           // Libera la memoria del nodo anterior de la cabeza
    }
    void deleteByValue(int val) {
        if (head == nullptr) {
            cout << "La lista esta vacia. No se puede eliminar." << endl;
            return;
        }

        // Caso especial: el nodo a eliminar es la cabeza
        if (head->id == val) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* current = head;
        // Recorre hasta que el siguiente nodo tenga el valor o se llegue al final
        while (current->next != nullptr && current->next->id != val) {
            current = current->next;
        }

        if (current->next == nullptr) { // No se encontró el valor
            cout << "Valor " << val << " no encontrado en la lista." << endl;
        } else { // Se encontró el valor en current->next
            Node* temp = current->next; // Guarda el nodo a eliminar
            current->next = temp->next; // El nodo actual salta el nodo a eliminar
            delete temp;                // Libera la memoria
        }
    }
    void display() {
        if (head == nullptr) {
            cout << "Lista vacia." << endl;
            return;
        }
        Node* current = head;
        while (current != nullptr) {
            cout << current->id << " -> ";
            current = current->next;
        }
        cout << "nullptr" << endl;
    }
};
class LinkedDoubleList{
private: 
    Node *head, *tail;
public:
    LinkedDoubleList() : head(nullptr), tail(nullptr){};
    ~LinkedDoubleList(){
        Node *current = head;
        while(current != nullptr){
            Node *nextNode = current -> next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
    };
    void insertFront(int val){
        Node *newNode = new Node(val);
        if(head == nullptr){
            head = newNode;
            tail = newNode;    
        } else {
            newNode -> next = head;
            head -> back = newNode;
        }
    };
    void insertBack(int val){
        Node *newNode = new Node(val);
        if(tail == nullptr){
            head = newNode;     
            tail = newNode;     
        } else {
            newNode -> back = tail;
            tail -> next = newNode;
            tail = newNode;
        }
    };
    void deleteFront(){
        if (head == nullptr){
            cout << "La lista esta vacia. \n";
            return;
        }
        Node *temp = head;
        if(head == tail){
            head = nullptr;
            tail = nullptr;
        } else {
            head = head -> next;
            head -> back = nullptr;
        }
        delete temp;
    };
    void deleteBack(){
        if(tail == nullptr){
            cout << "La lista esta vacia. \n";
            return;    
        }
        Node *temp = tail;
        if(head == tail){
            head = nullptr;
            tail = nullptr;
        } else {
            tail = tail -> back;
            tail -> next = nullptr;
        }
        delete temp;
    };
    void deleteByValue(int val){
        if(head == nullptr){
            cout << "La lista esta vacia. \n"; return;
        }
        Node *current = head;
        while(current !=nullptr && current->id != val){
            current = current -> next;
        }
        if(current == nullptr){
            cout << "Valor " << current->id << " no encontrado en la lista. \n"; return;
        }
        if(current == head){
            deleteFront();return;
        }
        if(current == tail){
            deleteBack(); return;
        }
        current -> back -> next = current -> next;
        current -> next -> back = current -> back;
        delete current;
    };
    void displayForward(){
        if(head == nullptr){
            cout << "La lista esta vacia. \n"; return;
        }
        Node *current = head;
        while(current != nullptr){
            cout << current->id << " <-> ";
            current = current->next;
        }
        cout << "nullptr\n";
    };
    void displayBackward(){
        if(tail == nullptr){
            cout << "La lista esta vacia. \n"; return;
        }
        Node *current = tail;
        while(current != nullptr){
            cout << current->id << " <-> ";
            current = current->back;
        }
        cout << "nullptr\n";
    };
};

int main() {
    LinkedDoubleList myDLL;

    myDLL.displayForward(); // Lista vacía
    myDLL.displayBackward(); // Lista vacía

    myDLL.insertFront(10); // Lista: 10
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.insertBack(5); // Lista: 10 <-> 5
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.insertFront(20); // Lista: 20 <-> 10 <-> 5
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteFront(); // Elimina 20. Lista: 10 <-> 5
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteBack(); // Elimina 5. Lista: 10
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.insertBack(30);
    myDLL.insertFront(40);
    myDLL.insertBack(50);
    // Lista: 40 <-> 10 <-> 30 <-> 50
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteByValue(10); // Elimina 10 (en medio)
    // Lista: 40 <-> 30 <-> 50
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteByValue(40); // Elimina 40 (cabeza)
    // Lista: 30 <-> 50
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteByValue(50); // Elimina 50 (cola)
    // Lista: 30
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteByValue(30); // Elimina 30 (último nodo)
    // Lista: vacía
    myDLL.displayForward();
    myDLL.displayBackward();

    myDLL.deleteByValue(100); // Intenta eliminar de lista vacía
    myDLL.displayForward();

    return 0;
}


#include <iostream>
using namespace std;
struct Node {
    int id;
    Node* next;
    Node(int val) : id(val), next(nullptr) {}
};
class Set{
private:
    Node *head;
public:
    int count;
    Set() : count(0){};
    ~Set(){};
    bool isEmpty() const { // Indica si el conjunto es vacío
        return (head == nullptr) ? true : false;
    }
    void add(int x){
        // Agrega elementos si no es del conjunto
        if(head == nullptr){ head = new Node(x); return; }
        bool aux = false;
        Node *newNode = new Node(x), *temp = head;
        while(temp != nullptr){
            if(temp->id == newNode->id){ aux = true; break;}
            temp = temp -> next; 
        }
        if(aux == true ){ cout << "El entero " << newNode->id <<  " ya existe en el conjunto.\n"; }
        else {
            newNode->next = head;
            head = newNode;
            count++;
            cout << "Elemento " << newNode->id << " agregado. \n";
        }
    }
    void printItems(){
        // Imprime la lista con los elementos
        if(head == nullptr){ cout << "La lista esta vacia.\n" ; return;}
        Node *temp = head;
        while(temp->next != nullptr){
            cout << temp -> id << " -> " ;
            temp = temp -> next;
        }
        cout << "nullptr \n" ;
    }
    /*
        List items(){
            // Retorna la lista con los elementos
            if(head == nullptr){ cout << "La lista esta vacia.\n";}
            return myList;
        }
    */
};
int main(){
    Set conjunto; 
    conjunto.add(1);
    conjunto.add(21);
    conjunto.add(44);
    conjunto.add(2);
    conjunto.add(50);
    conjunto.add(50);
    conjunto.add(10);
    conjunto.add(19);
    conjunto.printItems();
    cout << conjunto.count << endl;
    
    

    return 0;
}