// Rafael Teodoro Cruz - RA: 10723258
// Exercicio 1 - Regra do trapezio com OpenMP e reduction

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x) {
    return x * x;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <a> <b> <n> <threads>\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = atoi(argv[3]);
    int threads = atoi(argv[4]);

    if (n <= 0 || threads <= 0) {
        return 1;
    }

    double h = (b - a) / n;
    double aprox = (f(a) + f(b)) / 2.0;
    double inicio = omp_get_wtime();

    #pragma omp parallel for num_threads(threads) reduction(+:aprox)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        aprox += f(x);
    }

    aprox *= h;

    printf("Integral aproximada: %.10f\n", aprox);
    printf("Tempo: %.6f s\n", omp_get_wtime() - inicio);

    return 0;
}
