#include <iostream>
using namespace std;
class Stack{
private:
    struct Node{
        int data;
        Node* next;
        Node(const int& d, Node *n = nullptr) : data(d), next(n){}
    };
    Node *topNode;
    int count;
public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack(){ clear(); }
    void clear(){
        while(!isEmpty()){
            pop();
        }
    }
    bool isEmpty() const {
        return count == 0;
    }
    void push(const int& x) {
        // Agrega un elemento al tope
        topNode = new Node(x, topNode);
        count++;
    }
    void pop() {
        // Elimina el elemento del tope
        if (isEmpty()) {
            throw std::out_of_range("Pila vacia");
        }
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        count--;
    }
    int size() const {
        return count;
    }
    int top() const {
        if (isEmpty()) {
            throw std::out_of_range("Pila vacia");
        }
        return topNode->data;
    }
};

int main(){

    return 0;
}