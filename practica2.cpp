/*  Dado un arreglo de N números enteros, diseñar algoritmos recursivos que calculen:
        ● El mayor elemento del arreglo. 
        ● La suma de los elementos del arreglo. listo
        ● La media de todos los elementos del arreglo. listo
        ● Verificar si el arreglo está ordenado
*/

int sumaRecursiva(int* arr, int N){
    if (N==0) return arr[0];
    return sumaRecursiva(arr, N-1) + arr[N];
}
float promedioArr(int* arr, int N){
    return (float)sumaRecursiva(arr, N-1)/(float)N;
}
void swap(int& A, int& B){
    int x = A;
    A = B;
    B = x;
}
void recorrerArr(int* arr, int N, int indice=0){
    if (indice>=N) cout << arr[N] << endl;
    else{ cout << arr[indice] << " "; recorrerArr(arr, N, indice+1); } 
}
int mayorElemento(int* arr, int indice, int N, int mayor){
    if (indice == N) {
        return mayor;
    }
    int nuevoMayor = mayor;
    if(arr[indice] > mayor) nuevoMayor = arr[indice];
    return mayorElemento(arr, indice+1, N, nuevoMayor);
}
int mayorElementoI(int* arr, int N){
    int mayor = arr[0];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (arr[i] > mayor) {
                mayor = arr[i];
            }
        }
    }
    return mayor;
}
int menorElemento(int* arr, int indice, int N, int menor){
    if (indice == N) {
        return menor;
    }
    int nuevoMenor = menor;
    if(arr[indice] < menor) nuevoMenor = arr[indice];
    return menorElemento(arr, indice+1, N, nuevoMenor);
}
void arrOrdenado(int* arr, int n, int indice){
    if(indice >= n-1) return;
    int aux = menorElemento(arr, indice, n, arr[indice]);
    for(int i=0;i<n+1;i++){
        if(aux==arr[i]) swap(arr[i], arr[indice]);
    }
    arrOrdenado(arr, n, indice+1);
}
