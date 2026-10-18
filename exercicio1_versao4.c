
#include <stdio.h>
#include <omp.h>

// Rafael Teodoro Cruz - RA: 10723258
// Exercicio 1 - Versao 4: Distribuicao automatica com omp for

double f(double x) {
    return x * x;
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
        double parcial = 0.0;

        #pragma omp for
        for (int i = 1; i < n; i++) {
            double x = a + i * h;
            parcial += f(x);
        }

        #pragma omp critical
        {
            soma += parcial;
        }
    }

    double integral = h * ((f(a) + f(b)) / 2.0 + soma);

    printf("Integral aproximada: %.10f\n", integral);

    return 0;
}
