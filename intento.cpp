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

// Función para leer el archivo NumorisDB.in
int leerNumoris(const string& filename, Numor numoris[]) {
    ifstream DB(filename);
    if (!DB.is_open()) {
        cout << "Archivo no encontrado." << endl;
        return -1;
    }

    int N;
    DB >> N; DB.ignore();

    for (int i = 0; i < N; i++) {
        DB >> numoris[i].id >> numoris[i].nombre >> numoris[i].tipo >> numoris[i].dmg >> numoris[i].life;
    }

    DB.close();
    return N; 
}

// Función para leer el archivo TorreX.in
void leerTorre(string& filename, int& pisos, int numoris[]) {
    /*
        int torreNum;
        cin >> torreNum;
        char archivo2[15];
        sprintf(archivo2, "Torre%d.in", torreNum); 
    */
    filename += to_string(pisos); // Agrega el número de torre al nombre del archivo
    filename += ".in"; // Agrega la extensión .in
    
    ifstream torre(filename);
    if(!torre.is_open()){
        cout << "Archivo no encontrado." << endl;
        return -1;
    }

    torre >> pisos; torre.ignore(); //Guarda la cantidad de pisos
    torre.close();
}

int main(){
    int N, pisos, X; // En duda 

    Numor *numoris;
    int NumorisDB = leerNumoris("NumorisDB.in", numoris);
    numoris = new Numor[NumorisDB];

    cin >> X; // X en el archivo TorreX.in
    leerTorre("Torre", X, numoris);

    return 0;
}
