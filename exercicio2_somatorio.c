// Rafael Teodoro Cruz - RA: 10723258
// Exercicio 2 - Somatorio com critical ou reduction

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <critical|reduction> <threads> <expoente>\n", argv[0]);
        return 1;
    }

    int threads = atoi(argv[2]);
    int expoente = atoi(argv[3]);

    if (threads < 1 || expoente < 1 || expoente > 30) {
        return 1;
    }

    size_t n = (size_t)1 << expoente;
    int *v = malloc(n * sizeof(int));

    if (!v) {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        v[i] = (int)(i % 10) + 1;
    }

    long long soma = 0;
    double inicio = omp_get_wtime();

    if (strcmp(argv[1], "critical") == 0) {
        #pragma omp parallel num_threads(threads)
        {
            long long parcial = 0;

            #pragma omp for
            for (size_t i = 0; i < n; i++) {
                parcial += v[i];
            }

            #pragma omp critical
            soma += parcial;
        }
    } else if (strcmp(argv[1], "reduction") == 0) {
        #pragma omp parallel for num_threads(threads) reduction(+:soma)
        for (size_t i = 0; i < n; i++) {
            soma += v[i];
        }
    } else {
        free(v);
        return 1;
    }

    double tempo = omp_get_wtime() - inicio;

    printf("%s,%d,%zu,%.9f,%lld\n", argv[1], threads, n, tempo, soma);

    free(v);
    return 0;
}
