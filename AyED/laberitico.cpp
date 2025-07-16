#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>
#include <cmath>

using namespace std;
using namespace chrono;

bool valido(int x, int y, int N){
    return (x < N && x >= 0 && y < N && y >= 0);
}

int dx[] = {0, 0, -1, 1};
int dy[] = {-1, 1, 0, 0};

bool recorrer(int maze[10][10], int N, int x, int y, int numCelda, int llamadas){

    maze[x][y] = 5;

    if(9 == y && 9 == x){
        cout << "Cantidad de celdas: " << numCelda << endl;

        for(int i = 0; i < N; i ++){
            for(int j = 0; j < N; j++){
                cout << maze[i][j];
            } cout << endl;
        } return true;
    }

    for(int i = 0; i < 4; i ++){

        llamadas++; //Cuenta las llamada recursivas

        int siguienteX = x + dx[i];
        int siguienteY = y + dy[i];

        if(maze[siguienteX][siguienteY] == 0 && valido(siguienteX, siguienteY, N)){
            if(recorrer(maze, N, siguienteX, siguienteY, numCelda + 1,  llamadas)){
                return true;
            }
        }
        
    }
    maze[x][y] = 0;
    return false;

}

int main(){

    int N = 10, x = 0, y = 0, numCelda = 1, siguienteX = 0, siguienteY = 0, llamadas = 0;

    int maze[10][10] = {{0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
                {1, 1, 0, 0, 1, 1, 1, 1, 0, 1},
                {1, 1, 1, 0, 0, 0, 1, 1, 1, 1},
                {1, 1, 1, 0, 0, 0, 0, 1, 1, 1},
                {1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                {1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
                {1, 0, 0, 1, 1, 1, 0, 0, 1, 0},
                {1, 0, 0, 0, 1, 1, 1, 1, 1, 0},
                {0, 0, 1, 0, 1, 1, 1, 0, 0, 0},
                {1, 0, 0, 1, 1, 0, 0, 1, 1, 0}};

    cout << recorrer(maze, N, x, y, numCelda, llamadas);

    system("pause");
    return 0;
}
