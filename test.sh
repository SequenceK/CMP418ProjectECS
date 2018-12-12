#!/bin/bash

killfunc() {
    echo >&2 "$@"
    exit 1
}

[ "$#" -eq 3 ] || killfunc "run with: test.sh MAX_THREADS NUM_ITERATIONS NUM_ENTITIES"

echo "Testing multi with max ${1} threads, ${2} iterations and ${3} entities"

e=${3}

> data_${e}.csv
for((thr=1;thr<=${1};thr++))
do
    echo -n "${thr}, ${2}, ${e}" >> data_${e}.csv
    ./multi ${thr} ${2} ${e} >> data_${e}.csv
done
