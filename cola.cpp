#include <iostream>
using namespace std;

class Queue{
private:
    struct Node{
        int data;
        Node *next;
        Node(const int& d, Node *n = nullptr) : data(d), next(n){}
    };
    Node *front;
    Node *rear;
    int count;
public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    ~Queue() {
        Clear();
    }
    void Clear() {
        while (!IsEmpty()) {
            Dequeue();
        }
    }
    bool IsEmpty() const {
        return count == 0;
    }
    void Enqueue(const int& x) {
        // Agrega elemento al final
        Node *newNode = new Node(x);
        if (IsEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    void Dequeue() {
        // Elimina el elemento del frente
        if (IsEmpty()) {
            throw std::out_of_range("Cola vacía");
        }
        Node *temp = front;
        front = front->next;
        delete temp;
        count--;
        if (IsEmpty()) {
            rear = nullptr;
        }
    }
    int Size() const {
        // Tamaño de la cola
        return count;
    }
    int Head() const {
        // Devuelve el elemento del frente
        if (IsEmpty()) {
            throw std::out_of_range("Cola vacía");
        }
        return front->data;
    }
    void Print(){
        // Imprime los elementos de la cola
        if(IsEmpty()) {
            cout << "Cola vacía" << endl;
            return;
        }
        Node *current = front;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

int main(){

    return 0;
}