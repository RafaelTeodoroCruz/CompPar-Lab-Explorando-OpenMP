
#include <stdio.h>

// Rafael Teodoro Cruz - RA: 10723258
// Exercicio 1 - Versao 1: Regra do trapezio sequencial

double f(double x) {
    return x * x;
}

int main() {
    double a = 0.0;
    double b = 1.0;
    int n = 1000000;

    double h = (b - a) / n;
    double aprox = (f(a) + f(b)) / 2.0;

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        aprox += f(x);
    }

    aprox *= h;

    printf("Integral aproximada: %.10f\n", aprox);

    return 0;
}
