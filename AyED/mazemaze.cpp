#include <iostream>
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

int dx_CxF[] = {0, 1, 0, -1};
int dy_CxF[] = {1, 0, -1, 0};
int dx_FxC[] = {1, 0, -1, 0};
int dy_FxC[] = {0, 1, 0, -1};

int** crearLaberinto(int N, double prob_muro = 0.3){
    int** laberinto = new int*[N];
    for(int i = 0; i < N; i++){
        laberinto[i] = new int[N];
        for(int j = 0; j < N; j++){
            if ((i == 0 && j == 0) || (i == N-1 && j == N-1))
                laberinto[i][j] = 0;
            else
                laberinto[i][j] = (rand() < prob_muro * RAND_MAX) ? 1 : 0;
        }
    }
    return laberinto;
}

bool recorrerCxF(int** maze, int N, int x, int y, int numCelda, int& llamadas){

    maze[x][y] = 5;

    if(N-1 == y && N-1 == x){
        cout << "Cantidad de celdas: " << numCelda << endl;
        cout << "Llamadas: " << llamadas << endl;
        for(int i = 0; i < N; i ++){
            for(int j = 0; j < N; j++){
                cout << maze[i][j];
            } cout << endl;
        } return true;
    }

    for(int i = 0; i < 4; i ++){
        llamadas++;

        int siguienteX = x + dx_CxF[i];
        int siguienteY = y + dy_CxF[i];

        if(valido(siguienteX, siguienteY, N) && maze[siguienteX][siguienteY] == 0){
            if(recorrerCxF(maze, N, siguienteX, siguienteY, numCelda + 1,  llamadas)){
                return true;
            }
        }
        
    }
    maze[x][y] = 0;
    return false;
}

bool recorrerFxC(int** maze, int N, int x, int y, int numCelda, int& llamadas){

    maze[x][y] = 5;

    if(N-1 == y && N-1 == x){
        cout << "Cantidad de celdas: " << numCelda << endl;
        cout << "Llamadas: " << llamadas << endl;
        for(int i = 0; i < N; i ++){
            for(int j = 0; j < N; j++){
                cout << maze[i][j];
            } cout << endl;
        } return true;
    }

    for(int i = 0; i < 4; i ++){
        llamadas++;

        int siguienteX = x + dx_FxC[i];
        int siguienteY = y + dy_FxC[i];

        if(valido(siguienteX, siguienteY, N) && maze[siguienteX][siguienteY] == 0){
            if(recorrerFxC(maze, N, siguienteX, siguienteY, numCelda + 1,  llamadas)){
                return true;
            }
        }
        
    }
    maze[x][y] = 0;
    return false;
}

int main(){
    int N1 = 10, N2 = 20, N3 = 50, K1 = 1, K2 = 10, K3 = 100, x = 0, y = 0, numCelda = 1, siguienteX = 0, siguienteY = 0, llamadas = 0;
    srand(time(0));

    double promedioTime = 0.0;
    double aux = 3.0;

    auto startTime = high_resolution_clock::now();
    int** miLaberinto1 = crearLaberinto(N1);
    for(int i = 0; i < K1; i++){
        cout << recorrerCxF(miLaberinto1, N1, x, y, numCelda, llamadas) << endl;
        cout << endl;
        cout << recorrerFxC(miLaberinto1, N1, x, y, numCelda, llamadas) << endl;
        cout << endl;
    }
    // Final de Medición de tiempo
    auto endTime = high_resolution_clock::now();
    duration<float, milli> duration = endTime - startTime;
    double finalTime = duration.count();

    promedioTime += finalTime;

    auto startTime2 = high_resolution_clock::now();
    int** miLaberinto2 = crearLaberinto(N2);
    for(int i = 0; i < K2; i++){
        cout << recorrerCxF(miLaberinto2, N2, x, y, numCelda, llamadas) << endl;
        cout << endl;
        cout << recorrerFxC(miLaberinto2, N2, x, y, numCelda, llamadas) << endl;
        cout << endl;
    }
    // Final de Medición de tiempo
    auto endTime2 = high_resolution_clock::now();
    chrono::duration<float, milli> duration2 = endTime2 - startTime2;
    double finalTime2 = duration2.count();
    cout << "Tiempo de ejecución (CxF): " << finalTime2 << " ms" << endl;
    cout << endl;
    promedioTime += finalTime2;
    
    auto startTime3 = high_resolution_clock::now();
    int** miLaberinto3 = crearLaberinto(N3);
    for(int i = 0; i < K2; i++){
        cout << recorrerCxF(miLaberinto3, N3, x, y, numCelda, llamadas) << endl;
        cout << endl;
        cout << recorrerFxC(miLaberinto3, N3, x, y, numCelda, llamadas) << endl;
        cout << endl;
    }
    // Final de Medición de tiempo
    auto endTime3 = high_resolution_clock::now();
    chrono::duration<float, milli> duration3 = endTime3 - startTime3;
    double finalTime3 = duration3.count();
    cout << "Tiempo de ejecución (CxF): " << finalTime3 << " ms" << endl;
    cout << endl;
    promedioTime += finalTime3;


    cout << "Tiempo de ejecución Promedio: " << promedioTime/aux << " ms" << endl;
    cout << endl;
    
    for (int i = 0; i < N1; i++){
        delete[] miLaberinto1[i];
    } 
    for (int i = 0; i < N2; i++){
        delete[] miLaberinto2[i];
    } 
    for (int i = 0; i < N3; i++){
        delete[] miLaberinto3[i];
    } 
    delete[] miLaberinto1;
    delete[] miLaberinto2;
    delete[] miLaberinto3;

    return 0;
}

