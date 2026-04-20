#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VALIDATION_DIR="$ROOT_DIR/validation_runs"
TMP_DIR="$ROOT_DIR/.validation_tmp"

mkdir -p "$TMP_DIR"

pass_count=0
fail_count=0

for valfile in "$VALIDATION_DIR"/*.txt; do
    filename="$(basename "$valfile")"

    if [[ "$filename" == val_smith_* ]]; then
        continue
    fi

    cmd_line="$(awk '
        $0 == "COMMAND" { getline; print; exit }
    ' "$valfile")"

    if [[ -z "$cmd_line" ]]; then
        echo "FAIL  $filename"
        echo "  could not find COMMAND section"
        ((fail_count++))
        continue
    fi

    cmd_line="${cmd_line/#.\/sim/.\/bin\/sim}"

    trace_name="$(awk '{print $NF}' <<< "$cmd_line")"
    if [[ "$trace_name" != traces/* ]]; then
        cmd_line="${cmd_line% $trace_name} traces/$trace_name"
    fi

    expected_file="$TMP_DIR/${filename%.txt}_expected.txt"
    actual_file="$TMP_DIR/${filename%.txt}_actual.txt"
    diff_file="$TMP_DIR/${filename%.txt}_diff.txt"

    #include OUTPUT line aswel
    awk '
        $0 == "OUTPUT" { found_output=1 }
        found_output { print }
    ' "$valfile" > "$expected_file"

    echo "RUN   $filename"
    echo "  $cmd_line"

    (
        cd "$ROOT_DIR" || exit 1
        eval "$cmd_line"
    ) > "$actual_file" 2>&1

    if diff -iw "$actual_file" "$expected_file" > "$diff_file"; then
        echo "PASS  $filename"
        ((pass_count++))
    else
        echo "FAIL  $filename"
        echo "  diff saved to $diff_file"
        ((fail_count++))
    fi

    echo
done

echo "Summary: $pass_count passed, $fail_count failed"
