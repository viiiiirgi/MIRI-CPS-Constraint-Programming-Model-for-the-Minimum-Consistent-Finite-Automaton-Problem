#!/bin/bash

SOLVER="./finite-automaton"
INSTANCE_DIR="../instances"
OUTPUT_DIR="../out"
TIMEOUT_SECONDS=60
CHECKER_SCRIPT="./checker"


mkdir -p "$OUTPUT_DIR"

process_count=0
success_count=0
timeout_count=0
error_count=0
for infile in "$INSTANCE_DIR"/*.inp; do
  if [ -f "$infile" ]; then
    base=$(basename "$infile" .inp)

    outfile="$OUTPUT_DIR/${base}.out"

    echo "Processing '$infile' -> '$outfile'"

    timeout "$TIMEOUT_SECONDS" "$SOLVER" < "$infile" > "$outfile"
    status=$?

    process_count=$((process_count + 1))
    if [ $status -eq 0 ]; then
      echo "  Finished successfully for '$base.inp'."
      success_count=$((success_count + 1))
    elif [ $status -eq 124 ]; then
      echo "  TIMED OUT for '$base.inp' after ${TIMEOUT_SECONDS}s."
      timeout_count=$((timeout_count + 1))
      rm -f "$outfile"
    else
      echo "  ERROR: exited with status $status for '$base.inp'."
      error_count=$((error_count + 1))
      rm -f "$outfile"
    fi
  fi
done

echo "Finished processing $process_count instances."
echo "Summary: Success=$success_count, Timeout=$timeout_count, Error=$error_count"

echo "Starting checker script '$CHECKER_SCRIPT' on outputs in '$OUTPUT_DIR/'..."
checked_count=0
valid_count=0
invalid_count=0

for outfile in "$OUTPUT_DIR"/*.out; do
   if [ -f "$outfile" ]; then
     checked_count=$((checked_count + 1))
     base_out=$(basename "$outfile")
     echo "Checking '$outfile'..."

     if "$CHECKER_SCRIPT" < "$outfile"; then
       echo "  Check PASSED for '$base_out'."
       valid_count=$((valid_count + 1))
     else
       echo "  Check FAILED for '$base_out'."
       invalid_count=$((invalid_count + 1))
     fi
   fi
done

echo "Finished checking $checked_count output files."
echo "Check Summary: Valid=$valid_count, Invalid=$invalid_count"

exit 0
