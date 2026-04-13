#include <stdio.h>
#include <omp.h>

#define MAX_THREADS 100
#define NUM_CASOS 10

double paso;

int main() {
    long long num_pasos = 4e9;
    int threads_prueba[NUM_CASOS] = {
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100
    };

    printf("pasos,threads,pi,tiempo\n");

    for (int caso = 0; caso < NUM_CASOS; caso++) {
        int num_threads = threads_prueba[caso];
        int i, nthreads;
        double pi, sum[MAX_THREADS];

        paso = 1.0 / (double) num_pasos;
        omp_set_num_threads(num_threads);

        const double ST = omp_get_wtime();

        #pragma omp parallel
        {
            long long i;
            int id, nthrds;
            double x;

            id = omp_get_thread_num();
            nthrds = omp_get_num_threads();

            if (id == 0) nthreads = nthrds;

            for (i = id, sum[id] = 0.0; i < num_pasos; i = i + nthrds) {
                x = (i + 0.5) * paso;
                sum[id] += 4.0 / (1.0 + x * x);
            }
        }

        for (i = 0, pi = 0.0; i < nthreads; i++) {
            pi += sum[i] * paso;
        }

        const double STOP = omp_get_wtime();

        printf("%lld,%d,%.15f,%f\n", num_pasos, num_threads, pi, (STOP - ST));
    }

    return 0;
}