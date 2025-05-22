#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Numer {
    public:
        int id;
        string nombre;
        string tipo;
        int dmg;
        int vida;
        int vidaActual;
};
class Equipo{
    public:
        Numer* numoris[6];
        int danioRecibido;
        int numorisEliminados;
};
class Torre{
    public:
        Numer*** pisos;
        int* cantidadPorPiso;
        int pisosTotales;
};

int main(){

    return 0;
}