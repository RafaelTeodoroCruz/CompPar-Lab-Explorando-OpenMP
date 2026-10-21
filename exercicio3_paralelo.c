// Rafael Teodoro Cruz - RA: 10723258
// Exercicio 3 - Multiplicacao de matrizes paralela com OpenMP e blocagem (cache L1/L2)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

static void inicializar(double *a, double *b, double *c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            size_t pos = (size_t)i * n + j;
            a[pos] = ((i + j) % 11 + 1) / 11.0;
            b[pos] = ((3 * i + j) % 13 + 1) / 13.0;
            c[pos] = 0.0;
        }
    }
}

static void multiplicar(double *a, double *b, double *c, int n, int bloco, int threads) {
    // Cada thread recebe grupos distintos de linhas de C.
    // Nao existe escrita concorrente na mesma posicao da matriz C.
    #pragma omp parallel for num_threads(threads) schedule(static)
    for (int ii = 0; ii < n; ii += bloco) {
        int fim_i = (ii + bloco < n) ? ii + bloco : n;
        for (int kk = 0; kk < n; kk += bloco) {
            int fim_k = (kk + bloco < n) ? kk + bloco : n;
            for (int jj = 0; jj < n; jj += bloco) {
                int fim_j = (jj + bloco < n) ? jj + bloco : n;
                for (int i = ii; i < fim_i; i++) {
                    for (int k = kk; k < fim_k; k++) {
                        double valor = a[(size_t)i * n + k];
                        for (int j = jj; j < fim_j; j++) {
                            c[(size_t)i * n + j] += valor * b[(size_t)k * n + j];
                        }
                    }
                }
            }
        }
    }
}

static double conferir(double *a, double *b, double *c, int n) {
    double checksum = 0.0;
    for (size_t p = 0; p < (size_t)n * n; p++) {
        checksum += c[p];
    }
    double esperado = 0.0;
    for (int k = 0; k < n; k++) {
        esperado += a[k] * b[(size_t)k * n];
    }
    if (fabs(c[0] - esperado) > 1e-8 * (1.0 + fabs(esperado))) {
        fprintf(stderr, "Erro na validacao da matriz.\n");
        return -1.0;
    }
    return checksum;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <tamanho> <threads> <bloco>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int threads = atoi(argv[2]);
    int bloco = atoi(argv[3]);
    if (n < 1 || n > 4096 || threads < 1 || threads > 6 || bloco < 1 || bloco > n) {
        fprintf(stderr, "Parametros invalidos: 1<=n<=4096, 1<=threads<=6, 1<=bloco<=n.\n");
        return 1;
    }

    size_t elementos = (size_t)n * n;
    double *a = malloc(elementos * sizeof(double));
    double *b = malloc(elementos * sizeof(double));
    double *c = malloc(elementos * sizeof(double));
    if (!a || !b || !c) {
        fprintf(stderr, "Memoria insuficiente.\n");
        free(a); free(b); free(c);
        return 1;
    }

    inicializar(a, b, c, n);
    double inicio = omp_get_wtime();
    multiplicar(a, b, c, n, bloco, threads);
    double tempo = omp_get_wtime() - inicio;

    double checksum = conferir(a, b, c, n);
    if (checksum < 0.0) {
        free(a); free(b); free(c);
        return 2;
    }
    printf("paralelo,%d,%d,%d,%.9f,%.6f,OK\n", threads, n, bloco, tempo, checksum);
    free(a); free(b); free(c);
    return 0;
}
