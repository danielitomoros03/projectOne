#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>
#include <cmath>

using namespace std;
using namespace chrono;


// Estructura para almacenar las métricas de rendimiento
struct PerformanceMetrics {
    double executionTime;
    int recursiveCalls;
    int pathLength;
};

// Estructura para representar una posición en el laberinto
struct Position {
    int row;
    int col;
};

// Clase para el laberinto
class Maze {
private:
    vector<vector<int>> grid;
    int size;
    Position start;
    Position end;

public:
    // Constructor que recibe el tamaño N
    Maze(int n) : size(n) {
        // Inicializar la matriz con paredes (1)
        grid.resize(size, vector<int>(size, 1));
        
        // Generar un laberinto aleatorio
        generateMaze();
    }

    // Método para generar un laberinto aleatorio
    void generateMaze() {
        // Semilla para números aleatorios
        srand(static_cast<unsigned int>(time(nullptr)));
        
        // Establecer punto de entrada (esquina superior izquierda)
        start.row = 0;
        start.col = 0;
        grid[start.row][start.col] = 0;  // 0 representa camino libre
        
        // Establecer punto de salida (esquina inferior derecha)
        end.row = size - 1;
        end.col = size - 1;
        grid[end.row][end.col] = 0;
        
        // Generar caminos aleatorios para asegurar que exista al menos una solución
        generatePath();
        
        // Añadir caminos adicionales para hacer el laberinto más interesante
        addRandomPaths();
    }

    // Método para generar un camino garantizado entre entrada y salida
    void generatePath() {
        Position current = start;
        
        // Mientras no lleguemos al final
        while (current.row != end.row || current.col != end.col) {
            // Decidir aleatoriamente si moverse horizontal o verticalmente
            bool moveHorizontal = (rand() % 2 == 0);
            
            if (moveHorizontal && current.col < end.col) {
                // Mover a la derecha
                current.col++;
            } else if (!moveHorizontal && current.row < end.row) {
                // Mover hacia abajo
                current.row++;
            } else if (current.col < end.col) {
                // Si no podemos movernos en la dirección elegida, intentamos la otra
                current.col++;
            } else {
                current.row++;
            }
            
            // Marcar la celda como camino
            grid[current.row][current.col] = 0;
        }
    }

    // Método para añadir caminos aleatorios adicionales
    void addRandomPaths() {
        // Número de caminos adicionales proporcional al tamaño del laberinto
        int additionalPaths = size * 2;
        
        for (int i = 0; i < additionalPaths; i++) {
            int row = rand() % size;
            int col = rand() % size;
            
            // Si es una pared, convertirla en camino
            if (grid[row][col] == 1) {
                grid[row][col] = 0;
                
                // Asegurar que este nuevo camino se conecte con algún camino existente
                connectToExistingPath(row, col);
            }
        }
    }

    // Método para conectar un punto con un camino existente
    void connectToExistingPath(int row, int col) {
        // Direcciones: arriba, abajo, izquierda, derecha
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        
        // Elegir una dirección aleatoria
        int dir = rand() % 4;
        
        // Intentar conectar en esa dirección
        for (int i = 0; i < 4; i++) {
            int newDir = (dir + i) % 4;
            int newRow = row + dr[newDir];
            int newCol = col + dc[newDir];
            
            // Verificar límites
            if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
                // Si encontramos un camino, conectamos
                if (grid[newRow][newCol] == 0) {
                    return;
                }
                // Si es una pared, la convertimos en camino y terminamos
                grid[newRow][newCol] = 0;
                return;
            }
        }
    }

    // Método para imprimir el laberinto
    void printMaze() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == start.row && j == start.col) {
                    cout << "E ";  // Entrada
                } else if (i == end.row && j == end.col) {
                    cout << "S ";  // Salida
                } else {
                    cout << grid[i][j] << " ";
                }
            }
            cout << endl;
        }
    }

    // Getters
    const vector<vector<int>>& getGrid() const {
        return grid;
    }

    Position getStart() const {
        return start;
    }

    Position getEnd() const {
        return end;
    }

    int getSize() const {
        return size;
    }
};


void recorrerMatrizFxC(int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            // matriz[i][j];
        }
    }
}
void recorrerMatrizCxF(int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            // matriz[i][j];
        }
    }
}

int main(){
    auto startTime = high_resolution_clock::now();
    // Codigo a medir


    // Indicar punto de entrada y de salida en la matriz ?
    int N, K;
    cout << "Ingrese el tamaño de la matriz (N): ";
    cin >> N;
    cout << "Ingrese el número de repeticiones (K): ";
    cin >> K;
    
    // Validar entradas
    if (N <= 0 || K <= 0) {
        cout << "Error: N y K deben ser valores positivos." << endl;
        return 1;
    }
    
    // Crear el laberinto
    Maze maze(N);
    
    // Mostrar el laberinto generado
    cout << "\nLaberinto generado (E=Entrada, S=Salida, 0=Camino, 1=Pared):\n" << endl;
    maze.printMaze();

    auto endTime = high_resolution_clock::now();
    duration<float, milli> duration = endTime - startTime;
    double finalTime = duration.count();

    return 0;
}

