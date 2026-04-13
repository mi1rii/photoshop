#include <stdio.h>
#include <omp.h> // llamamos a la api

#define NUM_THREADS 100 // 4 hilos

int main(){
    omp_set_num_threads(NUM_THREADS); // todo lo de afuera es secuencial

    #pragma omp parallel // todo lo que esté dentro de este bloque se ejecutará en paralelo
    {
        int ID = omp_get_thread_num();
        printf("Procesadores (%d) ", ID);
        printf("_Multiples (%d) ", ID);
        printf("en acción (%d)\n", ID);
    }
}
//clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib
//-I/opt/homebrew/opt/libomp/include -lomp archivo.c
//gcc