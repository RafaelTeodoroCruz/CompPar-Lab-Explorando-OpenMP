
#include <stdio.h>
#include <omp.h>

// Rafael Teodoro Cruz - RA: 10723258
// Exercicio 1 - Versao 3: Funcao local e secao critica

double f(double x) {
    return x * x;
}

double trapezio_local(double a, double h, int n, int id, int total) {
    double parcial = 0.0;

    for (int i = id + 1; i < n; i += total) {
        double x = a + i * h;
        parcial += f(x);
    }

    return parcial;
}

int main() {
    double a = 0.0;
    double b = 1.0;
    int n = 1000000;
    int threads = 4;

    double h = (b - a) / n;
    double soma = 0.0;

    #pragma omp parallel num_threads(threads)
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        double parcial = trapezio_local(a, h, n, id, total);

        #pragma omp critical
        {
            soma += parcial;
        }
    }

    double integral = h * ((f(a) + f(b)) / 2.0 + soma);

    printf("Integral aproximada: %.10f\n", integral);

    return 0;
}
