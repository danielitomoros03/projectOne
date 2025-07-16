#include <iostream>
#include <stdexcept>

// Clase Node: Representa un elemento no nulo en la matriz
template <typename T>
class Node {
public:
    T tInfo;
    int iIdF, iIdC;        // Coordenadas (fila, columna)
    Node<T>* pNextR;        // Puntero al siguiente nodo en la misma fila
    Node<T>* pNextC;        // Puntero al siguiente nodo en la misma columna

    Node(T info, int idF, int idC) : 
        tInfo(info), iIdF(idF), iIdC(idC), pNextR(nullptr), pNextC(nullptr) {}
};

// Clase NodeR: Encabezado de fila
template <typename T>
class NodeR {
public:
    int iIdR;               // ID de la fila
    T tInfo;                // Información adicional (opcional)
    NodeR<T>* pNext;        // Puntero al siguiente encabezado de fila
    Node<T>* pFirst;        // Puntero al primer nodo de la fila

    NodeR(int idR) : 
        iIdR(idR), pNext(nullptr), pFirst(nullptr) {}
};

// Clase NodeC: Encabezado de columna
template <typename T>
class NodeC {
public:
    int iIdC;               // ID de la columna
    NodeC<T>* pNext;        // Puntero al siguiente encabezado de columna
    Node<T>* pFirst;        // Puntero al primer nodo de la columna

    NodeC(int idC) : 
        iIdC(idC), pNext(nullptr), pFirst(nullptr) {}
};

// Clase HeadNodeM: Cabecera principal de la matriz
template <typename T>
class HeadNodeM {
public:
    NodeR<T>* pFirstR;      // Primera fila
    NodeC<T>* pFirstC;      // Primera columna

    HeadNodeM() : pFirstR(nullptr), pFirstC(nullptr) {}
};

// Clase SparseMatrix: Implementación de matriz dispersa
template <typename T>
class SparseMatrix {
private:
    HeadNodeM<T>* Matrix;   // Cabecera principal

    // Busca o crea un encabezado de fila
    NodeR<T>* getOrCreateRow(int iIdR) {
        NodeR<T>** p = &(Matrix->pFirstR);
        while (*p != nullptr && (*p)->iIdR < iIdR) {
            p = &((*p)->pNext);
        }
        
        if (*p == nullptr || (*p)->iIdR != iIdR) {
            NodeR<T>* newRow = new NodeR<T>(iIdR);
            newRow->pNext = *p;
            *p = newRow;
            return newRow;
        }
        return *p;
    }

    // Busca o crea un encabezado de columna
    NodeC<T>* getOrCreateCol(int iIdC) {
        NodeC<T>** p = &(Matrix->pFirstC);
        while (*p != nullptr && (*p)->iIdC < iIdC) {
            p = &((*p)->pNext);
        }
        
        if (*p == nullptr || (*p)->iIdC != iIdC) {
            NodeC<T>* newCol = new NodeC<T>(iIdC);
            newCol->pNext = *p;
            *p = newCol;
            return newCol;
        }
        return *p;
    }

public:
    SparseMatrix() {
        Matrix = new HeadNodeM<T>();
    }

    ~SparseMatrix() {
        // Liberar memoria de filas
        NodeR<T>* currentRow = Matrix->pFirstR;
        while (currentRow != nullptr) {
            NodeR<T>* nextRow = currentRow->pNext;
            // Liberar nodos de la fila
            Node<T>* node = currentRow->pFirst;
            while (node != nullptr) {
                Node<T>* nextNode = node->pNextR;
                delete node;
                node = nextNode;
            }
            delete currentRow;
            currentRow = nextRow;
        }

        // Liberar memoria de columnas
        NodeC<T>* currentCol = Matrix->pFirstC;
        while (currentCol != nullptr) {
            NodeC<T>* nextCol = currentCol->pNext;
            // Nota: Los nodos ya fueron liberados al recorrer las filas
            delete currentCol;
            currentCol = nextCol;
        }

        delete Matrix;
    }

    // Verifica si existe un elemento en la posición (iI, iJ)
    bool isElementOf(int iI, int iJ) {
        NodeR<T>* row = Matrix->pFirstR;
        while (row != nullptr && row->iIdR < iI) {
            row = row->pNext;
        }
        
        if (row == nullptr || row->iIdR != iI) return false;
        
        Node<T>* node = row->pFirst;
        while (node != nullptr && node->iIdC < iJ) {
            node = node->pNextR;
        }
        
        return (node != nullptr && node->iIdC == iJ);
    }

    // Inserta un elemento en la posición (iI, iJ)
    void Insert(T tInfo, int iI, int iJ) {
        NodeR<T>* row = getOrCreateRow(iI);
        NodeC<T>* col = getOrCreateCol(iJ);
        
        // Insertar en fila
        Node<T>** pRow = &(row->pFirst);
        while (*pRow != nullptr && (*pRow)->iIdC < iJ) {
            pRow = &((*pRow)->pNextR);
        }
        
        // Insertar en columna
        Node<T>** pCol = &(col->pFirst);
        while (*pCol != nullptr && (*pCol)->iIdF < iI) {
            pCol = &((*pCol)->pNextC);
        }
        
        // Si ya existe, actualizar valor
        if (*pRow != nullptr && (*pRow)->iIdC == iJ) {
            (*pRow)->tInfo = tInfo;
            return;
        }
        
        // Crear nuevo nodo
        Node<T>* newNode = new Node<T>(tInfo, iI, iJ);
        
        // Enlazar en fila
        newNode->pNextR = *pRow;
        *pRow = newNode;
        
        // Enlazar en columna
        newNode->pNextC = *pCol;
        *pCol = newNode;
    }

    // Elimina un elemento en la posición (iI, iJ)
    void Delete(int iI, int iJ) {
        NodeR<T>* row = Matrix->pFirstR;
        while (row != nullptr && row->iIdR < iI) {
            row = row->pNext;
        }
        
        if (row == nullptr || row->iIdR != iI) return;
        
        // Buscar en fila
        Node<T>** pRow = &(row->pFirst);
        while (*pRow != nullptr && (*pRow)->iIdC < iJ) {
            pRow = &((*pRow)->pNextR);
        }
        
        if (*pRow == nullptr || (*pRow)->iIdC != iJ) return;
        
        Node<T>* nodeToDelete = *pRow;
        
        // Eliminar de fila
        *pRow = nodeToDelete->pNextR;
        
        // Buscar en columna
        NodeC<T>* col = Matrix->pFirstC;
        while (col != nullptr && col->iIdC < iJ) {
            col = col->pNext;
        }
        
        if (col == nullptr || col->iIdC != iJ) {
            delete nodeToDelete;
            return;
        }
        
        Node<T>** pCol = &(col->pFirst);
        while (*pCol != nullptr && (*pCol) != nodeToDelete) {
            pCol = &((*pCol)->pNextC);
        }
        
        if (*pCol == nullptr) {
            delete nodeToDelete;
            return;
        }
        
        // Eliminar de columna
        *pCol = nodeToDelete->pNextC;
        
        delete nodeToDelete;
    }

    // Método auxiliar para imprimir la matriz (no requerido en la especificación)
    void Print() {
        NodeR<T>* row = Matrix->pFirstR;
        while (row != nullptr) {
            Node<T>* node = row->pFirst;
            std::cout << "Fila " << row->iIdR << ": ";
            while (node != nullptr) {
                std::cout << "(" << node->iIdC << ", " << node->tInfo << ") ";
                node = node->pNextR;
            }
            std::cout << std::endl;
            row = row->pNext;
        }
    }
};

int main() {
    SparseMatrix<int> matrix;
    
    // Insertar elementos
    matrix.Insert(10, 0, 0);
    matrix.Insert(20, 0, 2);
    matrix.Insert(30, 1, 1);
    matrix.Insert(40, 2, 2);
    
    std::cout << "Matriz inicial:" << std::endl;
    matrix.Print();
    
    // Verificar existencia
    std::cout << "\nElemento en (0,0): " << matrix.isElementOf(0, 0) << std::endl; // true (1)
    std::cout << "Elemento en (1,1): " << matrix.isElementOf(1, 1) << std::endl; // true (1)
    std::cout << "Elemento en (3,3): " << matrix.isElementOf(3, 3) << std::endl; // false (0)
    
    // Actualizar elemento
    matrix.Insert(99, 0, 0);
    std::cout << "\nDespués de actualizar (0,0):" << std::endl;
    matrix.Print();
    
    // Eliminar elemento
    matrix.Delete(1, 1);
    std::cout << "\nDespués de eliminar (1,1):" << std::endl;
    matrix.Print();
    
    // Insertar en nueva fila/columna
    matrix.Insert(50, 3, 3);
    std::cout << "\nDespués de insertar (3,3):" << std::endl;
    matrix.Print();
    
    return 0;
}