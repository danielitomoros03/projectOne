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
public:
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


class MazeSolver {
private:
    Maze& maze; // Referencia al objeto Maze
    int N; // Tamaño del laberinto
    Position start;
    Position end;

    // Direcciones para el recorrido columna por columna (CxF)
    int dx_CxF[4] = {0, 1, 0, -1};
    int dy_CxF[4] = {1, 0, -1, 0};

    // Direcciones para el recorrido fila por fila (FxC)
    int dx_FxC[4] = {1, 0, -1, 0};
    int dy_FxC[4] = {0, 1, 0, -1};

    // Función auxiliar para verificar si una posición es válida dentro del laberinto
    bool valido(int x, int y, int N) {
        return (x < N && x >= 0 && y < N && y >= 0);
    }

public:
    // Constructor
    MazeSolver(Maze& m) : maze(m) {
        N = maze.getSize();
        start = maze.getStart();
        end = maze.getEnd();
    }

    // Función para resolver el laberinto columna por columna (recorrerCxF)
    bool recorrerCxF(vector<vector<int>>& current_maze, int x, int y, int numCelda, int& llamadas) {
        // Marca la celda actual como visitada (parte del camino)
        current_maze[x][y] = 5;

        // Si hemos llegado al final
        if (end.row == x && end.col == y) {
            cout << "Cantidad de celdas (CxF): " << numCelda << endl;
            cout << "Llamadas (CxF): " << llamadas << endl;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    cout << current_maze[i][j] << " ";
                }
                cout << endl;
            }
            return true;
        }

        // Explora los vecinos
        for (int i = 0; i < 4; i++) {
            llamadas++; // Incrementa el contador de llamadas por cada intento de movimiento

            int siguienteX = x + dx_CxF[i];
            int siguienteY = y + dy_CxF[i];

            // Verifica si la siguiente posición es válida y es un camino abierto (0)
            if (valido(siguienteX, siguienteY, N) && current_maze[siguienteX][siguienteY] == 0) {
                if (recorrerCxF(current_maze, siguienteX, siguienteY, numCelda + 1, llamadas)) {
                    return true;
                }
            }
        }
        // Si no se encuentra un camino desde esta celda, retrocede desmarcándola
        current_maze[x][y] = 0;
        return false;
    }

    // Función para resolver el laberinto fila por fila (recorrerFxC)
    bool recorrerFxC(vector<vector<int>>& current_maze, int x, int y, int numCelda, int& llamadas) {
        // Marca la celda actual como visitada (parte del camino)
        current_maze[x][y] = 5;

        // Si hemos llegado al final
        if (end.row == x && end.col == y) {
            cout << "Cantidad de celdas (FxC): " << numCelda << endl;
            cout << "Llamadas (FxC): " << llamadas << endl;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    cout << current_maze[i][j] << " ";
                }
                cout << endl;
            }
            return true;
        }

        // Explora los vecinos
        for (int i = 0; i < 4; i++) {
            llamadas++; // Incrementa el contador de llamadas por cada intento de movimiento

            int siguienteX = x + dx_FxC[i];
            int siguienteY = y + dy_FxC[i];

            // Verifica si la siguiente posición es válida y es un camino abierto (0)
            if (valido(siguienteX, siguienteY, N) && current_maze[siguienteX][siguienteY] == 0) {
                if (recorrerFxC(current_maze, siguienteX, siguienteY, numCelda + 1, llamadas)) {
                    return true;
                }
            }
        }
        // Si no se encuentra un camino desde esta celda, retrocede desmarcándola
        current_maze[x][y] = 0;
        return false;
    }
};

int main() {
    int N1 = 10, N2 = 20, N3 = 50;
    // La matriz 'maze' predefinida ya no se utiliza.
    // En su lugar, la clase Maze generará un laberinto aleatorio.
    Maze myMaze1(N1); // Crea un objeto Maze, que generará un laberinto aleatorio de tamaño N1
    Maze myMaze2(N2);
    Maze myMaze3(N3);

    auto startTime = high_resolution_clock::now();
    // Codigo a medir
    cout << "Laberinto Generado Aleatoriamente:" << endl;
    myMaze1.printMaze();
    cout << endl;

    MazeSolver solver(myMaze1); // Crea un objeto MazeSolver, pasando el laberinto generado

    // Resuelve usando recorrerCxF
    cout << "Resolviendo con recorrerCxF:" << endl;
    // Crea una copia del grid del laberinto para que el primer algoritmo no afecte al segundo.
    vector<vector<int>> maze_copy_CxF = myMaze1.getGrid(); 
    int numCelda_CxF = 1;
    int llamadas_CxF = 0;
    if (solver.recorrerCxF(maze_copy_CxF, myMaze1.getStart().row, myMaze1.getStart().col, numCelda_CxF, llamadas_CxF)) {
        cout << "¡Camino encontrado (CxF)!" << endl;
    } else {
        cout << "No se encontró camino (CxF)." << endl;
    }
    // Final de Medición de tiempo
    auto endTime = high_resolution_clock::now();
    duration<float, milli> duration = endTime - startTime;
    double finalTime = duration.count();
    cout << "Tiempo de ejecución (CxF): " << finalTime << " ms" << endl;
    cout << endl;

    auto startTime2 = high_resolution_clock::now();
    // Codigo a medir
    // Resuelve usando recorrerFxC
    cout << "Resolviendo con recorrerFxC:" << endl;
    // Crea otra copia del grid del laberinto para el segundo algoritmo.
    vector<vector<int>> maze_copy_FxC = myMaze1.getGrid(); 
    int numCelda_FxC = 1;
    int llamadas_FxC = 0;
    if (solver.recorrerFxC(maze_copy_FxC, myMaze1.getStart().row, myMaze1.getStart().col, numCelda_FxC, llamadas_FxC)) {
        cout << "¡Camino encontrado (FxC)!" << endl;
    } else {
        cout << "No se encontró camino (FxC)." << endl;
    }
    // Final de Medición de tiempo
    auto endTime2 = high_resolution_clock::now();
    chrono::duration<float, milli> duration2 = endTime2 - startTime2;
    double finalTime2 = duration2.count();
    cout << "Tiempo de ejecución (FxC): " << finalTime2 << " ms" << endl;
    cout << endl;
    
    system("pause");
    return 0;
}
