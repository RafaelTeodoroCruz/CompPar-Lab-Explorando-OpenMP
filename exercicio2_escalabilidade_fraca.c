// Exercicio 2 - Escalabilidade fraca com OpenMP
// Versoes critical e reduction; tamanho exato do vetor

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[3], "n") != 0) {
        fprintf(stderr, "Uso: %s <critical|reduction> <threads> n <quantidade_elementos>\n", argv[0]);
        return 1;
    }

    char *fim_threads;
    char *fim_n;
    errno = 0;
    long t = strtol(argv[2], &fim_threads, 10);
    unsigned long long quantidade = strtoull(argv[4], &fim_n, 10);

    if (errno || *fim_threads != '\0' || *fim_n != '\0' || t < 1 || t > 1024 ||
        quantidade == 0 || quantidade > (1ULL << 30) ||
        (strcmp(argv[1], "critical") != 0 && strcmp(argv[1], "reduction") != 0)) {
        fprintf(stderr, "Parametros invalidos. Maximo: 2^30 elementos.\n");
        return 1;
    }

    int threads = (int)t;
    size_t n = (size_t)quantidade;
    int *vetor = malloc(n * sizeof(*vetor));

    if (vetor == NULL) {
        perror("Falha ao alocar vetor");
        return 1;
    }

    for (size_t i = 0; i < n; i++) {
        vetor[i] = (int)(i % 10) + 1;
    }

    long long soma = 0;
    double inicio = omp_get_wtime();

    if (strcmp(argv[1], "critical") == 0) {
        #pragma omp parallel num_threads(threads)
        {
            long long parcial = 0;

            #pragma omp for
            for (size_t i = 0; i < n; i++) {
                parcial += vetor[i];
            }

            #pragma omp critical
            {
                soma += parcial;
            }
        }
    } else {
        #pragma omp parallel for num_threads(threads) reduction(+:soma)
        for (size_t i = 0; i < n; i++) {
            soma += vetor[i];
        }
    }

    double tempo = omp_get_wtime() - inicio;

    printf("%s,%d,%zu,%.9f,%lld\n", argv[1], threads, n, tempo, soma);

    free(vetor);
    return 0;
}
