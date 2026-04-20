#!/usr/bin/env bash
set -euo pipefail

OUT_DIR="experiment_results"
CSV_FILE="$OUT_DIR/results.csv"
SIM="./bin/sim"

mkdir -p "$OUT_DIR"

echo "predictor,benchmark,m,n,k,m2,mispred_rate" > "$CSV_FILE"

benchmarks=("gcc_trace.txt" "jpeg_trace.txt")

# bimodal: 7 <= m <= 12
for bench in "${benchmarks[@]}"; do
    for m in 7 8 9 10 11 12; do
        output="$($SIM bimodal "$m" "traces/$bench")"
        rate=$(echo "$output" | awk '/misprediction rate:/ {gsub("%","",$3); print $3}')
        echo "bimodal,$bench,$m,,,,$rate" >> "$CSV_FILE"
        echo "bimodal $bench m=$m rate=$rate"
    done
done

# gshare: 7 <= m <= 12, 2 <= n <= m, n even
for bench in "${benchmarks[@]}"; do
    for m in 7 8 9 10 11 12; do
        for n in 2 4 6 8 10 12; do
            if (( n <= m )); then
                output="$($SIM gshare "$m" "$n" "traces/$bench")"
                rate=$(echo "$output" | awk '/misprediction rate:/ {gsub("%","",$3); print $3}')
                echo "gshare,$bench,$m,$n,,,$rate" >> "$CSV_FILE"
                echo "gshare $bench m=$m n=$n rate=$rate"
            fi
        done
    done
done

echo "saved csv to $CSV_FILE"
