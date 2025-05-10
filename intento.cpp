#include<iostream>
#include<string>
#include<fstream>

using namespace std;

class Numor{

    public:

    int id;
    string nombre;
    string tipo;
    int dmg;
    int life;
}; 


int main(){

    int N, pisos;

    ifstream DB("NumorisDB.in"); 

    if(!DB.is_open()){
        cout << "No encentro el archivo Papi. " << endl;
        return -1;
    } 
    
    DB >> N; DB.ignore();

    Numor numoris[N], solucion[N]; // Para guardar los datos de los numoris, y de los equipos que llegan al final de la torre con vida
    

    for(int i = 0; i < N; i++){
        DB >> numoris[i].id >> numoris[i].nombre >> numoris[i].tipo >> numoris[i].dmg >> numoris[i].life; 
    }

    DB.close();
    int torreNum;
    cin >> torreNum;
    char archivo2[15];

    sprintf(archivo2, "Torre%d.in", torreNum); //Hace lo que te dije en Ws
    ifstream torre(archivo2);

    if(!torre.is_open()){
        cout << "No lo encuentro, mi amor";
        return -1;
    }

    torre >> pisos; torre.ignore(); //Guarda la cantidad de pisos
    
    return 0;
}
