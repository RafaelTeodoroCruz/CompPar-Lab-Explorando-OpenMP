#!/usr/bin/env bash
# Rafael Teodoro Cruz - RA: 10723258
# Coleta 3 repeticoes para cada versao com 1 a 6 threads.
set -euo pipefail

N="${1:-768}"
BLOCO="${2:-32}"
ARQUIVO="resultados_exercicio3.csv"

# Evita thread pools externos interferindo no benchmark.
export OMP_DYNAMIC=FALSE

printf 'versao,threads,n,bloco,repeticao,tempo,checksum,validacao\n' > "$ARQUIVO"
for versao in sequencial paralelo; do
    for threads in 1 2 3 4 5 6; do
        for repeticao in 1 2 3; do
            saida=$("./exercicio3_${versao}" "$N" "$threads" "$BLOCO")
            IFS=',' read -r versao_lida threads_lidas n_lido bloco_lido tempo checksum validacao <<< "$saida"
            if [ "$validacao" != "OK" ]; then
                echo "Falha de validacao: $saida" >&2
                exit 1
            fi
            linha="$versao_lida,$threads_lidas,$n_lido,$bloco_lido,$repeticao,$tempo,$checksum,$validacao"
            printf '%s\n' "$linha" | tee -a "$ARQUIVO"
        done
    done
done
printf '\nConcluido. CSV salvo em: %s\n' "$ARQUIVO"
echo 'Na versao sequencial, o campo threads e apenas uma configuracao informativa (o codigo e serial).'
