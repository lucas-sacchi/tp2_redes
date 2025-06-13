#!/bin/bash

OUTPUT_FILE="ab_results_threads.txt"
URL="http://localhost:8080/testeindex.html"

echo "Resultados dos testes AB - Iterativo" > $OUTPUT_FILE
echo "-----------------------------------" >> $OUTPUT_FILE

CONCURRENCIES=(1 2 5 10 20 30 40 50 75 100 150 200 300 400 500)

TOTAL_REQUESTS=(1000 2000 3000 4000 5000 7500 10000)

for N in "${TOTAL_REQUESTS[@]}"; do
    for C in "${CONCURRENCIES[@]}"; do
        echo "Teste: -n $N -c $C" >> $OUTPUT_FILE
        echo "-----------------------------------" >> $OUTPUT_FILE
        
        ab -n $N -c $C $URL 2>&1 | grep -E "Requests per second:|Time per request:|Failed requests" >> $OUTPUT_FILE
        
        echo "" >> $OUTPUT_FILE
    done
done

echo "Todos os testes foram executados. Resultados em $OUTPUT_FILE"
